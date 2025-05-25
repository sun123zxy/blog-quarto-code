---
title: "DockerCompose+VLESS+WS+TLS+Web 方式搭建 V2Ray 代理"
abstract: "无懈可击的超强配置搭建教程。"
date: 2022/02/04
date-modified: 2022/06/01
categories:
  - technical
  - web

format:
  html: default
---

全文参考：

+ [(VMESS) + WebSocket + TLS + Web | 新 V2Ray 白话文指南](https://guide.v2fly.org/advanced/wss_and_web.html)
+ [Docker 部署 V2Ray | 新 V2Ray 白话文指南](https://guide.v2fly.org/app/docker-deploy-v2ray.html)
+ [V2Ray(V2Fly) 官方文档](https://www.v2fly.org/)

## 原理

VLESS 作为内部传输协议，使用 HTTP 的扩展 WebSocket 作为传输载体，外层使用 TLS 加密传输。服务器端用 Nginx 搭建正常 HTTPS 网站，收到向特定路径发送的 WebSocket 流量时充当反向代理转发至 Docker 容器内的 V2Ray 处理。

从外部看，服务器是货真价实的 HTTPS 服务器，客户端发出的请求也是货真价实的 HTTPS 流量；直接用浏览器访问入口路径将返回 `400 Bad Request`（由 V2Ray 返回）或 `404 Not Found`（可在 Nginx 中预检测 WebSocket 请求，增强隐蔽性）；安全性、抗干扰能力则完全由 TLS 保障，几乎无懈可击。

更强的技术还有 Xray 中使用的 XTLS，无缝拼接了内外两层 TLS 使得性能进一步提高。不过因为和 V2Ray 主社区分离了，还是决定先用 VLESS+WS+TLS+Web 配置。

+ [V2Ray V2Fly Xray 的历史](https://tlanyan.pp.ua/xray-tutorial/)

结构上，使用 Docker 和 Docker Compose 容器化安装方便管理；而装在外层的 Nginx 可以作为所有网页服务的入口点，方便以后增加其它服务。

## 流程

以下所有操作在 Ubuntu 20.04 的 root 用户下进行。代码中部分需要自己填写的敏感信息会用如 `{[VARIABLE]}` 的记号标明，请自行替换。

### 准备工作

#### 安装 cURL

```bash
apt install curl
```

#### 安装 Docker

```bash
curl -fsSL https://get.docker.com/ | sh # 获取脚本并交给 sh (shell) 执行
systemctl start docker
```

#### 安装 Docker Compose

```bash
curl -L https://github.com/docker/compose/releases/download/1.25.0/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose
# docker-compose --version
```

#### 安装并运行 Nginx

```bash
apt install nginx
systemctl start nginx
```

此时通过浏览器访问服务器就能看到 Nginx 默认的欢迎界面了。

#### 调试技巧

Nginx 的日志存储在 `/var/log/nginx` 目录下；V2Ray 的日志在 Docker 容器里，之后我们将把它映射到宿主机上方便操作和存储。

一些常用的调试命令：

```bash
systemctl start nginx
systemctl status nginx
systemctl stop nginx

docker-compose up -d # Create and start containers（-d 表示后台运行）
docker-compose down  # Stop and remove containers, networks, images, and volumes
docker-compose start # Start services
docker-compose stop  # Stop services

docker ps # 查看各容器运行状态
docker log {[CONTAINER_ID]} # 查看某容器运行日志
```

### 安装 Certbot 并以 Webroot 方式获取 Let's Encrypt 证书

+ Certbot 官方教程：[User Guide — Certbot 1.22.0 documentation](https://eff-certbot.readthedocs.io/en/stable/using.html)
+ Certbot 原理：[HTTPS-使用Certbot自动配置Let’s Encrypt证书 - 简书](https://www.jianshu.com/p/3ffd27b64847)

Let's Encrypt 是一个免费、自动化和开放的证书颁发机构，为网站提供免费的 SSL/TLS 证书。要从 Let's Encrypt 获取某个域名的证书，需要证明拥有对该域名的控制权，而 Certbot 就是官方提供的自动化认证工具。

Webroot 是 Certbot 提供的一种认证方式，如果服务器上有网站运行且有能力修改其配置，就可以用该方式进行认证。使用这种方式获取证书时无需暂停网页服务端的运行。

> The webroot plugin works by creating a temporary file for each of your requested domains in `${webroot-path}/.well-known/acme-challenge`. Then the Let’s Encrypt validation server makes HTTP requests to validate that the DNS for each requested domain resolves to the server running certbot.

首先安装 Certbot：

```bash
apt install certbot python3-certbot
```

因为之前安装的 Nginx 已经在 `/var/www/html` 下生成默认的欢迎页网站，故可直接利用该目录进行 Webroot 认证：

```bash
certbot certonly --webroot -w /var/www/html -d {[YOUR_DOMAIN]}
```

认证过程中 Certbot 的回显信息：

```default
Saving debug log to /var/log/letsencrypt/letsencrypt.log
Plugins selected: Authenticator webroot, Installer None
Obtaining a new certificate
Performing the following challenges:
http-01 challenge for {[YOUR_DOMAIN]}
Using the webroot path /var/www/html for all unmatched domains.
Waiting for verification...
Cleaning up challenges

IMPORTANT NOTES:
 - Congratulations! Your certificate and chain have been saved at:
   /etc/letsencrypt/live/{[YOUR_DOMAIN]}/fullchain.pem
   Your key file has been saved at:
   /etc/letsencrypt/live/{[YOUR_DOMAIN]}/privkey.pem
   Your cert will expire on {[EXPIRE-DATE]}. To obtain a new or tweaked
   version of this certificate in the future, simply run certbot
   again. To non-interactively renew *all* of your certificates, run
   "certbot renew"
 - If you like Certbot, please consider supporting our work by:

   Donating to ISRG / Let's Encrypt:   https://letsencrypt.org/donate
   Donating to EFF:                    https://eff.org/donate-le
```

### 使用 Docker Compose 安装并配置 V2Ray

上传 `docker-compose.json` 至适当位置（建议妥善储存该文件，以便后续管理使用）：

```yml
version: "3.4"
services:
  v2ray:
    image: v2fly/v2fly-core
    container_name: v2ray
    restart: always # 自动重启容器
    ports: 
      - {[YOUR_PORT_OUTER]}:{[YOUR_PORT_INNER]} # 端口映射，注意外左内右
    command: v2ray -config=/etc/v2ray/config.json # 需要指定配置文件位置
    volumes:
      - /etc/v2ray:/etc/v2ray # 左边宿主目录，右边容器目录
      - /var/log/v2ray:/var/log/v2ray
```

利用 volume 技术，容器中 V2Ray 的配置文件被映射到宿主机的 `/etc/v2ray` 目录下，而日志信息被映射到 `/var/log/v2ray` 目录下。

故运行容器之前，我们在 `/etc/v2ray` 下放置 `config.json` 作为容器内 V2Ray 的配置文件：

```json
{
    "log": {
        "loglevel": "warning",
        "access": "/var/log/v2ray/access.log",
        "error": "/var/log/v2ray/error.log"
    },
    "inbounds": [
        {
            "port": "{[YOUR_PORT_INNER]}", // 容器内的监听端口
            "listen":"0.0.0.0", // Caution!
            "protocol": "vless",
            "settings": {
                "decryption": "none",
                "clients": [
                    {
                        "id": "{[YOUR_UUID]}",
                        "email": "{[YOUR_EMAIL]}" // 作标识用，可以随便填
                    }
                ]
            },
            "streamSettings": {
                "network": "ws",
                "wsSettings": {
                    "path": "{[YOUR_PATH]}" // 希望设定的 V2Ray 入口路径，如 "/ray"
                }
            }
        }
    ],
    "outbounds": [
        {
            "protocol": "freedom",
            "settings": {}
        }
    ]
}
```

> 有一个小坑点（见 [Issue #2221](https://github.com/v2ray/v2ray-core/issues/2221)）：与白话文指南不同，如果用 Docker 搭建 V2Ray，容器外的 Nginx 需要向容器内的 V2Ray 发送数据，因此容器内的 V2Ray 必须监听本机 IP `0.0.0.0` 而不是本地回环 IP `127.0.0.1`。
> 
> 常见的症状是客户端报 `502 Bad Gateway > websocket: bad handshake` ，Nginx 报 `upstream prematurely closed connection`，而容器内 V2Ray 没有报警日志。

最后，在之前放置 `docker-compose.yml` 的目录下执行：

```bash
docker-compose up -d
```

### 设置 Nginx 反向代理

+ [Nginx - Configuring HTTPS servers](http://nginx.org/en/docs/http/configuring_https_servers.html)
+ [Nginx - Module ngx_http_ssl_module](http://nginx.org/en/docs/http/ngx_http_ssl_module.html)

通常修改 Nginx 配置可以通过直接修改 `/etc/nginx/nginx.conf` 或在 `/etc/nginx/conf.d/` 目录下新建配置文件的方式进行，但根据推荐的目录结构（参见 [Nginx/DirectoryStructure - Debian Wiki](https://wiki.debian.org/Nginx/DirectoryStructure)），这里选择在 `/etc/nginx/sites-available/` 放置配置文件后在 `/etc/nginx/sites-enabled/my-enabled.conf` 中动态引用。

故首先在 `/etc/nginx/sites-available/` 下新建 `{[YOUR_DOMAIN]}.conf`（文件名可自行调整）：

```nginx
server {
  listen 443 ssl;
  listen [::]:443 ssl;

  ssl_certificate       /etc/letsencrypt/live/{[YOUR_DOMAIN]}/fullchain.pem;
  ssl_certificate_key   /etc/letsencrypt/live/{[YOUR_DOMAIN]}/privkey.pem;
  # 利用缓存重用 session 提高性能
  ssl_session_timeout 1d;
  ssl_session_cache shared:MozSSL:10m;
  ssl_session_tickets off;

  ssl_protocols         TLSv1.2 TLSv1.3;
  # 设置加密方式，默认的已经不安全了
  ssl_ciphers           ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES256-GCM-SHA384;
  ssl_prefer_server_ciphers off; # (?) Specifies that server ciphers should be preferred over client ciphers when using the SSLv3 and TLS protocols.

  server_name           {[YOUR_DOMAIN]};
  location {[YOUR_PATH]} { # 与 V2Ray 配置中的 path 保持一致
    if ($http_upgrade != "websocket") { # WebSocket 协商失败时返回 404
        return 404;
    }
    proxy_redirect off;
    proxy_pass http://127.0.0.1:{[YOUR_PORT_OUTER]}; # 设置反向代理转发至 V2Ray
    proxy_http_version 1.1;
    # 一些 WebSocket 需要的配置
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection "upgrade";
    proxy_set_header Host $host;
    # Show real IP in v2ray access.log
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
  }
}
# modified from (2022/01/30) https://guide.v2fly.org/advanced/wss_and_web.html#nginx-%E9%85%8D%E7%BD%AE
```

然后在 `/etc/nginx/sites-enabled/` 目录下，保持原有的 `default` 文件，并利用

```bash
ln -s /etc/nginx/sites-available/{[YOUR_DOMAIN]}.conf
```

创建软链接至刚才保存的配置文件．最后重新加载配置文件：

```bash
nginx -s reload
```

### 客户端

~~用的图形化界面还没研究配置文件~~，不过 outbounds 部分大致应如下所示：

```json
"outbounds": [
    {
        "protocol": "vless",
        "settings": {
            "vnext": [
                {
                    "address": "{[YOUR_DOMAIN]}",
                    "port": 443,
                    "users": [
                        {"id": "{[YOUR_UUID]}", "email": "{[YOUR_EMAIL]}"}
                    ]
                }
            ]
        },
        "streamSettings": {
            "network": "ws",
            "security": "tls",
            "wsSettings": {
                "path": "{[YOUR_PATH]}"
            }
        }
    }
]
```
