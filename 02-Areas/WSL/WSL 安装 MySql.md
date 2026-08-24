
参考 [微软:如何在WSL安装MySql](https://learn.microsoft.com/en-us/windows/wsl/tutorials/wsl-database#install-mysql)

1. 安装`wget` 
2. 安装`lsb-release
```bash
sudo apt install wget
sudo apt install lsb-release
````
3. 用Windows从[Oracle](https://dev.mysql.com/downloads/repo/apt/)下载后缀为`.deb`的文件 然后利用Windows资源管理器移动到wsl某个目录下 或在wsl中找个文件夹运行以下命令
```bash
 wget https://dev.mysql.com/get/mysql-apt-config_0.8.34-1_all.deb
```
4. 在wsl该目录下中运行
```bash
sudo dpkg -i mysql-apt-config_0.8.34-1_all.deb
```
5. 更新包索引 **必须的**
```bash
sudo apt update
```
6. 安装MySql
```bash
sudo apt install mysql-server
```

- 然后会弹出选择什么东西 用方向键选ok就行
- 然后应该是设置密码 然后确认密码 我设置的是 mysql0 最好只使用小写
- 启动MySql服务
```bash
sudo service mysql start
```

7. 要使用`<mysql/mysql.h>`头文件，需要安装 MySQL 开发包
```bash
sudo apt install libmysqlclient-dev
```