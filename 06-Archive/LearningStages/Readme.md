> "这不是黑历史, 这是我的来时路"

[STAGE01](STAGE01/STAGE01.md)  
[STAGE02](STAGE02/STAGE02.md) 
[STAGE03](STAGE03/STAGE03.md) 

[STAGE05](STAGE05/STAGE05.md) 

[Advanced](Advanced.md) 

# Begin

````markdown title:"OLD Overview" fold
# 这部分是干什么

这部分是基本的学习内容, 从0开始学习C/C++.

大致学习方向: C语言->**C++**->`计算机网络`->`MySQL数据库`->Linux操作系统->*数据结构与算法*
都是重点



----

# `C`

[这一阶段](00-STAGES/Stage1.md), 学习如何通过敲键盘让电脑能够运行简单程序, 比如输入任意两个数, 求他们的和.
基本编程原理, 编译原理, 程序是什么, 编程语言是什么, C语言语法, 各种语言基本通用的关键字.
掌握了基础就可以用别人的工具比如easyX图形库来完成一些简单的小游戏, 比如俄罗斯方块, 贪吃蛇.

一阶段项目: easyX实现贪吃蛇



[[0720]] C语言从0到1
[[0721]] 变量 `scanf`
[[0722]] 源代码执行原理

[[0724-0725]] 流程结构基础 流程结构嵌套
[[0726]] 整型存储原理

[[0728]] 字符型·浮点型存储原理
[[0729]] 运算符·表达式
[[0730]] 流程结构·综合运用

[[0801]] 指针基础
[[0802]] 一维数组

[[0805]] 一维数组与指针
[[0806]] 二维数组与指针
[[00-STAGES/Stage_01/Unit02/0807/0807]] 指针典型笔试题

[[0809]] `easyX`初体验
[[0810]] 项目实战：贪吃蛇
[[0811]] 自定义函数

[[0813]] 函数封装技巧与项目重构
[[00-STAGES/Stage_01/Unit03/0814/0814]] 函数指针·`typedef`·枚举
[[00-STAGES/Stage_01/0815]] `const`·宏·变量作用域

[[0817]] 堆与栈·函数递归
[[0818-0819]] 项目实战：俄罗斯方块
[[00-STAGES/Stage_01/0820]] 数组组件

[[0831A]] 字符串原理与常用函数
[[0831P]] 字符串函数与字符串组件封装（1）
[[0901A]] 字符串组件封装（2）
[[0901P]] 结构体·共用体

[[0907A]] 链表原理
[[0907P]] 链表组件封装（一）
[[0908A]] 链表组件封装（二）
[[0908P]] 组件打包与使用

[[0915A-P]] 文件处理


----

# `C++`

其实C++可以和C一起学习, 没必要分开. [这一阶段](00-STAGES/Stage2.md)主要是学习C和C++的一些区别, C++的新特性, C++的发展史, 同时了解C的发展史.
同样掌握了基础就可以学一些进阶的, 比如用C++的QT库开发桌面应用, 也就是电脑上跑的一些软件, 比如微信.

二阶段项目: 利用基于easyX的游戏框架实现飞机大战


## Chapter 1 *C++基础*

[[0921]] 
1. C++/C 部分区别
2. C++基础1
[[0922]] C++基础2

 01.C++基础1--------09.21 AM
 02.C++基础2--------09.21 PM
 03.C++基础3--------09.22 AM


---
## Chapter 2 *类基础*

[[0928]] Class

 04.C++类基础--------09.28 AM
 05.C++类练习--------09.28 PM

---
## Chapter 3 *类成员进阶* 

[[1013A]] Class Expand 

 06.C++类成员进阶1--------10.13 AM

---
## Chapter 4 *类之间的关系* 

[[1013P 类的横向关系]] 组合 - 依赖 - 关联 - 聚合 横向

 07.C++类成员进阶2--------10.13 PM

---

[[1019 继承]]  
[[1020]] 多态 虚函数

 08.C++继承--------10.19 AM
 09.C++继承2--------10.19 PM
 10.C++多态--------10.20 AM
 11.C++多态2--------10.20 PM

---
## Chapter 5 
[[1026]] 
	头文件与原文件
	`define` 
	内联函数`inline`

[[1027]] 
	迭代器`itreator`
	运算符重载`operator`
	`<list>` 
	

 12.探索程序--------10.26 AM
 13.探索程序2--------10.26 PM
 14.迭代器和链表--------10.27 AM
 15.宠物小屋--------10.27 PM

---

[[1102-1103-1109]] __飞机大战__

 16.宠物小屋2--------11.02 AM
 17.飞机大战分析--------11.02 PM
 18.飞机大战开发--------11.03 AM
 19.飞机大战开发2--------11.03 PM
 20.飞机大战开发3--------11.09 AM
 21.飞机大战开发完结--------11.09 PM

[[1110]] 拷贝构造

 22.拷贝构造--------11.10 AM

---

[[1116-1117AM]] 
[[1117PM]] __STL__

 23.设计模式单例--------11.16 PM
 24.设计模式工厂--------11.17 AM
 25.STL1--------11.17 PM



[[1123]]  

[[1130]]   
[[1201]]  
[[00-STAGES/Stage_02/1202]] 
[[1203]] 
[[1204]] 



----

# 计算机网络

计算机网络体系结构, ARP协议, DNS协议, ICMP协议等, 网络层IP协议. 传输层TCP/UDP协议. 应用层HTTP协议, HTTPS协议等

学习网络编程, 利用套接字实现程序间通信, 主机间通信.

> ==2025==


0113 [[250113]] ISO/OSI
0114 [[250114]] `UDP_Server` 
0115 [[250115]] `UDP_Clinet` 

0117 [[250117]] 子网划分 IP首部 以太网首部
0118 [[250118]] 发送接收缓冲区 阻塞非阻塞
0119 [[250119]] TCP首部 TCP机制
0120 [[250120]] 心跳机制 Nagle算法 拥塞控制
0121 [[250121]] 数据库
	0122 
	0123 
	0124 
	0125 

0308 [[250308]] Linux基础命令
0309 [[250309]] `vi gcc gdb makefile`

0315 [[250315]] Linux文件
0316 [[250316]] 虚拟文件系统 Windows内存管理



----


## MySQL数据库


基本增删改查, 触发器, 视图, 存储过程. 事务, 索引, 锁.

推荐微信公众号"组织爱熊猫"

[这一阶段](00-STAGES/Stage3.md)项目: 结合计算机网络部分, 基于QT完成一个Instance Message

----

# Linux操作系统

啥都有, 也就那几块知识点.

面试主要问进程间通信等

[这一阶段](00-STAGES/Stage4.md)项目: C语言实现epoll+线程池的高并发服务器

[[0330.github_regex]]  

----

[[0413.]] 

----

[[0419.进程Process]] 
[[0420.并发应用开发(多进程拷贝)]] 

----

[[0426.ICP进程间通信]] 

---
[[0510.守护进程Deamon]] 
[[0511.pthread线程基础]] 

----

[[0517.]] 
[[0518.线程安全4线程互斥访问_线程同步]] 

----


[[0525.信号]] 


---
---
---
---

[[0606.SOCKET]] 
[[0606.服务器01]] 
[[0607.服务器02]] 
[[0608.服务器03]] 
[[0609.服务器04]] 





----

# 数据结构与算法

基本数据结构, 栈,堆, 队列, 链表, 树. 图.
leetcode刷题, 一天一道.

[数据结构与算法](00-STAGES/Stage5.md)



[[00-STAGES/Stage_05/0826]] 时间 空间复杂度
[[0828-0829]] value-index
[[0830]] Find Unique Number `^`

[[0902]] Add Without `+`
[[0902 Bubble Sort]] 
[[0904]] Insert Sort; Select Sort
[[0905]] Shell Sort 
[[0906]] Fibonacci

[[0909 Quick Sort]] 
[[0909 Binary Chop]]
[[0911]] Merge Sort; Quick Sort
[[0912]] Link List
[[0913]] Merge 2 Link List

[[0918]] 
[[0919]] Stack
[[0920]] Queue


[[0923]] 
1. Queue By 2 Stack;
2. Joseph Ring;
3. Solid Window
[[0925]] 
4. Stack By 2 Queue;
5. 空格替换;
6. 单词反转;
[[0926]] KMP
[[0927]] Sunday

[[1009]] Hash Table
[[1010 Hash More]] 
[[1010 Bucket Sort]] 
[[1010 Radix Sort]] 
[[1010 Count Sort]] 
[[1011]] Count Sort 优化
[[1011 Tree]] 


[[1014]] 二叉树 续 
[[1016]] 二叉树竖向遍历 0代替空节点的构造
[[1017]] 二叉树数组构造 横向遍历(层序遍历)
[[1018 Level Order With 'Enter']] 
[[1018 Heap Sort]] 


[[1021]] 二叉树循环遍历
[[1023]] 递归时间复杂度 BST创建 删除节点
[[1024 节]] 
[[1025]] 
	BST转有序双向链表
	判断二叉树是否平衡
	BST旋转

[[1028]] __红黑树__ 
[[1030]] BRT增
[[1031-1101]] RBT删

[[1106]] $B$-Tree $B^+$-Tree
[[1107]] 并查集
[[1108]] 哈夫曼树 字典树 

[[1111]] 
[[1113]] 
[[1114]] 

[[1118-Graph]] 
[[1118-DP]] 


## 数据结构可视化


Currently, we have visualizations for the following data structures and algorithms:

## Basics

- [Stack: Array Implementation](https://www.cs.usfca.edu/~galles/visualization/StackArray.html)
- [Stack: Linked List Implementation](https://www.cs.usfca.edu/~galles/visualization/StackLL.html)
- [Queues: Array Implementation](https://www.cs.usfca.edu/~galles/visualization/QueueArray.html)
- [Queues: Linked List Implementation](https://www.cs.usfca.edu/~galles/visualization/QueueLL.html)
- Lists: Array Implementation (available in [java](https://www.cs.usfca.edu/~galles/visualization/java/visualization.html) version)
- Lists: Linked List Implementation (available in [java](https://www.cs.usfca.edu/~galles/visualization/java/visualization.html) version)

## Recursion

- [Factorial](https://www.cs.usfca.edu/~galles/visualization/RecFact.html)
- [Reversing a String](https://www.cs.usfca.edu/~galles/visualization/RecReverse.html)
- [N-Queens Problem](https://www.cs.usfca.edu/~galles/visualization/RecQueens.html)

## Indexing

- [Binary and Linear Search (of sorted list)](https://www.cs.usfca.edu/~galles/visualization/Search.html)
- [Binary Search Trees](https://www.cs.usfca.edu/~galles/visualization/BST.html)
- [AVL Trees (Balanced binary search trees)](https://www.cs.usfca.edu/~galles/visualization/AVLtree.html)
- [Red-Black Trees](https://www.cs.usfca.edu/~galles/visualization/RedBlack.html)
- [Splay Trees](https://www.cs.usfca.edu/~galles/visualization/SplayTree.html)
- [Open Hash Tables (Closed Addressing)](https://www.cs.usfca.edu/~galles/visualization/OpenHash.html)
- [Closed Hash Tables (Open Addressing)](https://www.cs.usfca.edu/~galles/visualization/ClosedHash.html)
- [Closed Hash Tables, using buckets](https://www.cs.usfca.edu/~galles/visualization/ClosedHashBucket.html)
- [Trie (Prefix Tree, 26-ary Tree)](https://www.cs.usfca.edu/~galles/visualization/Trie.html)
- [Radix Tree (Compact Trie)](https://www.cs.usfca.edu/~galles/visualization/RadixTree.html)
- [Ternary Search Tree (Trie with BST of children)](https://www.cs.usfca.edu/~galles/visualization/TST.html)
- [B Trees](https://www.cs.usfca.edu/~galles/visualization/BTree.html)
- [B+ Trees](https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html)

## Sorting

- [Comparison Sorting](https://www.cs.usfca.edu/~galles/visualization/ComparisonSort.html)
    - Bubble Sort
    - Selection Sort
    - Insertion Sort
    - Shell Sort
    - Merge Sort
    - Quick Sort
- [Bucket Sort](https://www.cs.usfca.edu/~galles/visualization/BucketSort.html)
- [Counting Sort](https://www.cs.usfca.edu/~galles/visualization/CountingSort.html)
- [Radix Sort](https://www.cs.usfca.edu/~galles/visualization/RadixSort.html)
- [Heap Sort](https://www.cs.usfca.edu/~galles/visualization/HeapSort.html)

## Heap-like Data Structures

- [Heaps](https://www.cs.usfca.edu/~galles/visualization/Heap.html)
- [Binomial Queues](https://www.cs.usfca.edu/~galles/visualization/BinomialQueue.html)
- [Fibonacci Heaps](https://www.cs.usfca.edu/~galles/visualization/FibonacciHeap.html)
- [Leftist Heaps](https://www.cs.usfca.edu/~galles/visualization/LeftistHeap.html)
- [Skew Heaps](https://www.cs.usfca.edu/~galles/visualization/SkewHeap.html)

## Graph Algorithms

- [Breadth-First Search](https://www.cs.usfca.edu/~galles/visualization/BFS.html)
- [Depth-First Search](https://www.cs.usfca.edu/~galles/visualization/DFS.html)
- [Connected Components](https://www.cs.usfca.edu/~galles/visualization/ConnectedComponent.html)
- [Dijkstra's Shortest Path](https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html)
- [Prim's Minimum Cost Spanning Tree](https://www.cs.usfca.edu/~galles/visualization/Prim.html)
- [Topological Sort (Using Indegree array)](https://www.cs.usfca.edu/~galles/visualization/TopoSortIndegree.html)
- [Topological Sort (Using DFS)](https://www.cs.usfca.edu/~galles/visualization/TopoSortDFS.html)
- [Floyd-Warshall (all pairs shortest paths)](https://www.cs.usfca.edu/~galles/visualization/Floyd.html)
- [Kruskal Minimum Cost Spanning Tree Algorithm](https://www.cs.usfca.edu/~galles/visualization/Kruskal.html)

## Dynamic Programming

- [Calculating nth Fibonacci number](https://www.cs.usfca.edu/~galles/visualization/DPFib.html)
- [Making Change](https://www.cs.usfca.edu/~galles/visualization/DPChange.html)
- [Longest Common Subsequence](https://www.cs.usfca.edu/~galles/visualization/DPLCS.html)

## Geometric Algorithms

- [2D Rotation and Scale Matrices](https://www.cs.usfca.edu/~galles/visualization/RotateScale2D.html)
- [2D Rotation and Translation Matrices](https://www.cs.usfca.edu/~galles/visualization/RotateTranslate2D.html)
- [2D Changing Coordinate Systems](https://www.cs.usfca.edu/~galles/visualization/ChangingCoordinates2D.html)
- [3D Rotation and Scale Matrices](https://www.cs.usfca.edu/~galles/visualization/RotateScale3D.html)
- [3D Changing Coordinate Systems](https://www.cs.usfca.edu/~galles/visualization/ChangingCoordinates3D.html)

## Others ...

- [Disjoint Sets](https://www.cs.usfca.edu/~galles/visualization/DisjointSets.html)
- Huffman Coding (available in [java](https://www.cs.usfca.edu/~galles/visualization/java/visualization.html) version)

````




# End

