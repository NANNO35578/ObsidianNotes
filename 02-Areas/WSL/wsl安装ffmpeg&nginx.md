# `ffmpeg` 安装
[参考](https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu)

若软件源包含清华，不用开加速器
```bash
sudo apt-get install ffmpeg
```

安装完成输入`ffmpeg`出现如下输出就安装成功了

![[../../'attachments/ffmpegInstall.png]]


---
# `nginx` 安装
[参考](https://nginx.org/en/linux_packages.html#Debian)

在WSL上安装Nginx的步骤与在Debian上类似。以下是详细步骤：
### 1. 更新系统
首先，更新你的包列表和已安装的包：*如果卡了可以ctrl C 关掉不用管*
```bash
sudo apt update
sudo apt upgrade
```

### 2. 安装所需的依赖
安装必要的依赖包：
```bash
sudo apt install curl gnupg2 ca-certificates lsb-release debian-archive-keyring
```

### 3. 导入Nginx签名密钥
获取并导入Nginx的官方签名密钥：
```bash
curl https://nginx.org/keys/nginx_signing.key | gpg --dearmor \
    | sudo tee /usr/share/keyrings/nginx-archive-keyring.gpg >/dev/null
```

### 4. 验证密钥
确保下载的文件包含正确的密钥：
```bash
gpg --dry-run --quiet --no-keyring --import --import-options import-show /usr/share/keyrings/nginx-archive-keyring.gpg
```

检查输出中是否包含以下指纹：*应该有*
```
573BFD6B3D8FBC641079A6ABABF5BD827BD9BF62
```

### 5. 设置Nginx的APT源
如果你想安装稳定版Nginx，运行以下命令：*我选的这个*
```bash
echo "deb [signed-by=/usr/share/keyrings/nginx-archive-keyring.gpg] \
http://nginx.org/packages/debian `lsb_release -cs` nginx" \
    | sudo tee /etc/apt/sources.list.d/nginx.list
```

如果你想使用主线版本的Nginx，运行以下命令：
```bash
echo "deb [signed-by=/usr/share/keyrings/nginx-archive-keyring.gpg] \
http://nginx.org/packages/mainline/debian `lsb_release -cs` nginx" \
    | sudo tee /etc/apt/sources.list.d/nginx.list
```

### 6. 设置仓库优先级
设置仓库优先级，以优先使用Nginx的包：*仓库优先级是什么在下面*
```bash
echo -e "Package: *\nPin: origin nginx.org\nPin: release o=nginx\nPin-Priority: 900\n" \
    | sudo tee /etc/apt/preferences.d/99nginx
```

### 7. 安装Nginx
最后，更新包列表并安装Nginx：
```bash
sudo apt update
sudo apt install nginx
```

### 8. 启动Nginx
安装完成后，可以启动Nginx：
```bash
sudo service nginx start
```

### 9. 验证安装
在Windows主机浏览器中访问 wsl的ip我的是`172.24.105.254`，如果看到Nginx的欢迎页面*如下图*，说明安装成功。
![[../../'attachments/nginxInstall.png]]


sudo ln -sf /usr/local/nginx/sbin/nginx /usr/local/bin/nginx


---
## 设置仓库优先级是什么意思

设置仓库优先级是为了控制在系统中不同软件源（仓库）之间的包选择顺序。具体来说，它可以帮助确保在安装或升级软件时，优先使用特定来源的版本，而不是默认的系统源或其他第三方源。
### 主要目的：
1. **确保使用特定版本**：例如，使用Nginx的官方仓库而不是默认的Debian仓库，以获得最新的功能和修复。
2. **避免版本冲突**：如果多个源提供同一软件包，优先级设置可以防止安装不兼容的版本。
### 优先级设置的方式：
在Debian及其衍生系统中，优先级通过 `/etc/apt/preferences.d/` 目录中的文件进行配置。文件包含规则，指定特定包的优先级值，值越高，优先级越高。
### 示例：
- 在你的例子中，优先级被设置为900，这意味着系统会优先选择来自 `nginx.org` 的Nginx包，而不是其他来源

这种配置有助于保证你所安装的软件版本是你希望的版本，避免因软件源的不同导致的潜在问题



---

## `nginx`默认配置文件位置

```bash
sudo nvim /usr/local/nginx/conf/nginx.conf
sudo nvim /etc/nginx/nginx.conf
```

##### 设置配置文件路径
```bash
sudo nginx -c filename
```

---

# `rtmp` 模块`nginx`安装


1. **安装编译所需的依赖**
```bash
sudo apt install build-essential libpcre3 libpcre3-dev libssl-dev zlib1g zlib1g-dev
```

2. **下载Nginx和RTMP模块**：
```bash
wget http://nginx.org/download/nginx-1.28.0.tar.gz
tar -zxvf nginx-1.28.0.tar.gz
git clone https://github.com/arut/nginx-rtmp-module.git # 有点难下载
```

3. **编译Nginx**：
```bash
cd nginx-1.28.0
./configure --with-http_ssl_module --add-module=../nginx-rtmp-module
make
sudo make install
```

4. 添加软链接
```bash
sudo ln -sf /usr/local/nginx/sbin/nginx /usr/local/bin/nginx
```

**启动Nginx**：
```bash
sudo /usr/local/nginx/sbin/nginx
```
运行以下命令，检查Nginx是否支持RTMP模块：
```bash
/usr/local/nginx/sbin/nginx -V
```

---

# `rtmp` 推流测试

## 首先修改配置文件
- nginx 此时以`nobody:nogroup` 启动，无权限查看wsl中文件

1. 设置启动用户 我的运行用户为 `usr24:usr24` 有权限
```conf
# user nobody;
user usr24 usr24;
```

2. 设置直播点播路径
```conf
rtmp {
	server {
		listen 1935;
		application live{
			live on;
			record off;
		}
		application demand{
			play /home/usr24/video; # 此处不可用~
		}
	}
}
```

3. 重启`nginx`
```bash
sudo nginx -s reload
```


## 直播
- 使用`ffmpeg` 推流
```bash
ffmpeg -re -i ~/NetDiskProject/b1.mp4 -vcodec copy -acodec copy -f flv rtmp://localhost/live
```

- 在推流期间 主机Windows可使用`ffplay` 打开
```bash
.\ffplay.exe rtmp://172.24.105.254:1935/live
```

## 点播
- 主机直接打开
```bash
.\ffplay.exe rtmp://172.24.105.254:1935/demand/mc.mp4
```

*也可以使用VCL、PotPlayer* 直接打开URL

---


[其他可能出现的问题](https://poe.com/s/CMpWqQ1fuIyCAkywlSLQ)

刚安装完`nginx`，修改了配置文件，添加了`rtmp`，Nginx启动失败的原因是配置文件中的 `rtmp` 指令未被识别。这意味着你当前的Nginx版本没有编译RTMP模块。

### 解决步骤
1. **确认Nginx版本和模块支持**
    运行以下命令确认当前Nginx版本及其编译的模块：
    ```bash
nginx -V
    ```
    检查输出中是否包含 `--add-module=...` 或类似的RTMP模块。如果没有，说明当前Nginx版本不支持RTMP。
2. **安装带RTMP模块的Nginx**
    如果当前版本不支持RTMP，你需要安装一个支持RTMP的版本。以下是安装步骤：
    #### 使用PPA安装带RTMP模块的Nginx
```bash
sudo add-apt-repository ppa:sergey-dryabov/nginx-rtmp
sudo apt update
sudo apt install nginx
```
- `add-apt-repository` 命令未找到，这通常是因为未安装 `software-properties-common` 包
```
sudo apt install software-properties-common
```

- `add-apt-repository` 命令无法访问Launchpad*手动添加所需的Nginx RTMP* [[#`rtmp` 模块`nginx`安装]]

3. **重新启动Nginx**
    安装完成后，尝试重新启动Nginx：
    ```bash
    sudo systemctl restart nginx
    ```
    
4. **检查是否成功启动**
    再次检查Nginx的状态：
    ```bash
sudo systemctl status nginx.service
    ```
 


