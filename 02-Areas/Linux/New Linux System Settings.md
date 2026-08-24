# 1.添加`sudo`权限

```bash
su
/etc/sbin/usermode -aG haha
```
----

# 2.改软件源
```bash
# 中科大
deb https://mirrors.ustc.edu.cn/debian/ trixie main contrib non-free non-free-firmware
deb-src https://mirrors.ustc.edu.cn/debian/ trixie main contrib non-free non-free-firmware
deb https://mirrors.ustc.edu.cn/debian-security/ trixie-security main contrib non-free non-free-firmware
deb-src https://mirrors.ustc.edu.cn/debian-security/ trixie-security main contrib non-free non-free-firmware
deb https://mirrors.ustc.edu.cn/debian/ trixie-updates main contrib non-free non-free-firmware
deb-src https://mirrors.ustc.edu.cn/debian/ trixie-updates main contrib non-free non-free-firmware

# 阿里云
deb https://mirrors.aliyun.com/debian/ trixie main contrib non-free non-free-firmware
deb-src https://mirrors.aliyun.com/debian/ trixie main contrib non-free non-free-firmware
deb https://mirrors.aliyun.com/debian-security/ trixie-security main contrib non-free non-free-firmware
deb-src https://mirrors.aliyun.com/debian-security/ trixie-security main contrib non-free non-free-firmware
deb https://mirrors.aliyun.com/debian/ trixie-updates main contrib non-free non-free-firmware
deb-src https://mirrors.aliyun.com/debian/ trixie-updates main contrib non-free non-free-firmware

# 清华
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ trixie main contrib non-free non-free-firmware
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ trixie main contrib non-free non-free-firmware
deb https://mirrors.tuna.tsinghua.edu.cn/debian-security/ trixie-security main contrib non-free non-free-firmware
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian-security/ trixie-security main contrib non-free non-free-firmware
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ trixie-updates main contrib non-free non-free-firmware
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ trixie-updates main contrib non-free non-free-firmware

# 官方保底
deb http://deb.debian.org/debian/ trixie main contrib non-free non-free-firmware
deb-src http://deb.debian.org/debian/ trixie main contrib non-free non-free-firmware
deb http://security.debian.org/debian-security trixie-security main contrib non-free non-free-firmware
deb-src http://security.debian.org/debian-security trixie-security main contrib non-free non-free-firmware
deb http://deb.debian.org/debian/ trixie-updates main contrib non-free non-free-firmware
deb-src http://deb.debian.org/debian/ trixie-updates main contrib non-free non-free-firmware
```



---
# 3.一些软件
```
sudo apt install git
sudo apt install tmux
sudo apt install neovim
sudo apt install btop
```


---

# 4.设置英文
1. 重新配置 locale
    `sudo dpkg-reconfigure locales`
    - 在列表里 **取消** `zh_CN.UTF-8`（按空格取消 * 号），只保留  
        `en_US.UTF-8` 和 `C.UTF-8`（建议保留后者做 fallback）。
    - 下一步问“默认系统 locale”时选 `en_US.UTF-8`。
2. 立即生效
```
    sudo localectl set-locale LANG=en_US.UTF-8   # systemd 系统
    # 老版 sysvinit 可忽略
```
3. 注销或重启
    `sudo reboot`
    重新登录后 `locale` 应只有 `LANG=en_US.UTF-8`。


