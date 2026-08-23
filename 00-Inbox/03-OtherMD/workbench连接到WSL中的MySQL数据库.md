[参考1](https://askubuntu.com/questions/1249973/connect-mysql-workbench-to-wsl2) 

# 创建连接用户
在wsl中运行
```bash
mysql -u root -p
```

输入数据库密码
插入新用户`bench` 密码设置为`mysql0`
```sql
CREATE USER 'bench'@'%' IDENTIFIED BY 'mysql0';
GRANT ALL PRIVILEGES ON *.* TO 'bench'@'%' WITH GRANT OPTION;
FLUSH PRIVILEGES;
```

GRANT ALL PRIVILEGES ON *.* TO 'bench'@'%' IDENTIFIED BY 'mysql0';
FLUSH PRIVILEGES;

# 创建连接
Windows打开workbench 主页面`MySQL Connections` 后面有一个⊕ 点击创建新连接
`Hostname` 为wsl终端运行ifconfig 显示的ip
`Username` 为刚才创建的用户名 如此处可为`bench`
点击Store in Vault... 输入刚才设置的密码
下方测试连接

不出意外可以看到wsl中MySQL数据

![[mysqlConnection.png]]


若版本不一致会有警告 应该可忽略

不出意外连接很慢
