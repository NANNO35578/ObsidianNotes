## `list`
- `list, l` : 列出指定的函数或行
行可以通过以下方式指定：
  - `LINENUM`，列出当前文件中该行周围的内容，
  - `FILE:LINENUM`，列出指定文件中该行周围的内容，
  - `FUNCTION`，列出该函数开头周围的内容，
  - `FILE:FUNCTION`，用于区分同名的静态函数。
  - `*ADDRESS`，列出包含该地址的行周围的内容。

 
## `break`
-  `break [PROBE_MODIFIER] [LOCATION] [thread THREADNUM] [-force-condition] [if CONDITION]` 在指定位置设置断点 
- LOCATION 可以是行规格、地址或下文描述的明确位置
- 如果没有 LOCATION，则使用所选堆栈帧的当前执行地址


![[gdbbreak.png]]

## `continue`
- `continue [N]` 继续调试的程序，在信号或断点之后
- 如果从断点继续，可以将数字 N 作为参数使用，这意味着将该断点的忽略计数设置为 N - 1（这样断点在第 N 次到达之前不会触发）
- 如果启用了非停止模式，则仅继续当前线程；否则，程序中的所有线程都会继续。要在非停止模式下继续所有已停止的线程，使用 -a 选项
- 同时指定 -a 和忽略计数是一个错误


## `step`
- `step [N]` 执行源代码一行，不跳过函数
- N 代表执行N次, 或直达程序因其他原因停止

## `next`
- 源代码执行一行，跳过函数


## `watch` 查看变量
- `watch [-location] EXPRESSION` 设置一个监控断点以监视表达式（EXPRESSION）
- -location : 这将评估表达式，并监视其所指向的内存
监控断点会在表达式的值发生变化时停止程序的执行并打印变化

## `info`
`info, inf, i` : 通用命令，用于显示被调试程序的信息
info 子命令列表：
- `info address` -- 描述符号 SYM 的存储位置。
- `info all-registers` -- 列出所选堆栈帧的所有寄存器及其内容。
- `info args` -- 当前堆栈帧的所有参数变量或匹配 REGEXPs 的变量。
- `info breakpoints, info b` -- 指定断点的状态（如果没有参数，则列出所有用户可设置的断点）。
- `info functions` -- 所有函数名或匹配 REGEXPs 的函数名。
- `info locals` -- 当前堆栈帧的所有局部变量或匹配 REGEXPs 的变量。
- `info program` -- 程序的执行状态。
- `info registers, info r` -- 所选堆栈帧的整数寄存器及其内容列表。
- `info stack, info s` -- 堆栈的回溯，或最内层的 COUNT 帧。
- `info threads` -- 显示当前已知的线程。
- `info watchpoints` -- 指定观察点的状态（如果没有参数，则列出所有观察点）

![[gdbinfo.png]]
## `where`
 - **显示堆栈帧**：列出当前所有堆栈帧及其对应的源代码位置
 - `backtrace`



---
## 查看源码
- `layout src` 上下拆分
- 左右拆分: 
```bash
tui new-layout aka {-horizontal cmd 1 src 1 } 2 # 一个cmd 一个src
layout aka
set tui active-border-mode half # 设置边框
```

## 查看反汇编
- disassemble
```bash
(gdb) disassemble mine::addVal(int)
Dump of assembler code for function _ZN4mine6addValEi:
   0x0000000000001166 <+0>:     push   %rbp
   0x0000000000001167 <+1>:     mov    %rsp,%rbp
   0x000000000000116a <+4>:     sub    $0x10,%rsp
   0x000000000000116e <+8>:     mov    %rdi,-0x8(%rbp)
   0x0000000000001172 <+12>:    mov    %esi,-0xc(%rbp)
   0x0000000000001175 <+15>:    mov    -0x8(%rbp),%rax
   0x0000000000001179 <+19>:    mov    (%rax),%edx
   0x000000000000117b <+21>:    mov    -0xc(%rbp),%eax
   0x000000000000117e <+24>:    add    %eax,%edx
   0x0000000000001180 <+26>:    mov    -0x8(%rbp),%rax
   0x0000000000001184 <+30>:    mov    %edx,(%rax)
   0x0000000000001186 <+32>:    mov    -0x8(%rbp),%rax
   0x000000000000118a <+36>:    mov    %rax,%rdi
   0x000000000000118d <+39>:    call   0x1156 <_ZN4mine6getValEv>
   0x0000000000001192 <+44>:    leave
   0x0000000000001193 <+45>:    ret
End of assembler dump.
```



## 查看函数名称修饰
- 反汇编 `_ZN4mine6addValEi  => int mine::addVal(int)` 


