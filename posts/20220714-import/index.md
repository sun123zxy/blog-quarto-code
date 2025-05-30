---
title: "关于 Python 的 import"
abstract: "`__init__.py`, `sys.path`, `python -m` and more..."
date: 2022/07/14
date-modified: 2022/07/14
categories:
  - technical
  - Python
  
format:
  html: default
---

好久以前就被 Python 的相对与绝对导入所困扰。去年粗浅探究后自以为完全理解，近来又因 `sys.path[0]` 和 `os.getcwd()` 的不一致而刷新了认知...

Python 官方文档 [5. The import system — Python 3.10.5 documentation](https://docs.python.org/3/reference/import.html) 当然是最好的学习指南，但全部看完对我来说还是有点难度。这里只选择一些要点讨论。

## from import

`import A`、`import A as B`、`from A import B` 结构中，`A` 最小只能到 module。因此，只有使用 `from import` 结构才可以单独获取 module 里的属性。另外，相对引用必须使用 `from import` 结构。

`from module import *` 将导入 `module` 中的所有成员（有单双下划线前导的成员除外）。对于 package 可在 `__init__.py` 中定义 `__all__ = ["module", "module", ...]` 来手动控制的实际导入内容。

## Package 与 \_\_init\_\_.py

Python 3.3 以后的 package 不再硬性需要 `__init__.py`，普通文件夹等同于 `__init__.py` 留空的 namespace package。（关于 regular package 和 namespace package 的区别，参见 [5. The import system — Python 3.10.5 documentation](https://docs.python.org/3/reference/import.html#regular-packages)）

`__init__.py` 的作用在于当我们直接导入一个 package 的时候，实际上是执行了 `__init__.py`。换句话说，直接导入一个 package 就是把它看做一个逻辑写在 `__init__.py` 里的 module。

需要注意的是，对于形如 `A.B.C` 的导入，`A`、`A.B`、`A.B.C` 对应的 `__init__.py` 都会被执行。也就是说，只要导入路径经过该 package，该 package 的 `__init__.py` 就会被执行。

## Submodules

> When a submodule is loaded using any mechanism (e.g. `importlib` APIs, the `import` or `import-from` statements, or built-in `__import__()`) a binding is placed in the parent module’s namespace to the submodule object. For example, if package `spam` has a submodule `foo`, after importing `spam.foo`, `spam` will have an attribute `foo` which is bound to the submodule.
> 
> ...
> 
> Given Python’s familiar name binding rules this might seem surprising, but it’s actually a fundamental feature of the import system. The invariant holding is that if you have `sys.modules['spam']` and `sys.modules['spam.foo']` (as you would after the above import), the latter must appear as the `foo` attribute of the former.
> 
> — [5. The import system — Python 3.10.5 documentation](https://docs.python.org/3/reference/import.html#submodules)

这是说，import 进来的 module 会被挂载到本 module 上作为其属性。

这个性质可以弄出来很多看上去很奇怪的玩意儿，比如说自己导入自己后可以 `me.me.me.me...` 无限嵌套之类的...

---

另外，对于形如 `import A.B.C` 的导入，`A`、`A.B`、`A.B.C` 都会被挂载到本 module 上。然而，`from A.B import C` 却只会挂载 `C`，而 `import A.B.C as D` 也只会挂载 `D` ，即使 `A`、`A.B` 都被执行且都在 `sys.modules` 里。

## sys.path

> A list of strings that specifies the search path for modules. Initialized from the environment variable [`PYTHONPATH`](https://docs.python.org/3/using/cmdline.html#envvar-PYTHONPATH), plus an installation-dependent default.
> 
> As initialized upon program startup, the first item of this list, `path[0]`, is the directory containing the script that was used to invoke the Python interpreter. If the script directory is not available (e.g. if the interpreter is invoked interactively or if the script is read from standard input), `path[0]` is the empty string, which directs Python to search modules in the current directory first. Notice that the script directory is inserted *before* the entries inserted as a result of [`PYTHONPATH`](https://docs.python.org/3/using/cmdline.html#envvar-PYTHONPATH).
> 
> A program is free to modify this list for its own purposes. Only strings and bytes should be added to [`sys.path`](https://docs.python.org/3/library/sys.html#sys.path "sys.path"); all other data types are ignored during import.
> 
> — [sys — System-specific parameters and functions — Python 3.10.5 documentation](https://docs.python.org/3/library/sys.html#sys.path)

`sys.path` 是 Python 搜索 module 的基准目录（即绝对导入）。其由环境变量 `PYTHONPATH` 和一些默认路径（和安装环境有关，参见 [PYTHONHOME](https://docs.python.org/3/using/cmdline.html#envvar-PYTHONHOME)）组成，而在运行 script 时，**script 的所在目录会被临时加入 `sys.path[0]` 中**。如果运行的并不是 script（例如是交互式运行或从 stdin 中读取脚本代码），**`sys.path[0]` 则被设置为空字符串，代表当前工作目录**。

`sys.path` 有优先级，排在前面的优先级高。

---

需要特别注意的是，**script 的所在目录不是当前工作目录**。例如，在 `D:\test` 下执行

```sh
python path/to/file.py
```

时，`sys.path[0]` 为 `D:\test\path\to\file.py`，而当前工作目录则是 `D:\test`（也即 `os.getcwd()`）。

当前工作目录是 Python 寻找其他文件时的基准路径，而所有绝对导入操作都只与 `sys.path` 有关，两者是完全不同的。

`python -m` 的情况稍有不同，参见后文。

## python -m

> Search [`sys.path`](https://docs.python.org/3/library/sys.html#sys.path "sys.path") for the named module and execute its contents as the [`__main__`](https://docs.python.org/3/library/__main__.html#module-__main__ "__main__: The environment where top-level code is run. Covers command-line interfaces, import-time behavior, and ``__name__ == '__main__'``.") module.
> 
> Since the argument is a *module* name, you must not give a file extension (`.py`). The module name should be a valid absolute Python module name, but the implementation may not always enforce this (e.g. it may allow you to use a name that includes a hyphen).
> 
> Package names (including namespace packages) are also permitted. When a package name is supplied instead of a normal module, the interpreter will execute `<pkg>.__main__` as the main module. This behaviour is deliberately similar to the handling of directories and zipfiles that are passed to the interpreter as the script argument.
> 
> > Note
> > 
> > This option cannot be used with built-in modules and extension modules written in C, since they do not have Python module files. However, it can still be used for precompiled modules, even if the original source file is not available.
> 
> If this option is given, the first element of [`sys.argv`](https://docs.python.org/3/library/sys.html#sys.argv "sys.argv") will be the full path to the module file (while the module file is being located, the
> first element will be set to `"-m"`). As with the [`-c`](https://docs.python.org/3/using/cmdline.html#cmdoption-c) option, the current directory will be added to the start of [`sys.path`](https://docs.python.org/3/library/sys.html#sys.path "sys.path").
> 
> [`-I`](https://docs.python.org/3/using/cmdline.html#cmdoption-I) option can be used to run the script in isolated mode where [`sys.path`](https://docs.python.org/3/library/sys.html#sys.path "sys.path") contains neither the current directory nor the user’s site-packages directory. All `PYTHON*` environment variables are ignored, too.
> 
> Many standard library modules contain code that is invoked on their execution as a script. An example is the [`timeit`](https://docs.python.org/3/library/timeit.html#module-timeit "timeit: Measure the execution time of small code snippets.") module:
> 
> ```sh
> python -m timeit -s 'setup here' 'benchmarked code here'
> python -m timeit -h # for details
> ```
> 
> Raises an [auditing event](https://docs.python.org/3/library/sys.html#auditing) `cpython.run_module` with argument `module-name`.
> 
> > See also
> > 
> > [`runpy.run_module()`](https://docs.python.org/3/library/runpy.html#runpy.run_module "runpy.run_module")
> > 
> > Equivalent functionality directly available to Python code
> 
> [PEP 338](https://www.python.org/dev/peps/pep-0338) – Executing modules as scripts
> 
> Changed in version 3.1: Supply the package name to run a `__main__` submodule.
> 
> Changed in version 3.4: namespace packages are also supported
> 
> — [1. Command line and environment — Python 3.10.5 documentation](https://docs.python.org/3/using/cmdline.html#cmdoption-m)

在 `sys.path` 指定的目录中寻找 module 并以 `__main__` module 的身份执行指定 module。

注意不要在名字后面加 `.py`，因为我们已经把执行的文件当作 module 来看待。

如果指定的是一个 Package name（即目录名），将会执行 `<pkg>.__main__`（即 `<pkg>/__main__.py`）。 

另外，如果使用 `python -m a.b.module`，`sys.argv` 的首位将被设置为被执行 module 文件的**完整路径**（与之相对，`python a/b/module.py` 中 `sys.argv[0]` 将会是**相对当前工作目录的路径**，即 `a/b/module.py`）；同时，**当前工作目录**会被加入 `sys.path` 的首位。

---

`python -m A.B.module` 将顺次执行 `A`、`A.B` 的 `__init__.py`，即使该 module 没有任何导入行为。

`python -m` 对于直接执行 package 内部的代码是必要的。若直接以 script 方式运行，一旦涉及到任何高于该 script 所在目录（含该目录）的相对导入，Python 就会抛出如下错误：

```default
ImportError: attempted relative import with no known parent package
```

而一个 module 也不能导入超过 `python -m` 参数指定的最顶层结构的 module，否则会抛出错误：

```default
ImportError: attempted relative import beyond top-level package
```

## sys.modules

> The first place checked during import search is [`sys.modules`](https://docs.python.org/3/library/sys.html#sys.modules "sys.modules"). This mapping serves as a cache of all modules that have been previously imported, including the intermediate paths. So if `foo.bar.baz` was previously imported, [`sys.modules`](https://docs.python.org/3/library/sys.html#sys.modules "sys.modules") will contain entries for `foo`, `foo.bar`, and `foo.bar.baz`. Each key will have as its value the corresponding module object.
> 
> During import, the module name is looked up in [`sys.modules`](https://docs.python.org/3/library/sys.html#sys.modules "sys.modules") and if present, the associated value is the module satisfying the import, and the process completes. However, if the value is `None`, then a [`ModuleNotFoundError`](https://docs.python.org/3/library/exceptions.html#ModuleNotFoundError "ModuleNotFoundError") is raised. If the module name is missing, Python will continue searching for the module.
> 
> [`sys.modules`](https://docs.python.org/3/library/sys.html#sys.modules "sys.modules") is writable. Deleting a key may not destroy the associated module (as other modules may hold references to it),
> but it will invalidate the cache entry for the named module, causing Python to search anew for the named module upon its next
> import. The key can also be assigned to `None`, forcing the next import of the module to result in a [`ModuleNotFoundError`](https://docs.python.org/3/library/exceptions.html#ModuleNotFoundError "ModuleNotFoundError").
> 
> Beware though, as if you keep a reference to the module object, invalidate its cache entry in [`sys.modules`](https://docs.python.org/3/library/sys.html#sys.modules "sys.modules"), and then re-import the named module, the two module objects will *not* be the same. By contrast, [`importlib.reload()`](https://docs.python.org/3/library/importlib.html#importlib.reload "importlib.reload") will reuse the *same* module object, and simply reinitialise the module contents by rerunning the module’s code.
> 
> — [5. The import system — Python 3.10.5 documentation](https://docs.python.org/3/reference/import.html#the-module-cache)

`sys.modules` 是一个 `dict`，Python 在导入之前会去检查 `sys.module` 里是否已经存有需要的 module 的 module object。如果有，就直接用这个；如果值为 `None`（意思是以前找过但没找到），就直接报错；如果该键值对不存在，就继续搜索过程。总之，`sys.modules` 扮演了一个类似 cache 的角色。

对于形如 `A.B.C` 的导入，Python 会顺次导入 `A`、`A.B` 和 `A.B.C` 并把他们加入 `sys.modules`。

## 参考

- [5. The import system — Python 3.10.5 documentation](https://docs.python.org/3/reference/import.html)

- [6. Modules — Python 3.10.5 documentation](https://docs.python.org/3/tutorial/modules.html)

- [Python for the Lab | Complete Guide to Imports in Python: Absolute, Relative, and More](https://www.pythonforthelab.com/blog/complete-guide-to-imports-in-python-absolute-relative-and-more/)

- [Python 包内的导入问题（绝对导入和相对导入） - Anonymous596 - 博客园](https://www.cnblogs.com/gaowengang/p/8543840.html)
