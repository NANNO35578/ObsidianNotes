很好 👍 你想要的场景其实是 **Nginx 只做静态文件分发，把 HLS 文件（.m3u8 + .ts）放在服务器上供点播**。
这个流程 **不需要 RTMP 模块**，只要普通的 Nginx 就能实现。下面我一步步给你：

---

## 1. 安装 Nginx

不同平台安装方式：

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install nginx -y
```

### CentOS / RHEL

```bash
sudo yum install epel-release -y
sudo yum install nginx -y
```

### Windows

* 直接到 [nginx.org](https://nginx.org/en/download.html) 下载压缩包，解压后运行 `nginx.exe` 即可。

---

## 2. 准备视频转码为 HLS

Nginx **不会自己切片视频**，需要用 `ffmpeg` 提前处理。

假设你有一个 `video.mp4`，用 `ffmpeg` 转成 HLS：

```bash
ffmpeg -i video.mkv -codec: copy -start_number 0 -hls_time 10 -hls_list_size 0 -f hls video.m3u8
```

执行后会得到：

* `video.m3u8`（索引文件）
* `video0.ts`, `video1.ts`, …（分片）

这些文件放到 Nginx 能访问的目录，比如 `/var/www/html/hls/`。

---

## 3. 配置 Nginx

编辑配置文件（通常是 `/etc/nginx/sites-enabled/default` 或 `/etc/nginx/nginx.conf`）：

```nginx
server {
    listen 80;
    server_name localhost;

    location /hls/ {
        root /var/www/html;
        add_header Cache-Control no-cache;
        types {
            application/vnd.apple.mpegurl m3u8;
            video/mp2t ts;
        }
    }
}
```

⚠️ 注意：

* `root /var/www/html;` → 实际路径要改成你放 `.m3u8` 和 `.ts` 的目录。
* `/hls/` 表示访问地址类似 `http://your-ip/hls/video.m3u8`。

---

## 4. 测试

1. 重启 Nginx

   ```bash
   sudo systemctl restart nginx
   ```
2. 在浏览器 / 播放器打开

   ```
   http://your-ip/hls/video.m3u8
   ```
3. HTML5 播放器可以直接 `<video>` 播放：

   ```html
   <video controls autoplay>
     <source src="http://your-ip/hls/video.m3u8" type="application/x-mpegURL">
   </video>
   ```

---

✅ 这样就是最小化部署：

* Nginx 仅做 **静态文件服务器**。
* ffmpeg 负责把 MP4 转成 HLS 分片。
* 客户端用 `<video>` 或播放器加载 `.m3u8` 文件。

---

要不要我帮你写一个 **完整脚本**（自动把 mp4 转成 HLS 并拷贝到 Nginx 目录）？这样你只需要执行一次脚本，就能马上点播。
