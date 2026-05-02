--- Off-Page Cross-Links - Lua Filter
--- modified from Mickaël Canouil's filter
--- https://github.com/mcanouil/quarto-issues-experiments/blob/main/lua-filters/offpage-crosslinks.lua
--- In non-HTML output formats, some relative links to other pages
--- in a Quarto website or book project are broken. This filter rewrites
--- those left-over links to absolute URLs using the project's site-url, so readers
--- can follow them to the live HTML site.
---
--- This filter is intended to run at the `post-quarto` phase, so Quarto's
--- own link resolver runs first. We then only rewrite unresolved *relative*
--- page links that remain.
---
--- Note the limitations: for book projects, since everything is rendered
--- as if it is under the first listed contents file, relative links in later
--- book files may not resolve from their true source location.

-- ============================================================================
-- MODULE-LEVEL VARIABLES
-- ============================================================================

--- @type string|nil The resolved site URL from project metadata
local site_url = nil

--- @type boolean Whether to collapse ".." segments in paths. Currently always enabled.
local collapse_dots = true

-- ============================================================================
-- HELPER FUNCTIONS (PRIVATE)
-- ============================================================================

--- Read site-url from QUARTO_EXECUTE_INFO project metadata.
--- Tries website.site-url first, then book.site-url.
--- Workaround for https://github.com/quarto-dev/quarto-cli/issues/13029
--- where project-level metadata (website/book) is not available in the
--- document Meta passed to Lua filters.
--- @return string|nil The site-url value, or nil if unavailable
local function get_site_url_from_execute_info()
  local path = os.getenv('QUARTO_EXECUTE_INFO')
  if not path then return nil end

  local file = io.open(path, 'r')
  if not file then return nil end

  local content = file:read('*a')
  file:close()

  if not content or content == '' then return nil end

  local ok, info = pcall(quarto.json.decode, content)
  if not ok or not info then return nil end

  local format_meta = info['format'] and info['format']['metadata']
  if not format_meta then return nil end

  if format_meta['website'] and format_meta['website']['site-url'] then
    return format_meta['website']['site-url']
  elseif format_meta['book'] and format_meta['book']['site-url'] then
    return format_meta['book']['site-url']
  end

  return nil
end

--- Read site-url from document metadata as a fallback.
--- Tries website.site-url, then book.site-url.
--- @param meta table The document metadata table
--- @return string|nil The site-url value, or nil if not found
local function get_site_url_from_meta(meta)
  local website = meta['website']
  if website and website['site-url'] then
    return pandoc.utils.stringify(website['site-url'])
  end

  local book = meta['book']
  if book and book['site-url'] then
    return pandoc.utils.stringify(book['site-url'])
  end

  return nil
end

--- Split a link target into path and suffix (
--- query string or fragment identifier).
--- @param target string The original target
--- @return string path, string suffix
local function split_target(target)
  local path, suffix = target:match('^(.-)([?#].*)$')
  return path or target, suffix or ''
end

--- Check whether a link target is a relative cross-page link.
--- Returns true for targets that point to .html or .qmd files
--- without an absolute URI scheme.
--- @param target string The link target URL
--- @return boolean True if the target is a relative page link
local function is_rewriteable_page_link(target)
  if not target or target == '' then return false end
  if target:match('^#') then return false end
  if target:match('^//') then return false end
  if target:match('^%a[%a%d+%-%.]*:') then return false end
  -- if target:match('%.html[#?]') or target:match('%.html$') then return true end
  -- if target:match('%.qmd[#?]') or target:match('%.qmd$') then return true end
  return true
end

--- Collapse ".." segments in a path (e.g., foo/../bar → bar).
--- @param p string The path to collapse
--- @return string The collapsed path
local function collapse_path_dots(p)
  local parts = pandoc.path.split(p)
  local out = {}
  for _, part in ipairs(parts) do
    if part == '..' and #out > 0 and out[#out] ~= '..' then
      out[#out] = nil
    else
      out[#out + 1] = part
    end
  end
  return pandoc.path.join(out)
end

--- Rewrite a relative page link target to an absolute URL.
--- Resolves the target relative to the current input file directory,
--- normalises .qmd extensions to .html, and prepends the site-url.
--- @param target string The original relative link target
--- @param base_url string The site-url to prepend
--- @return string The rewritten absolute URL
local function rewrite_target(target, base_url)
  -- Keep query/fragment intact; only rewrite the path portion.
  local path, suffix = split_target(target)
  -- Note that prior to our script,
  -- Quarto rewrites project-relative links to be current-file-relative,
  -- so the else branch here won't actually happen.
  if pandoc.path.is_relative(path) then
    -- obtain the absolute path of the current input file directory
    local current_file = quarto.doc and quarto.doc.input_file
    if not current_file then return target end
    local current_dir = pandoc.path.directory(current_file)
    -- obtain the absolute path of the project directory
    local project_dir = quarto.project and quarto.project.directory
    if not project_dir then return target end
    -- obtain the absolute path of the target link (possibly with uncollapsed `..`)
    local resolved_path = pandoc.path.normalize(pandoc.path.join({current_dir, path}))
    -- subtract the project directory from the resolved path (possibly with uncollapsed `..`)
    -- should be safe if resolved_path is within the project directory
    path = pandoc.path.make_relative(resolved_path, project_dir)
    -- finally, collapse any `..` segments if enabled
    if collapse_dots then path = collapse_path_dots(path) end
  else
    path = path:gsub('^/', '')
    quarto.log.warning('offpage-crosslinks: unexpected absolute path "%s" in link target "%s"; treating as project-relative', path, target)
  end

  -- Normalize path separators for URLs
  path = path:gsub('\\', '/'):gsub('/%.$', '/'):gsub('^%.$', ''):gsub('^%./', '')
  path = path:gsub('%.qmd$', '.html')

  local separator = base_url:match('/$') and '' or '/'
  return base_url .. separator .. path .. suffix
end

-- ============================================================================
-- FILTER EXPORT
-- ============================================================================

-- Skip if not in a project context
if quarto.project.directory == nil then return nil end

return {
  {
    Meta = function(meta)
      -- Resolve site-url from project metadata.
      site_url = get_site_url_from_execute_info()
      if not site_url then
        site_url = get_site_url_from_meta(meta)
      end
      if not site_url then
        quarto.log.warning('offpage-crosslinks: site-url not found in project metadata.')
      end

      return nil
    end
  },
  {
    -- Rewrite relative cross-page links to absolute URLs.
    Link = function(el)
      if not site_url then return nil end
      if not is_rewriteable_page_link(el.target) then return nil end

      el.target = rewrite_target(el.target, site_url)
      return el
    end
  }
}
