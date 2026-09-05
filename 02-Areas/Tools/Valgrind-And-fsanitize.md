# `valgrind`

### 使用方法

1. **运行 Valgrind**：  
    运行程序时使用 Massif：
```bash
    valgrind --tool=massif ./your_program
```

2. **生成报告**：  
    生成的 `massif.out.<pid>` 文件可以使用 `ms_print` 工具进行分析：
```bash
    ms_print massif.out.56683
```

3. **可视化**：  
    还可以使用工具（如 `massif-visualizer`）对输出结果进行图形化展示，帮助更直观地理解内存使用情况

---

### 3. **运行 Valgrind**

使用 Valgrind 运行你的程序，并检测内存泄漏：

```bash
valgrind --leak-check=full ./my_program
```

解释：

* **`--leak-check=full`**: 启用详细的内存泄漏检查。
* **`--show-reachable=yes`**: 显示所有仍然可以访问的内存（默认开启）。

### 4. **分析 Valgrind 输出**

运行程序后，Valgrind 会输出报告，包括内存分配的详细信息以及是否存在泄漏。如果有内存泄漏，Valgrind 会提供有关泄漏的位置、大小和上下文的详细信息。

一些关键字：

* **"definitely lost"**: 明确的内存泄漏。
* **"indirectly lost"**: 通过其他变量间接丢失的内存。
* **"still reachable"**: 程序结束时仍能访问的内存，但不一定是泄漏。
* **"possibly lost"**: 可能发生泄漏，但不确定。

示例输出：

```
==12345== 10 bytes in 1 blocks are definitely lost in loss record 1 of 1
==12345==    at 0x4C2FB55: operator new(unsigned long) (vg_replace_malloc.c:299)
==12345==    by 0x109149: main (my_program.cpp:12)
```

### 5. **修复内存泄漏**

根据 Valgrind 提供的报告，返回到源代码中，定位泄漏发生的位置，并修复内存泄漏（如使用 `free()` 释放动态分配的内存）。

### 6. **再次运行 Valgrind**

修复完问题后，重新编译并再次运行 Valgrind，确认问题是否解决。

如果仍然存在内存泄漏，Valgrind 会再次显示相应的报告，你可以继续调试，直到所有泄漏问题被解决。

### 常见的 Valgrind 参数

* `--leak-check=yes`: 启用内存泄漏检查。
* `--track-origins=yes`: 追踪内存未初始化的根源。
* `--show-leak-kinds=all`: 显示所有类型的泄漏，包括直接和间接泄漏。
* `--num-callers=20`: 显示最多 20 个调用栈。

### 7. **查看报告文件**

如果你需要更详细的报告，也可以将输出写入文件：

```bash
valgrind --leak-check=full --log-file=valgrind_report.txt ./my_program
```

然后你可以查看 `valgrind_report.txt` 文件，里面包含了详细的报告。

### 总结

通过上述步骤，你可以使用 Valgrind 高效地检测和修复 C/C++ 程序中的内存泄漏。确保在编译时启用调试信息，运行 Valgrind 检查内存泄漏，并根据输出报告修复代码

---
# `-fsanitize` 

你提到的参数是 **`-fsanitize=address`**，它是 GCC 和 Clang 编译器提供的一个功能，能够在程序运行时检测内存错误（如越界访问、使用未初始化的内存、内存泄漏等），并提供详细的堆栈跟踪信息。

### 使用 `-fsanitize=address` 启用地址消毒器

1. **编译时加上 `-fsanitize=address` 参数：**

   在编译时使用 `-fsanitize=address` 来启用 **AddressSanitizer (ASan)**，该工具会自动检测内存错误，并在发生错误时输出详细的堆栈跟踪。

   ```bash
   g++ -g -fsanitize=address -o my_program my_program.cpp
   gcc -g -fsanitize=address -o my_program my_program.c
   ```

   这里的 `-g` 选项依然是启用调试信息，以便能显示更详细的堆栈信息。

2. **运行程序：**

   编译后，直接运行你的程序：

   ```bash
   ./my_program
   ```

   如果程序出现了内存错误（如越界访问、内存泄漏、重复释放等），**AddressSanitizer** 会输出详细的错误信息，并给出错误发生的堆栈信息，帮助定位问题。

### 地址消毒器（ASan）的输出示例：

```bash
=================================================================
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000200 at pc 0x0000004dfc85 bp 0x7ffddde0c1f0 sp 0x7ffddde0c1e8
READ of size 4 at 0x602000000200 thread T0
    #0 0x4dfc84 in main (my_program+0x4dfc84)
    #1 0x7f98764c8382 in __libc_start_main (libc.so.6+0x23882)
    #2 0x4dfac8 in _start (my_program+0x4dfac8)
```

### 常见内存错误类型：

* **Heap-buffer-overflow**：堆内存溢出访问错误。
* **Stack-buffer-overflow**：栈内存溢出访问错误。
* **Use-after-free**：使用已经释放的内存。
* **Use-after-return**：返回后使用已经返回的内存。
* **Memory leak**：内存泄漏（与 Valgrind 类似，但 ASan 更专注于运行时检查）。
* **Uninitialized memory read**：读取未初始化的内存。

### 其他有用的 `-fsanitize` 选项：

* **`-fsanitize=undefined`**: 检测未定义行为（如整数溢出、空指针解引用等）。
* **`-fsanitize=thread`**: 检测线程数据竞争问题。
* **`-fsanitize=leak`**: 检测内存泄漏（类似于 Valgrind，但更轻量级）。

### 总结：

通过使用 `-fsanitize=address`，你可以在运行时检测并定位内存错误，并提供清晰的堆栈跟踪信息，这对于调试 C/C++ 程序中的内存问题非常有帮助。



---

```cpp
#include <iostream>
#include <list>
#include <string>

void leakFunc(int id) {
    char sqlStr[] = "";
    sprintf(sqlStr, "SELECT * From t_user Where u_id = %d;", id);

    std::list<std::string> lst;
    lst.push_back("hahha");
    lst.push_back("SELECT * From t_user Where u_id = 199;");

    std::cout << "haha leakFunc:" << id << '\n';
}

int main(int argc, char const *argv[]) {
    leakFunc(199);

    std::cout << "leak main" << '\n';
    return 0;
}
```
对于以上程序, 在`leakFunc`中写溢出, 根据命令`g++ -g -fsanitize=address leak.cpp -o leak`编译后运行输出的内容

![[../../'attachments/fsaninite.png]]



---
# 其他参数

除了 `-fsanitize=address` 之外，GCC 和 Clang 提供了多个 **`-fsanitize`** 参数用于不同类型的程序错误检测。以下是常见的几种 **`-fsanitize`** 参数，及其用途：

### 1. **`-fsanitize=address`**

* **用途**：检测内存错误（越界访问、使用未初始化内存、内存泄漏等）。
* **常见问题**：内存溢出、堆栈溢出、未初始化内存访问等。

示例：

```bash
g++ -g -fsanitize=address -o my_program my_program.cpp
```

### 2. **`-fsanitize=undefined`**

* **用途**：检测程序中未定义的行为（undefined behavior）。这包括整数溢出、除以零、空指针解引用等。
* **常见问题**：整型溢出、零除、空指针访问等。

示例：

```bash
g++ -g -fsanitize=undefined -o my_program my_program.cpp
```

**常见检测内容**：

* **整数溢出**：例如对整数加法、乘法等操作时溢出。
* **空指针解引用**：尝试解引用空指针。
* **签名扩展**：对负数进行不正确的类型转换。

示例输出：

```bash
main.cpp:12:5: runtime error: signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'
```

### 3. **`-fsanitize=leak`**

* **用途**：检测内存泄漏。此选项检查程序是否有未释放的内存。
* **常见问题**：动态内存分配后没有正确释放，导致内存泄漏。

示例：

```bash
g++ -g -fsanitize=leak -o my_program my_program.cpp
```

**注**：与 `valgrind` 类似，`-fsanitize=leak` 检测程序的内存泄漏，但更轻量级，并且运行速度更快。

### 4. **`-fsanitize=thread`**

* **用途**：检测线程数据竞争（race conditions）。这对于多线程程序非常有用，能够检测出多个线程间对同一内存位置的非同步访问。
* **常见问题**：数据竞争，两个线程在没有适当同步的情况下访问共享变量。

示例：

```bash
g++ -g -fsanitize=thread -o my_program my_program.cpp
```

**常见问题**：

* **数据竞争**：多个线程同时读写同一内存，且没有加锁等同步机制。
* **死锁**：线程因为相互等待而无法继续执行。

### 5. **`-fsanitize=integer`**

* **用途**：检测整数相关的问题，如整数溢出和转换错误。
* **常见问题**：整型溢出、负数到无符号数的转换等。

示例：

```bash
g++ -g -fsanitize=integer -o my_program my_program.cpp
```

### 6. **`-fsanitize=bool`**

* **用途**：检查布尔值的错误使用，尤其是在布尔类型和整数类型之间的隐式转换。
* **常见问题**：错误的布尔类型转换（例如将 `0` 解释为 `true`，将非零值解释为 `false`）。

示例：

```bash
g++ -g -fsanitize=bool -o my_program my_program.cpp
```

### 7. **`-fsanitize=vptr`**

* **用途**：检测虚拟函数调用的错误，特别是使用虚函数时，错误的虚表指针（vtable pointer）访问。
* **常见问题**：使用不合法的虚函数指针。

示例：

```bash
g++ -g -fsanitize=vptr -o my_program my_program.cpp
```

### 8. **`-fsanitize=safe-stack`**

* **用途**：启用栈保护，保护函数栈溢出（例如防止栈溢出攻击）。此选项会插入额外的代码，监视栈上的内存，以确保栈的完整性。
* **常见问题**：栈溢出攻击、缓冲区溢出等。

示例：

```bash
g++ -g -fsanitize=safe-stack -o my_program my_program.cpp
```

### 9. **`-fsanitize=memory`**

* **用途**：启用内存错误检测。此选项检测内存访问的错误（如访问未初始化的内存、非法内存访问等）。
* **常见问题**：读取未初始化的内存、访问已经释放的内存等。

示例：

```bash
g++ -g -fsanitize=memory -o my_program my_program.cpp
```

### 10. **`-fsanitize=coverage`**

* **用途**：启用代码覆盖率检查。在测试期间，记录每个执行的代码行，并生成覆盖率报告。
* **常见问题**：检查哪些代码行未被测试，帮助进行单元测试覆盖率分析。

示例：

```bash
g++ -g -fsanitize=coverage -o my_program my_program.cpp
```

### 11. **`-fsanitize=shift`**

* **用途**：检查位移运算（如左移、右移）的错误，特别是移位数值不合理的情况。
* **常见问题**：将负数或超过位宽的数值进行位移。

示例：

```bash
g++ -g -fsanitize=shift -o my_program my_program.cpp
```

### 综合使用多个 `-fsanitize` 参数：

你可以组合多个 `-fsanitize` 参数来启用多个错误检查，例如：

```bash
g++ -g -fsanitize=address,undefined,leak -o my_program my_program.cpp
```

这将启用 **内存错误检查**、**未定义行为检查** 和 **内存泄漏检查**。

---

### 总结

* **`-fsanitize=address`**：内存错误（越界、未初始化等）
* **`-fsanitize=undefined`**：未定义行为（整数溢出、空指针解引用等）
* **`-fsanitize=leak`**：内存泄漏
* **`-fsanitize=thread`**：线程数据竞争
* **`-fsanitize=integer`**：整数错误（溢出等）
* **`-fsanitize=bool`**：布尔类型错误
* **`-fsanitize=vptr`**：虚拟函数指针错误
* **`-fsanitize=safe-stack`**：栈溢出保护
* **`-fsanitize=memory`**：内存访问错误
* **`-fsanitize=coverage`**：代码覆盖率检查
* **`-fsanitize=shift`**：位移错误

使用这些 `-fsanitize` 选项，可以在开发过程中及时发现程序中的各种潜在错误和漏洞，提升代码质量和安全性。
