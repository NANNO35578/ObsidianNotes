<div style="display: flex; align-items: center;">
    <img src="RelationMap.png" alt="nginx logo" style="width: 500px; margin-left: 10px;">
	<div style="flex: 1;">
		<h1><strong style="color:#e67e80">Welcome</strong> to my <em>Repository</em>.</h1>
		<p>Make a note of <strong style="color:#a7c080">something</strong>, This is <u style="color:#7fbbb3">Introduction</u>;</p>
		<p><del>Study note in Colin;</del> Code Note</p>
    </div>
</div>

[^1] this content will show while ctrl pressed and mouse pointed

[TestLink-Actully RTC](note_RTC/RTC_README.md)

DS api key:sk-34135fb42fda40059f13e792ec111a4f

----

# 不知道叫什么好

|             | Link                  |            Topic             |
| ----------- | --------------------- | :--------------------------: |
| Stage One   | [[Stage1]]            |              C               |
| Stage Two   | [[Stage2]]            |             C++              |
| Stage Three | [[Stage3]]            | Network & MySQL & IM & Linux |
| Stage Four  | [[Stage4]]            |            Linux             |
| Stage Five  | [[Stage5]]            | data structures & algorithms |
|             |                       |                              |
| BRUSH       | [[OverviewBrush]]     |  Brush algorithm questions   |
| interview   | [[OverviewInterview]] |                              |
| RTC basic   | [[RTC_README]]        |      Created By Cursor       |
| Diploma     | [[Deploma]]           |                              |
|             | [[OverviewAndroid]]   |                              |
|             | [[OverviewReview]]    |                              |
|             | [[OverviewOtherMD]]   |                              |

# [[Interview Questions]]


# 结构体中柔性数组重载运算符`{cpp}new`

```cpp title:"柔性数组重载 new"
/// 音频数据帧
///  成员描述
struct STRU_AUDIO_FRAME {
  int type;
  int frameSize;
  // 为了兼容更广的编译器，可以使用 [1] 而非 []：
  char audioFrame[];  // ? -> char[];

  /// 1) 带两个参数：baseSize 是 sizeof(STRU_AUDIO_FRAME)，
  //    frameSize 是用户想要的额外字节数
  /// @brief #### 使用时: STRU_AUDIO_FRAME *rq= new (size) STRU_AUDIO_FRAME(size);
  static void *operator new(std::size_t baseSize, std::size_t frameSize) {
    return ::operator new(baseSize + frameSize);
  }

  // 2) 匹配的 delete，确保 delete p 正常工作
  static void operator delete(void *p) noexcept { ::operator delete(p); }

  // 可选：在构造抛出时被调用（C++14 起才需要）
  static void operator delete(void *p, std::size_t, std::size_t) noexcept {
    ::operator delete(p);
  }

  STRU_AUDIO_FRAME(int size) : type(DEF_PACK_AUDIO_FRAME), frameSize(size) {
    memset(audioFrame, 0, frameSize);
  }

  void setFrame(const char *frame, std::size_t size) {
    memcpy(audioFrame, frame, size);
  }

  int getSize( ) const { return sizeof(STRU_AUDIO_FRAME) + frameSize; }
};
```


##### Grade Three Experiments
- [x] 信息存储实验一0913--------1
- [x] 信息存储实验二0927--------2 n
- [x] 信息存储实验三1010--------- n
- [x] 信息存储实验四1025--------
- [x] 无线传感网实验一二1014-------- r
- [x] 无线传感网实验三四1021-------- r
- [x] Unix实验一1009--------
- [x] Unix实验二1017--------
- [x] Unix实验三1031--------
- [x] Unix实验四1114--------
- [x] 网络设备实验一1016-------- r
- [x] 网络综合布线实验一1009-------- r
- [x] 网络综合布线实验二1011-------- n
- [x] 网络综合布线实验三1018-------- r
- [x] 网络综合布线实验四1115---------
- [ ] 网络设备实验二1023-------- 
- [x] 网络设备实验三1101--------
- [ ] 网络设备实验四1106--------
- [ ] 协议开发实验一1022-------- 
- [ ] 协议开发实验二1108--------
- [ ] 协议开发实验三1120--------
- [ ] 协议开发实验四1204  --------

---

# 可以刷题的Online Judge 
[HRBUST_OJ](http://acm.hrbust.edu.cn/)
[电子科技大学CDOJ](http://acm.uestc.edu.cn)
[北京大学PKU](http://acm.pku.edu.cn/JudgeOnline/)
[浙江大学ZJU](http://acm.zju.edu.cn/)
[杭州电子科技大学HDU][http://acm.hdu.edu.cn/]
[哈尔滨工程大学hrbeu](http://acm.hrbeu.edu.cn/)
[同济大学TJU](http://acm.tongji.edu.cn/)
[浙江工商Zjgsu](http://acm.zjgsu.edu.cn/JudgeOnline/)
[宁波理工NIT](http://acm.nit.net.cn)
[北京化工大学BUCT](http://coder.buct.edu.cn/oj/)
[中国地质大学CUG](http://lab.cug.edu.cn/COJ/)
[中国科技大学USTC](http://acm.ustc.edu.cn/)
[中山大学Zsu](http://acm.zsu.edu.cn/)
[吉林大学jlu](http://acm.jlu.edu.cn/)
[浙江林学院ZJFC](http://info.zjfc.edu.cn:2000/)
[暨南大学Zhuhai]( http://202.116.24.78/JudgeOnline/index.acm)
[武汉大学whu](http://acm.whu.edu.cn/) 
[南开大学Naikai]( http://acm.nankai.edu.cn/)
[大连理工Dlut](http://acm.dlut.edu.cn/)
[哈尔滨工业大学HIT](http://acm.hit.edu.cn/)
[北京邮电大学Bupt]( http://acm.cs.bupt.cn/onlinejudge/)
[湖南大学HNU](http://acm.hnu.cn:8080/online/)
[香港大学hkoi](http://judge.hkoi.org/)
[天津大学TJU](http://cs.tju.edu.cn/acm/)
[四川大学SCU](http://acm.scu.edu.cn/)
[汕头大学STU](http://acm.stu.edu.cn/)
[福州大学FZU](http://acm.fzu.edu.cn/)
[厦门大学XMU](http://acm.xmu.edu.cn/JudgeOnline/)
[福建师范大学FJNU](http://acm.fjnu.edu.cn/)
[华中科技大学HUST](http://acm.hust.edu.cn/JudgeOnline/)
[华东师范大学ECNU](http://acm.cs.ecnu.edu.cn/)
[浙江工业大学ZJUT](http://acm.zjut.edu.cn/)
[浙江师范大学ZJNU](http://acm.zjnu.cn/)
[南京航空航天大学](http://acm.nuaa.edu.cn)
[西南科技大学swust](http://acm.swust.edu.cn:8080/JudgeOnline/)
[华东理工大学ecust](http://acm.ecust.edu.cn/)
[西南科技大学](http://acm.swust.edu.cn/oj/)



------

# `Markdown` 语法

## 通用语法

### 基本语法

| 代码              | 作用                       |
| --------------- | ------------------------ |
| `-`             | 渲染为一个点, 同理还有`*`, `+`     |
| `1. `           | 渲染为有序列表                  |
| `- [ ] `        | 将渲染为代办, 写成`- [x]` 将渲染为完成 |
| `#`             | 渲染为一级标题, 同理`##` 为二级标题    |
| `**bold**`      | 文本加粗                     |
| `*Italic*`      | 文本倾斜                     |
| `~~delete~~`    | 文本删除线                    |
| `==highlight==` | 文本高亮                     |
| `---`           | 渲染为分隔线                   |
| \`\`\`代码块\`\`\` | 渲染代码块                    |
| \`code\`        | 渲染一行代码                   |
| `[name](url)`   | 渲染链接                     |
| `>`             | 自己试试(quote)              |
| `[^1]`          | 引用                       |

### 用得上的HTML

`{html}<strong style="color:#e67e80">HTML</strong>` 强调, 可配置颜色, 这个例子渲染的HTML字样是这样的: <strong style="color:#e67e80">HTML</strong>
其他颜色:
- <strong style="color:a7c080">编辑器</strong>  <strong style="color:e69875">编辑器</strong>  <strong style="color:e67e80">编辑器</strong>  <strong style="color:d0b2dd">编辑器</strong>  <strong style="color:7fbbb3">编辑器</strong>  <strong style="color:dbbc7f">编辑器</strong>  <strong style="color:d3c6aa">编辑器</strong> 

`{html}<u style="color:#e67e80">下划线</u>` HTML的下划线, 这个例子渲染的下划线字样是这样的: <u style="color:#e67e80">下划线</u>. 空格同样渲染: <u style="color:#e67e80"> </u> ,但只渲染一个, 可以这样:  <u style="color:#e67e80"> | | | . . . .</u>

`{html}<em>文本倾斜</em>` HTML的文本倾斜, 这个例子渲染的下划线字样是这样的: <em>文本倾斜</em>

HTML代码块:
<code class="language-cpp">
int main() {
    return 0;
}
</code>
<code>Code</code> 一行代码


<h1>HTML Head 1</h1>
<h6>HTML Head 6</h6>
<ul>
	<li> li</li>
	<li> li</li>
</ul>
<ol>
	<li> li</li>
	<li> li</li>
	</ol>
<p style="color:red;">这是一段红色文字</p>
<h2 style="color:blue;">蓝色标题</h2>
<strong style="color:#3c7;">加粗并且带颜色</strong>
<p style="font-size:1.5em;">使用 em（1.5 倍）</p>
<p style="font-size:120%;">使用百分比（120%）</p>
<p style="font-size:1.2rem;">使用 rem（1.2 倍）</p>
#### 其他HTML语法

用于缩进:
<dl>
  <dt>First Term</dt>
  <dd>This is the definition of the first term.</dd>
  <dd><div style="display: inline-block; width: 600px; height: 10px; background-color: #D3C6AA;"></div>  </dd>
  <dt>获取 <strong style="color:#E67E80;">出口公网 IP</strong></dt>
  <dd><code>curl -s ifconfig.me</code> 或者<code> dig +short myip.opendns.com @resolver1.opendns.com</code> 所以显示的就是通过 ISP 出口看到的地址（可能是 NAT 分配的）</dd>
  <li>如果你在 <strong style="color:#E67E80;">WSL</strong> 里运行这个tmux插件，获取的 IP 也是 Windows 宿主机的公网出口 IP，不会是 WSL 内部的 <code>192.168.*.*</code> 或 <code>172.*.*</code></li>
</dl>

左侧嵌入图片:
<div style="display: flex; align-items: center;">
    <img src="https://github.com/user-attachments/assets/9335b488-ffcc-4157-8364-2370a0b70ad0" alt="nginx logo" style="width: 200px; margin-left: 10px;">
	<div style="flex: 1;">
        <p><strong>Nginx</strong></p>
        <p>异步框架的<a href="https://zh.wikipedia.org/wiki/%E7%B6%B2%E9%A0%81%E4%BC%BA%E6%9C%8D%E5%99%A8" title="网页服务器">网页服务器</a>，也可以用作<a href="https://zh.wikipedia.org/wiki/%E5%8F%8D%E5%90%91%E4%BB%A3%E7%90%86" title="反向代理">反向代理</a>、<a href="https://zh.wikipedia.org/wiki/%E8%B4%9F%E8%BD%BD%E5%9D%87%E8%A1%A1" title="负载均衡">负载平衡器</a>和<a href="https://zh.wikipedia.org/wiki/HTTP%E7%BC%93%E5%AD%98" title="HTTP缓存">HTTP缓存</a>。</p>
        <p>Nginx使用异步事件驱动的方法来处理请求。Nginx的模块化事件驱动架构<a href="https://zh.wikipedia.org/wiki/Nginx#cite_note-aosabook-15">[15]</a>可以在高负载下提供更可预测的性能<a href="https://zh.wikipedia.org/wiki/Nginx#cite_note-Configuration-16">[16]</a>。</p>
    </div>
</div>


颜色块:
<sub>default</sub>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #D3C6AA;"></div>  
<sub>accents</sub>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #E67E80;">#E67E80</div><div style="display: inline-block; width: 50px; height: 50px; background-color: #E69875;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #DBBC7F;">DBBC7F</div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #A7C080;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #83C092;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #7FBBB3;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #D699B6;"></div>  
<sub>greys</sub>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #7A8478;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #859289;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #9DA9A0;"></div>  
<sub>statusline</sub>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #A7C080;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #D3C6AA;"></div>  
<div style="display: inline-block; width: 50px; height: 50px; background-color: #E67E80;"></div>

## obsidian语法


> [!note] 

> [!bug] 

> [!abstract] 

> [!info] 

> [!todo] 

> [!tip] 

> [!success] 

> [!question] 

> [!warning] 

> [!quote] 

> [!example] 

> [!error] 

> [!fail] 

> [!code] 

> [!question] Can callouts be nested?
> nice
> > [!todo] Yes!, they can.
> > 哈哈哈
> > > [!example]  You can even use multiple layers of nesting.
> > > okkk
> > 
> > 2
> 
> 1

> [!abstract] 
>> ```cpp
>> template <typename T, typename U>
>> auto add(T t, U u) -> decltype(t + u) {
>>  return t + u;
>> }
>> ```
> - 好使
> 1. 分点1
> 2. 分点2

## obsidian插件语法

只展示示例
### Code Styler

```cpp title:"Add Title For CodeBlock"
#include <iostream>
```

`{cpp}std::cout<<"Render Color For lineCode"<<'\n';`
### Iconize

:RiNumbersFill: :RiNumber1: :LiCode2: :MiVim: :VsFileTypeEmacs: :MiVscode: :VsFileTypeAccess2: :VsFileTypeOutlook:

### List Callout
- ! 
- @ 
- $ 
- % 
- & 
- ~ 
- / haha a

### Badges


 `[!!note:note]`       `[!!info:haha]`       `[!!abstract:haha]`       `[!!tip:haha]`       `[!!done:haha]`       `[!!help:haha]` 
 `[!!warning:haha]`       `[!!fail:haha]`       `[!!danger:haha]`       `[!!bug:haha]`       `[!!example:haha]`       `[!!quote:haha]`

 `[!!notice:haha]`       `[!!charge:haha]`       `[!!refine:haha]`       `[!!meta:haha]`       `[!!home:haha]`       `[!!specimen:haha]`
 `[!!emergency:haha]`       `[!!claim:haha]`       `[!!dig:haha]`       `[!!hold:haha]`       `[!!milestone:haha]`       `[!!image:haha]`  

 `[!!compute:haha]`       `[!!account:haha]`       `[!!command:haha]`       `[!!prohibit:haha]`       `[!!highlight:haha]`       `[!!witness:haha]`      
 `[!!sprout:haha]`       `[!!component:haha]`       `[!!party:haha]`       `[!!compass:haha]`       `[!!judgment:haha]`       `[!!deed:haha]`   
   
 `[!!stop:haha]`       `[!!profile:haha]`       `[!!attachment:haha]`       `[!!extract:haha]`       `[!!polish:haha]`       `[!!crystallize:haha]`
 `[!!map:haha]`       `[!!balance:haha]`       `[!!honor:haha]`       `[!!bomb:haha]`       `[!!snippet:haha]`       `[!!lightbulb:haha]`   
   
 `[!!power:haha]`       `[!!point:haha]`       `[!!expedition:haha]`       `[!!feast:haha]`       `[!!reward:haha]`       `[!!magnet:haha]`       
 `[!!lock:haha]`       `[!!branch:haha]`       `[!!verse:haha]`       `[!!dream:haha]`       `[!!definition:haha]`       `[!!flag:haha]`    
   
 `[!!hat-tip:haha]`       `[!!exclaim:haha]`       `[!!complete:haha]`       `[!!process:haha]`       `[!!mention:haha]`       `[!!knowledge:haha]`
 `[!!gift:haha]`       `[!!vault:haha]`       `[!!love:haha]`       
 
 `[!!|message-square|comment:haha|var(--color-cyan-rgb)]`       `[!!|dice|roll:haha|120,82,238]`       `[!!|dice|roll:haha|120,82,238]`       `[!!|gem|mineral:haha|var(--my-custom-rgb)]`       `[!!|apple|fruit:haha|var(--color-red-rgb)]`       `[!!|banana|fruit:haha|var(--color-red-rgb)]`
 `[!!|apple|fruit:haha|var(--color-red-rgb)]`       `[!!|brain|brain:haha|var(--color-purple-rgb)]`       `[!!|sun|weather:haha|var(--color-yellow-rgb)]`       `[!!|cloudy|weather:haha|var(--mono-rgb-100)]`       `[!!|moon|weather:haha|var(--mono-rgb-100)]`       `[!!|sun|weather:haha|var(--mono-rgb-100)]`       
 `[!!|earth|weather:haha|var(--mono-rgb-100)]`       `[!!|sunset|weather:haha|var(--color-orange-rgb)]`       `[!!|dumbbell|reps:haha|var(--mono-rgb-00)]`       `[!!|gift|event:haha|var(--color-blue-rgb)]`       `[!!|plus-square|credit:haha|var(--color-green-rgb)]`       `[!!|minus-square|debit:haha|var(--color-pink-rgb)]`       
 `[!!|bug|debit:haha|230,126,128]`       `[!!|computer|debit:haha|127,192,168]`       `[!!|ghb>haha:1.0]`    `[!!|ghs>okay:success]`



----

# 一些字符

- **不同字体会包含不同字符, 可以在Windows的字符映射表查看"

````text title:"Some Characters"

←───→      ⟵────⟶   ↑ ↓ ↑ ↓
⇐────⇒     ⬅────➡     ⭠────⭢
┌────┐     ┌────┐     ┌────┐
│ A1 │──▶──│ A2 │──▶──│ A3 │
└─┬──┘     └────┘     └────┘
  │
  
▼                 ⮀ ⮂ ⮁ ⮃ ◢ ◣ ◤ ◥ ▸ ▹                                                
┌──────────────────┬────┬────┐
│                  │    │    │
│                  │    │    │
├──────────────────┼────┼────┤
│                  │    │    │
│                  │    │    │
└──────────────────┴────┴────┘

````




----
----
-----
----
----
----
----
----
----
---


[^1]: this is a what
	i don't know
	okay🫠
	
