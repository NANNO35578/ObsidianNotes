# 实现的功能
##### 基本功能
- [x]  登录系统:注册,登录
- [x]  获取文件列表
- [x]  下载文件,下载文件夹
- [x]  上传文件,上传文件夹(优先实现)
- [x]  新建文件夹
- [x]  进入,退出文件夹
- [x]  获取本人所有分享的文件信息
- [x]  通过分享码获取某人分享
- [x]  秒传(上传)
- [x]  断点续传-上传,下载

##### 高级功能
- [ ]  图片类,可以分类 -- 使用opencv识别图像
- [x]  视频类,可以在线观看
- [ ]  文件类,可以在线观看(最简单 在线查看txt)
- [ ]  同步
- [ ]  多地登录x

##### 优化
- 大文件传输



---
# 数据库设计

设计数据库 `mysql` `NetDisk`
注册,登录
##  用户信息表
```sql
DROP TABLE IF EXISTS t_user ;
CREATE TABLE t_user (
    u_id bigint(11) NOT NULL AUTO_INCREMENT, 
    u_tel varchar(20) NOT NULL ,
    u_password varchar(45) NOT NULL, 
    u_name varchar(45) NOT NULL, 
    PRIMARY KEY ( u_id )
)ENGINE=InnoDB CHARSET=utf8;
```

##  文件表 文件详细信息
```sql
#文件信息表 t_file (用户 id , 大小 , 路径 , 引用计数 , MD5 , 上传完状态 , 文件类型 )  
DROP TABLE IF EXISTS t_file ;  
CREATE TABLE t_file (  
    f_id bigint(11) NOT NULL AUTO_INCREMENT,   
    f_size bigint(11) NOT NULL,   
    f_path varchar(260) NOT NULL,  # 服务端文件路径 包含文件名
    f_count bigint(11) NOT NULL,   
    f_MD5 varchar(45) NOT NULL,   
    f_state int NOT NULL ,   # 上传是否完成
    f_type varchar(10) NOT NULL,  
    PRIMARY KEY (f_id)  
) ENGINE=InnoDB CHARSET=utf8;
```

*f_count* : 秒传 分享文件时加一 上传及完成时为1 为0 应当删除
##  用户文件关系表
```sql
#用户文件关系表 t_user_file ( 用户 id , 文件 id , 目录 , 文件名字, 上传时间 , 分享码, 分享密码 , 分享时间 )
DROP TABLE IF EXISTS t_user_file ;
CREATE TABLE t_user_file ( 
    u_id bigint(11) NOT NULL, 
    f_id bigint(11) NOT NULL, 
    f_dir varchar(260) NOT NULL,  # 服务端文件目录 不包含文件名
    f_name varchar(260) NOT NULL,
    f_uploadtime datetime NOT NULL,
    s_link bigint(11),
    s_linkPassword bigint(4) ,
    s_linkTime datetime # 分享的时间
) ENGINE=InnoDB CHARSET=utf8;
```

### 创建视图 `user_file_info`

```sql
#新建视图 user_file_info  
create view user_file_info as (  
    select f_size , f_MD5 , f_type , f_path , f_count , f_state , t_user_file.*   # t_user_file 所有内容
    from t_file inner join t_user_file on t_file.f_id = t_user_file.f_id   
);
```

### 触发器
- 添加用户文件关系 文件引用计数 +1
```sql
drop trigger if exists refPlusOne;  
delimiter //   
	create trigger refPlusOne  after insert   
	on t_user_file   for each row   
	begin   
	    update t_file set f_count = f_count + 1 where t_file.f_id = new.f_id;   
	end //   
delimiter ;  
```

- 删除用户文件关系 文件引用计数 -1
```sql
drop trigger if exists refMinusOne;  
delimiter //   
	create trigger refMinusOne  after delete   
	on t_user_file   for each row   
	begin   
		update t_file set f_count = f_count - 1 where t_file.f_id = old.f_id;   
		    delete from t_file where f_count = 0 and t_file.f_id = old.f_id;   
	end //   
delimiter ;
```

### 断点续传的表
- 断点续传功能实现存储在用户本地数据库
- 文件存储类型数据库 `sqlite` u_id.db
- 记录正在上传和下载的任务 `t_upload` `t_download`

---
# 注册登录
##### 电话号合法性检验
> [!abstract] 正则表达式 `QRegExp exp("^1[3456789][0-9]\{9\}$");`
> `exp.exactMatch(tel);`

##### 密码传输
> [!abstract] `MD5` 信息摘要5 
> 明文加密为密文
> 对信息一致性，完整性验证

*加盐* : 原始数据中加入特定信息

![[注册登录.png]]

---
# 上传文件

| 上传文件请求<br>(文件头)                                                                       | 上传文件回复<br>文件头回复                                               | 文件内容请求                                                     | 文件内容回复                                                        |
| ------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------- |
| 0) timestamp<br>1) userid<br>2) 文件名<br>3) 大小<br>4) 路径<br>5) md5<br>6) 上传时间<br>7) 文件类型 | 0) timestamp<br>1) userid<br>2) fileid<br>3) result<br>4) md5 | 0) timestamp<br>1) userid<br>2) fileid<br>3) 文件内容<br>4) 长度 | 0) timestamp<br>1) userid<br>2) fileid<br>3) result<br>4) len |

![[上传文件.png]]

---
# 获取文件列表

> [!abstract] 柔性数组
> 柔性数组是一种特殊的数组形式，通常用于结构体中
> 允许在结构体的最后定义一个不定大小的数组，从而使得结构体的*大小可以在运行时动态确定*
> 特性多用于处理不定长度数据 ~~必须放在结构体最后~~
> ~~可用std::vector~~

```c
struct MyStruct {
    int count;
    int data[]; 
    // 必须定义为 类型 变量名[];  此时不占空间 结构体大小为4 
};
```

| 获取文件列表请求               | 文件信息列表                      | *文件信息*                                              |
| ---------------------- | --------------------------- | --------------------------------------------------- |
| 1) userid<br>2) 要获取的路径 | 1) 目录<br>2) 个数<br>3) *文件信息* | 1) fileid<br>2) 文件名字<br>3) 上传时间<br>4) 大小<br>5) 文件类型 |

![[获取当前文件夹文件列表.png]]

## 字节单位换算
```cpp
  static QString trans(int nByte) {
    const int KB = 1024;
    const int MB = KB * 1024;
    const int GB = MB * 1024;

    if (nByte < KB) {
      return QString::number(nByte) + "B";
    } else if (nByte < MB) {
      return QString::number(nByte / (float)KB, 'f', 1) + "KB";
    } else if (nByte < GB) {
      return QString::number(nByte / (float)MB, 'f', 1) + "M";
    } else {
      return QString::number(nByte / (float)GB, 'f', 1) + "G";
    }
  }
```

---
# 下载文件
- 下载完成，加入到完成表，通过`QPushButton::setToolTip` 传输文件本地路径
- 通过 `QProcess::startDetached` 执行系统命令 打开文件
```bash
 explorer /select, "E:\\1.txt"
```

| 下载文件请求                                           | 下载文件回复                                              | 文件头请求                                                                        | 文件头回复                                                   | 文件内容请求                                                      | 文件内容回复                                                        |
| ------------------------------------------------ | --------------------------------------------------- | ---------------------------------------------------------------------------- | ------------------------------------------------------- | ----------------------------------------------------------- | ------------------------------------------------------------- |
| 0) timestamp<br>1) userid<br>2) f_id<br>3) f_dir | 0) timestamp<br>1) userid<br>2) fileid<br>2) result | 0) timestamp<br>1) fileid<br>2) 文件名<br>3) size<br>4) 路径<br>5) md5<br>6) 文件类型 | 0) timestamp<br>1) userid<br>2) fileid<br>3) result<br> | 0) timestamp<br>1) userid<br>2) fileid<br>3) 文件内容<br>4) len | 0) timestamp<br>1) userid<br>2) fileid<br>3) result<br>4) len |

![[下载文件.png]]

##### 只打开文件夹代码
```cpp
QProcess process;
process.startDetached("explorer", QStringList() << QString("/select,") << 
QString("%1").arg(path));
```
- path 为要打开的路径
- *注意*: 此处路径必须是'\\'拼接, 比如 "E:\\1.txt" 而不能是 "E:/1.txt", qt 环境可以使用 replace 进行转换

##### `Explorer [/n][/e][[,/root],[path]][[,/select],[path filename]]`
- 参数说明
- `/n` 表示以“我的电脑”方式打开一个新的窗口，通常打开的是 Windows 安装分区的根目录
- `/e` 表示以“资源管理器”方式打开一个新的窗口，通常打开的也是 Windows 安装分区的根目录
- `/root [path]`表示打开指定的文件夹，/root 表示只显示指定文件夹下面的文件（夹），不显示其它磁盘分区和文件夹；`[path]`表示指定的路径
- `/select [path filename]`表示打开指定的文件夹并且选中指定的文件，`[path filename]`表示指定的路径和文件名

---
# 新建文件夹&上传文件夹
##### 新建文件夹

| 新建文件夹请求                                          | 新建文件夹回复                               |
| ------------------------------------------------ | ------------------------------------- |
| 0) 时间戳<br>1) 用户id<br>2) 文件夹名<br>3) 路径<br>4) 上传时间 | 0) 时间戳<br>1) 用户id<br>2) 文件id<br>3) 结果 |

![[新建文件夹.png]]

##### 上传文件夹

> [!abstract] 上传服务器显示打开文件失败原因 解决
> 由于上传文件时运行速度过快 服务端收到创建文件夹请求后
> *文件夹还没创建*就收到了在该文件夹下创建文件的请求 导致文件打开失败 从而文件上传失败
>  使用`QTimer`每200ms才进行一个文件上传任务, 确保服务器创建了文件夹
```cpp
QTimer::singleShot(200, this, [subDir, fileName, this]( ) { slot_uploadFile(subDir, fileName); });
```

同理创建子目录可能出现父目录还未创建，可采用相同方法解决


![[上传文件夹.png]]


---
# 分享文件&获取分享

##### 分享文件
- 分享文件请求同样为柔性数组
- 分享链接(int)生成
```cpp
    link = 1 + random() % 8;
    link *= (int)1e8;
    link += random() % (int)1e8;
```

![[分享文件创建分享链接.png]]

##### 获取分享

![[依据分享码获取文件.png]]

## 秒传
- 通过文件MD5已经文件`f_state` 区分是否可以秒传
- 查询数据库 若正在上传文件md5存在且`f_state` 为1 表示有相同文件已上传到服务端
- 直接更新数据库 插入用户信息

## 文件夹跳转
##### 双击打开子文件夹
- 获取当前双击文件信息，若不为"file" 创建子目录字符串
- 设置当前路径为子目录
- 删除当前文件列表
- 获取新列表

##### 按钮按下返回上一级
- 若当前路径为 '/' 不可再跳转
- 不为'/' : 拆分出当前文件夹倒数第二个'/' 左边部分为新目录
- 设置当前路径为新目录
- 删除当前文件列表
- 获取新列表

---
# 下载文件夹


![[文件夹下载.png]]


---
# 删除文件&文件夹

- 文件引用计数为0 可直接删除服务端文件
- 文件夹不可直接删除服务端文件，因为服务端文件夹下可能有引用计数不为0的文件

![[删除文件&文件夹.png]]


---
# 断点续传
##### 上传/下载任务何时暂停
1. 用户自行点击暂停/继续 (客户端开启)
2. 用户因意外程序退出 (客户端关闭)

##### 任务暂停与恢复 对于情景 1
- 任务信息仍然保存在用户客户端内存
- 在`FileInfo`结构体中有成员`isPause` 将其设置可进行客户端开启时的任务暂停与继续
- 具体为在设置相应标志后，在文件块回复中进行挂起

```cpp
    // pause?
    while (info.isPause) {
      QThread::msleep(100);
      // avoid block window thread Add this: get signal an excuse
      QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

      if (m_isQuit)
        return;
    }
```

- 由于客户端单线程，若某些操作进入循环不能尽快跳出，同样可加入如下代码防止主窗口操作无响应
- `QCoreApplication::processEvents(QEventLoop::AllEvents, 100);`
- 例如对于大文件获取MD5就将进行很多次循环

## 断点续传
- 对应情景 2 
- 任务信息丢失

- 考虑客户端本地持久化 存储任务信息 
- 使用`sqlite` 将任务存储为 `u_id.db` 分为上传表/下载表
- 用户登录时 加载未完成的任务
- 用户上传/下载文件时，收到服务端回复/请求时，即可写入客户端本地数据库，然后将任务信息插入主窗体
- 此时客户端本地数据库已有相应任务信息存储(并不存储已处理多大数据)

**具体流程**: 
1. 当客户端再次开启登录后，即可读取数据库信息，加载任务
2. 用户点击继续后
	- 若为下载任务，客户端读取本地已下载文件大小，向服务端发送对应 继续下载文件请求，服务器收到后打开文件，继续写
	- 若为上传任务，服务端收到客户端继续上传请求后，读取服务端文件大小，返回数据包，客户端根据返回包从相应位置继续上传
3. 任务完成后从数据库删除

![[断点续传.png]]

##### 上传表
```sql
CREATE TABLE t_upload (
	timestamp int,
	f_id int,
	f_name varchar(260),
	f_dir varchar(260),
	f_time varchar(260),
	f_size int,
	f_md5 varchar(60),
	f_type varchar(60),
	f_absolutePath varchar(260)
);
```

##### 下载表
```sql
create table t_download (
	timestamp int,
	f_id int,
	f_name varchar(260),
	f_dir varchar(260),
	f_time varchar(260),
	f_size int,
	f_md5 varchar(60),
	f_type varchar(60),
	f_absolutePath varchar(260)
);
```

---
# 文件预览支持
## 视频预览

- 服务器搭建[`nginx`](https://zh.wikipedia.org/wiki/Nginx) 部署[`HLS(HTTP Live Streaming)`](https://zh.wikipedia.org/wiki/HTTP_Live_Streaming) 
- 为了支持HLS，通常需要Nginx [RTMP](https://zh.wikipedia.org/wiki/RTMP)模块

<div style="display: flex; align-items: center;">
    <img src="https://github.com/user-attachments/assets/9335b488-ffcc-4157-8364-2370a0b70ad0" alt="nginx logo" style="width: 200px; margin-left: 10px;">
	<div style="flex: 1;">
        <p><strong>Nginx</strong></p>
        <p>异步框架的<a href="https://zh.wikipedia.org/wiki/%E7%B6%B2%E9%A0%81%E4%BC%BA%E6%9C%8D%E5%99%A8" title="网页服务器">网页服务器</a>，也可以用作<a href="https://zh.wikipedia.org/wiki/%E5%8F%8D%E5%90%91%E4%BB%A3%E7%90%86" title="反向代理">反向代理</a>、<a href="https://zh.wikipedia.org/wiki/%E8%B4%9F%E8%BD%BD%E5%9D%87%E8%A1%A1" title="负载均衡">负载平衡器</a>和<a href="https://zh.wikipedia.org/wiki/HTTP%E7%BC%93%E5%AD%98" title="HTTP缓存">HTTP缓存</a>。</p>
        <p>Nginx使用异步事件驱动的方法来处理请求。Nginx的模块化事件驱动架构<a href="https://zh.wikipedia.org/wiki/Nginx#cite_note-aosabook-15">[15]</a>可以在高负载下提供更可预测的性能<a href="https://zh.wikipedia.org/wiki/Nginx#cite_note-Configuration-16">[16]</a>。</p>
    </div>
</div>

> [!abstract] 特点
> - Nginx是一款面向性能设计的HTTP服务器，相较于[Apache](https://zh.wikipedia.org/wiki/Apache_HTTP_Server "Apache HTTP Server")、[lighttpd](https://zh.wikipedia.org/wiki/Lighttpd "Lighttpd")具有占有[内存](https://zh.wikipedia.org/wiki/%E9%9A%8F%E6%9C%BA%E5%AD%98%E5%82%A8%E5%99%A8 "随机存储器")少，稳定性高等优势。与旧版本（≤2.2）的Apache不同，Nginx不采用每客户机一线程的设计模型，而是充分使用异步逻辑从而削减了上下文调度开销，所以并发服务能力更强。整体采用模块化设计，有丰富的模块库和第三方模块库，配置灵活。在Linux操作系统下，Nginx使用*epoll*事件模型，得益于此，Nginx在Linux操作系统下效率相当高。同时Nginx在OpenBSD或FreeBSD操作系统上采用类似于epoll的高效事件模型kqueue
> 
> - 整体采用[模块化设计](https://zh.wikipedia.org/wiki/%E6%A8%A1%E5%9D%97%E5%8C%96%E8%AE%BE%E8%AE%A1 "模块化设计")是Nginx的一个重大特点，甚至http服务器核心功能也是一个模块。旧版本的Nginx的模块是静态的，添加和删除模块都要对Nginx进行重新编译，1.9.11以及更新的版本已经支持动态模块加载





---
### 如何预览
- 部署完成后，待预览的视频可复制到`HLS`文件夹下，客户端只需通过类似`http://172.24.105.254/hls/ror2.mp4`的链接即可访问视频
- 同时需要支持视频播放的播放器，客户端通过[`FFmpeg`](https://zh.wikipedia.org/wiki/FFmpeg)以及[`SDL`](https://zh.wikipedia.org/wiki/SDL)搭建播放器，通过服务器回复的链接即可预览

### `HLS`部署
- HLS（HTTP Live Streaming）是一种基于HTTP的流媒体传输协议，广泛用于视频直播和点播。使用Nginx进行HLS流媒体服务可以通过配置Nginx和一些相关模块来实现. 
- 工作原理是把整个流分成一个个小的基于HTTP的文件来下载，每次只下载一些
- 当媒体流正在播放时，客户端可以选择从许多不同的备用源中以不同的速率下载同样的资源，允许流媒体会话适应不同的数据速率
- 在开始一个流媒体会话时，客户端会下载一个包含元数据的[扩展 M3U (m3u8)](https://zh.wikipedia.org/wiki/M3U "M3U") 播放列表文件，用于寻找可用的媒体流

```nginx
user usr24 usr24; # 配置启动用户

rtmp {
    server {
        listen 1935;  # RTMP端口
        chunk_size 4096;

        application live {
            live on;
            record off;

            # HLS配置
            hls on;
            hls_path /tmp;  # HLS切片存储路径
            hls_fragment 3s;  # 每个切片的时长
        }
    }
}

http {
    server {
        listen 8080;  # HTTP端口
        location / {
            root /home/usr24/NetDiskProject/video/;  # 指向HLS切片存储路径
            add_header Cache-Control no-cache;
        }
    }
}
```

- `nignx`重启即可通过`http://172.24.105.254/hls/ror2.mp4`链接访问视频
```bash
sudo nginx -s reload
```

### 客户端播放器实现
- 基于 FFmpeg 和 SDL
##### 主要功能
1. 视频和音频的解码与同步播放
	- 支持同时解码视频和音频流，并实现音视频同步
	- 使用 FFmpeg 进行解码，SDL 进行音频播放和定时器管理
2. 播放控制
	- 支持播放、暂停、停止、跳转（seek）等基本播放控制
	- 通过 play()、pause()、stop()、seek() 等方法控制播放状态
3. 多线程解码
	- 视频解码在独立线程中进行（继承自 QThread）
	- 音频解码通过 SDL 的回调机制实现
4. 音视频数据队列管理
	- 维护音频和视频的解码包队列，防止内存溢出（有阈值限制）
	- 跳转时清空队列和解码器缓存，避免花屏和音画不同步
5. 信号与槽机制
	- 通过 Qt 信号槽机制，将解码后的图像发送到界面显示
	- 发送播放状态、总时长等信号
6. 支持本地和网络文件播放
	- 通过 FFmpeg 的网络初始化和中断回调，支持网络流媒体播放
7. 音视频同步
	- 通过音频时钟和视频时钟的对比，实现音视频同步播放

##### 主要流程

- 初始化 FFmpeg、SDL，打开媒体文件，查找音视频流
- 分别初始化音频和视频解码器，创建解码线程和音频回调
- 主线程循环读取媒体包，分发到音频/视频队列
- 视频线程/定时器解码视频帧，发送到界面显示
- SDL 音频回调解码音频帧，填充到音频缓冲区播放
- 支持播放控制（暂停、继续、停止、跳转）
- 资源回收和线程安全退出

##### 视频预览流程

![[视频预览.png]]


---
---

# 视频播放背景知识
认识一个人
- 雷霄骅 中国传媒大学的博士生 通信与信息系统专业 [博客](https://blog.csdn.net/leixiaohua1020?type=blog)
- 有很大贡献

## 不同封装格式
`wmv` `flv` `mp4` `mov` `avi` `mkv` `rmvb`
- 用不同的后缀表示,这些格式代表封装格式
- 封装格式 即把视频数据和音频数据打包成一个文件的规范
- 不同的封装格式之间差距不大, 各有优劣

## 什么是分辨率?

- 横向像素数和纵向像素数的乘积
- 常见的 超高清 1080p 也就是 1920\*1080, 也就是1张画面长是1920像素, 宽是1080像素 , 长宽可以形成一个平面空间, 空间上有1920\*1080个点, 就是我们所谓的像素点
- 比如一个像素点是 16位数据表示的(BMP或PNG格式图片), 那么一个1080p的图片大小是可以算出来的(大概是4MB)

## 视频播放器原理

视音频技术主要包含以下几点:
- 封装技术
- 视频压缩编码技术以及音频压缩编码技术
- 如果考虑到网络传输的话，还包括流媒体协议技术

## 播放流程
![[视频播放流程.png]]
## 流媒体协议

| <br>名称<br>   | <br>推出机构<br> | <br>传输层协议<br> | <br>客户端<br> | <br>目前领域    |
| ------------ | ------------ | ------------- | ----------- | ----------- |
| RTSP+RTP     | IETF         | TCP+UDP       | VLC WMP     | IPTV 以及各大直播 |
| RTMP         | Adobe        | TCP           | Flash       | 互联网直播       |
| HTTP         | WWW+IETF     | TCP           | Flash       | 互联网点播       |

## 封装格式


| 名称   | 推出机构          | 流媒体 | 支持的视频编码                       | 支持的音频编码                              | 目前领域      |
| ---- | ------------- | --- | ----------------------------- | ------------------------------------ | --------- |
| AVI  | 微软            | 不支持 | 几乎所有格式                        | 几乎所有格式                               | BT下载影视    |
| MP4  | MPEG          | 支持  | MPEG-2, MPEG-4,H.264, H.263等  | AAC, MPEG-1 Layers I, II, III, AC-3等 | 互联网视频网站   |
| TS   | MPEG          | 支持  | MPEG-1, MPEG-2, MPEG-4, H.264 | MPEG-1 Layers I,II, III, AAC,        | IPTV，数字电视 |
| FLV  | Adobe         | 支持  | Sorenson, VP6,H.264           | MP3, ADPCM,Linear PCM, AAC等          | 互联网视频网站   |
| MKV  | CoreCodec     | 支持  | 几乎所有格式                        | 几乎所有格式                               | 互联网视频网站   |
| RMVB | Real NetWorks | 支持  | RealVideo 8, 9, 10            | AAC, Cook Codec,RealAudio Lossless   | BT下载影视    |

## 相关概念
*编码*(encode): 通过特定的压缩技术,将某个视频的视频流格式转换成另一种视频格式的视频流方式。
*解码*(decode): 通过特定的解压缩技术,将某个视频格式的视频流转换成另一种视频格式的视频流方式。
*转码*(transcode): 视频转码技术将视频信号从一种格式转换成另一种格式。

*视频*:
- 改变分辨率(resolution)
- 改变帧率(frame rate)
- 改变比特率 等编码参数

*音频*:
- 改变采用率(sample rate)
- 改变通道数(channels)
- 改变位宽(sample format)

*封装*(mux):复用,按一定格式组织原视音视频流
*解封装*(demux):解复用,按一定格式解析出原始音视频流

*rtsp流*: RTSP(Real Time Streaming Protocol), RFC2326,实时流传输协议,是TCP/IP协议体系中的一个 应用层协议。
*rtmp流*: Real Time Messaging Protocol(实时消息传输协议),是adobe公司的协议
*hls流*: HLS是Apple的动态码率自适应技术。主要用于PC和Apple终端的音视频服务。包括一个m3u8索引文件,TS媒体分片文件

*推模式*: 当通知消息来之时,把所有相关信息都通过参数的形式“推给”观察者。
*拉模式*: 当通知消息来之时,通知的函数不带任何相关的信息,而是要观察者主动去“拉”信息
*实时流*: Real Time stream 实时传输的音视频流

---



> [!quote] 其他

---
# Qt`connect` 

```cpp
QObject::connect(act_addFriend, SLOT(triggered(bool)), this, SLOT(slot_addFriend(bool)));
QObject::connect(act_addFriend, &QAction::triggered, this, &FriendList::slot_addFriend);
```

*第二种更好 :*

1. **类型安全**：第二种写法使用了函数指针，编译器可以在编译时检查信号和槽的参数类型是否匹配，提供更好的类型安全。
2. **更好的性能**：第一种写法使用字符串形式的 `SLOT` 和 `SIGNAL`，在运行时解析，可能导致性能损耗。而第二种写法是在编译时解析的，性能更优。
3. **重构友好**：第二种写法在重构代码时更加友好。如果你重命名槽函数，编译器会提示错误，而第一种写法可能会在运行时才发现问题。
4. **现代 Qt 风格**：第二种写法符合现代 Qt 的编程风格，通常被认为是最佳实践。

---
# QT程序发布
`windeployqt ` + path exe


---






















---
# End
---