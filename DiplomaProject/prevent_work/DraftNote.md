# C


````markdown title:"C"

# C语言学习笔记（10篇完整版）
## 笔记1：C语言基础入门与环境搭建
### 核心知识点
1. **C语言概述**：编译型高级语言，执行效率高，常用于系统开发、嵌入式开发
2. **开发环境**：
   - 编译器：GCC、Clang、MSVC
   - 编辑器：VS Code、Dev-C++、CLion
3. **第一个C程序（固定模板）**
```c
#include <stdio.h>  // 标准输入输出头文件

// main函数：程序入口，有且仅有一个
int main() {
    printf("Hello, C Language!\n");  // 输出函数
    return 0;  // 程序正常结束
}
```
4. **程序执行流程**：编写代码 → 编译 → 链接 → 运行
5. **基础语法规则**：语句以`;`结尾，区分大小写，注释不执行

### 编译运行命令
```bash
gcc test.c -o test  # 编译
./test              # 运行(Linux/Mac)
test.exe            # 运行(Windows)
```

---

## 笔记2：C语言数据类型与变量
### 核心知识点
1. **数据类型分类**
   - 基本类型：整型(`int`)、字符型(`char`)、浮点型(`float/double`)
   - 无类型：`void`
2. **常用数据类型大小**
   | 类型     | 字节数 | 取值范围                |
   |----------|--------|-------------------------|
   | int      | 4      | -21亿~21亿              |
   | char     | 1      | -128~127                |
   | float    | 4      | 小数，6位有效数字       |
   | double   | 8      | 小数，15位有效数字      |
3. **变量定义与使用**
```c
// 格式：类型 变量名 = 值;
int age = 20;         // 整型变量
char ch = 'A';        // 字符变量
float score = 95.5f;  // 浮点变量
double pi = 3.14159;  // 双精度浮点
```
4. **常量**：`const int MAX = 100;`（不可修改）
5. **命名规则**：字母、数字、下划线，不能以数字开头，不能用关键字

---

## 笔记3：输入与输出函数
### 核心知识点
1. **标准输出：printf()**
```c
printf("字符串\n");  // 普通输出
printf("%d", 10);    // 输出整型
printf("%c", 'a');   // 输出字符
printf("%f", 3.14);  // 输出浮点数
printf("%s", "abc"); // 输出字符串
```
2. **标准输入：scanf()**
```c
int num;
scanf("%d", &num);  // &：取地址符，必须加

// 多变量输入
int a, b;
scanf("%d %d", &a, &b);
```
3. **常用转义字符**
   - `\n`：换行
   - `\t`：制表符（空格）
   - `\\`：输出反斜杠
   - `\'`/`\"`：输出单/双引号

---

## 笔记4：运算符与表达式
### 核心知识点
1. **算术运算符**
   `+ - * / %`（加、减、乘、除、取余）
   - 注意：整数除法`5/2=2`，小数除法`5.0/2=2.5`
2. **赋值运算符**
   `= += -= *= /= %=`
   ```c
   a += 1;  // 等价 a = a + 1;
   ```
3. **自增/自减运算符**
   `++`（自增1）、`--`（自减1）
   - 前置：`++a`（先加1，再使用）
   - 后置：`a++`（先使用，再加1）
4. **关系运算符**
   `> < >= <= == !=`（结果：真=1，假=0）
5. **逻辑运算符**
   `&&`（与）、`||`（或）、`!`（非）

---

## 笔记5：流程控制-分支语句
### 核心知识点
1. **if 语句**
```c
// 单分支
if(条件){
    执行语句;
}

// 双分支
if(条件){
    语句1;
}else{
    语句2;
}

// 多分支
if(条件1){}
else if(条件2){}
else{}
```
2. **switch 语句**（适用于固定值判断）
```c
switch(变量){
    case 1: 语句; break;
    case 2: 语句; break;
    default: 默认语句; break;
}
```
3. **注意**：switch只能判断**整型/字符型**，必须加`break`防止穿透

---

## 笔记6：流程控制-循环语句
### 核心知识点
1. **for 循环**（固定次数循环）
```c
for(初始化; 条件; 更新){
    循环体;
}
// 示例：循环10次
for(int i=0; i<10; i++){
    printf("%d ", i);
}
```
2. **while 循环**（条件循环）
```c
while(条件){
    循环体;
}
```
3. **do-while 循环**（先执行，再判断）
```c
do{
    循环体;
}while(条件);
```
4. **循环控制**
   - `break`：跳出整个循环
   - `continue`：跳过本次循环，执行下一次

---

## 笔记7：数组的定义与使用
### 核心知识点
1. **数组概念**：相同类型数据的集合，占用连续内存
2. **一维数组**
```c
// 定义：类型 数组名[长度];
int arr[5];  // 定义长度为5的整型数组
// 初始化
int arr[5] = {1,2,3,4,5};
int arr[] = {1,2,3};  // 自动计算长度
```
3. **数组访问**：下标从**0**开始
```c
arr[0] = 10;  // 第一个元素
printf("%d", arr[0]);
```
4. **二维数组**（矩阵）
```c
int arr[2][3] = {{1,2,3},{4,5,6}};
// 访问：arr[行][列]
```
5. **字符串数组**：`char str[] = "hello";`（以`\0`结尾）

---

## 笔记8：函数基础
### 核心知识点
1. **函数作用**：封装代码，复用、简化程序
2. **函数定义格式**
```c
返回值类型 函数名(参数列表){
    函数体;
    return 返回值;
}
```
3. **示例：求和函数**
```c
// 定义
int add(int a, int b){
    return a + b;
}

// 调用
int res = add(10, 20);
```
4. **函数分类**
   - 无参无返：`void fun(){}`
   - 有参有返：`int fun(int a){}`
5. **函数声明**：调用前必须声明（头文件/顶部声明）

---

## 笔记9：指针入门
### 核心知识点
1. **指针概念**：存放**内存地址**的变量
2. **指针定义**
```c
int a = 10;
int *p = &a;  // *定义指针，&取变量地址
```
3. **指针操作**
   - 取值：`*p`（访问指针指向的变量）
   - 取地址：`&a`（获取变量a的地址）
```c
printf("%d", *p);  // 输出10
*p = 20;           // 等价 a=20
```
4. **指针与数组**
```c
int arr[] = {1,2,3};
int *p = arr;
printf("%d", *(p+1));  // 输出arr[1]=2
```
5. **空指针**：`int *p = NULL;`（安全指针）

---

## 笔记10：字符串处理函数
### 核心知识点
1. **字符串本质**：以`\0`结尾的字符数组
2. **必备头文件**：`#include <string.h>`
3. **常用字符串函数**
```c
char str1[20] = "hello";
char str2[20] = "world";

// 1. 求长度 strlen()
int len = strlen(str1);  // len=5

// 2. 拷贝 strcpy()
strcpy(str1, str2);  // str1 = "world"

// 3. 拼接 strcat()
strcat(str1, str2);  // str1 = "helloworld"

// 4. 比较 strcmp()
int res = strcmp(str1, str2);  // 0=相等，>0/<0
```
4. **字符串输入/输出**
```c
char str[20];
scanf("%s", str);   // 输入（无空格）
printf("%s", str);  // 输出
gets(str);          // 输入（支持空格）
puts(str);          // 输出并换行
```

---

### 总结
10篇笔记覆盖**C语言核心基础**：环境搭建→数据类型→输入输出→运算符→分支循环→数组→函数→指针→字符串，从入门到基础实战全覆盖，可直接用于学习、复习、作业使用。
````


# Java

````markdown title:"Java"

# Java 笔记 01：面向 C++ 开发者的 Java 基础认知与核心差异
## 1. 语言定位与执行模型
- C++：编译型语言，源码→编译→链接→本地机器码，直接运行在操作系统，跨平台需重新编译。
- Java：半编译半解释型，源码→编译为**字节码（.class）**，由 JVM（Java 虚拟机）解释/即时编译执行，一次编译跨平台运行。
- 无手动内存管理：JVM 自带 GC（垃圾回收），无需像 C++ 手动 `new/delete` 管理堆内存，避免野指针、内存泄漏。

## 2. 基本数据类型与对应关系
Java 基本类型均为**值类型**，无指针，大小固定不随平台变化：

| C++ 类型 | Java 对应类型 | 位数 | 说明 |
|---------|-------------|-----|------|
| bool    | boolean     | 1bit | 仅 `true/false`，不可与数字互转 |
| char    | char        | 16bit | Unicode 字符，而非 ASCII |
| short   | short       | 16 | - |
| int     | int         | 32 | - |
| long    | long        | 64 | C++ long 平台相关，Java 固定 64 位 |
| float   | float       | 32 | - |
| double  | double      | 64 | - |

注意：
- Java 无 `unsigned` 类型。
- 字符串是 `String` 类对象，不是字符数组，不可直接修改，类似 C++ `const string`。

## 3. 语法结构差异
### 3.1 入口方法
C++：
```cpp
int main() { ... }
```
Java：
必须写在类中，静态入口方法：
```java
public class Demo {
    public static void main(String[] args) {
        // 程序入口
    }
}
```

### 3.2 无指针、无引用符号
- Java 无 `*`、`->`、`&` 指针操作。
- 对象均通过**引用**操作，语法类似 C++ 指针，但无地址运算。

### 3.3 数组
C++：
```cpp
int arr[5];
int* arr = new int[5];
```
Java：
```java
int[] arr = new int[5];   // 固定长度，初始化默认 0/false/null
arr.length;              // 获取长度，属性而非方法
```
数组是对象，越界会抛出 `ArrayIndexOutOfBoundsException`，而非未定义行为。

## 4. 类与对象基础
- 一切代码必须依附于**类**，无全局函数、全局变量。
- 成员访问控制：`public`/`private`/`protected`，默认包访问权限（C++ 无此概念）。
- 创建对象：
  ```java
  // 等价 C++ Object* obj = new Object();
  Object obj = new Object();
  ```
- 无析构函数，对象不再被引用时由 GC 自动回收。

## 5. 异常机制
- 替代 C++ 错误码、段错误，Java 强制/非强制异常处理。
- 关键字：`try/catch/finally/throw/throws`。
- 空指针访问抛出 `NullPointerException`，而非崩溃。

## 6. 核心总结（C++ 视角）
1. Java = 无指针、带 GC、基于 JVM 的跨平台面向对象语言。
2. 语法接近 C++，但更严谨，无未定义行为。
3. 内存、数组越界、空指针等问题由 JVM 统一处理。
4. 所有代码必须在类中，无全局概念。






Java 笔记（共10篇，第二篇）

# Java 笔记 02：面向 C++ 开发者的 Java 类与对象进阶

## 1. 类的定义与访问控制（对比 C++）

Java 类的定义语法与 C++ 相似，但访问控制更严格，且无全局类、友元机制（C++ 友元在 Java 中通过包访问权限或反射间接实现）。

### 1.1 类的基本结构

C++ 类定义：

```cpp
class Person {
private:
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {} // 构造函数
    void show() { cout << name << "," << age << endl; }
};
```

Java 类定义（对应写法）：

```java
public class Person {
    // 成员变量（默认包访问权限，需显式写private）
    private String name;
    private int age;

    // 构造函数（无返回值，与类名完全一致，无默认参数）
    public Person(String n, int a) {
        name = n;
        age = a;
    }

    // 成员方法
    public void show() {
        System.out.println(name + "," + age);
    }
}
```

### 1.2 访问控制修饰符（核心差异）

Java 有4种访问权限（C++ 仅3种，无包访问权限），优先级：public > protected > 包访问（默认） > private：

- private：仅当前类可访问（与 C++ private 一致）
    
- 默认（无修饰符）：同一包内的类可访问（C++ 无此权限，类似“半公开”）
    
- protected：同一包内 + 子类可访问（C++ protected 仅子类可访问，包内无关类不可访问）
    
- public：所有类可访问（与 C++ public 一致）
    

注意：Java 无 C++ 的“友元函数/友元类”，若需跨类访问私有成员，需通过 public getter/setter 方法（这是 Java 规范）。

## 2. 构造函数与析构函数（关键差异）

### 2.1 构造函数

相同点：都是对象创建时自动调用，用于初始化成员变量。

核心差异：

- Java 构造函数无返回值（包括 void），C++ 构造函数也无返回值，但 Java 严禁写返回值类型（写了就变成普通方法）。
    
- Java 支持**构造函数重载**（与 C++ 一致），但无 C++ 的“默认参数”（需通过重载实现类似效果）。
    
- Java 有**默认构造函数**：若类中未定义任何构造函数，JVM 自动生成无参构造；若定义了构造函数，默认构造函数失效（C++ 也有默认构造，但规则一致）。
    

示例（构造函数重载，替代 C++ 默认参数）：

```java
public class Person {
    private String name;
    private int age;

    // 无参构造（手动定义，替代默认构造）
    public Person() {
        name = "未知";
        age = 0;
    }

    // 单参构造
    public Person(String n) {
        name = n;
        age = 18; // 默认年龄
    }

    // 双参构造
    public Person(String n, int a) {
        name = n;
        age = a;
    }
}
```

### 2.2 析构函数（无对应概念）

C++ 析构函数（~类名()）：对象销毁时调用，用于释放手动分配的内存（如 new 的资源）。

Java 无析构函数：因为 JVM 有 GC（垃圾回收），会自动回收对象占用的堆内存，无需手动释放。

补充：若需在对象回收前执行特定操作（如关闭文件、释放网络连接），可重写 `finalize()` 方法，但该方法由 GC 调用，调用时机不确定，不推荐使用（替代方案：手动写 close() 方法，主动调用）。

## 3. 对象创建与内存分配（对比 C++ 指针）

### 3.1 对象创建方式（核心差异）

C++ 对象创建有2种方式（栈对象、堆对象）：

```cpp
Person p1("张三", 20); // 栈对象，自动销毁
Person* p2 = new Person("李四", 22); // 堆对象，需手动 delete
```

Java 对象创建只有1种方式（堆对象），通过 `new` 关键字，无需手动释放：

```java
Person p1 = new Person("张三", 20); // 堆对象，GC 自动回收
Person p2 = new Person("李四", 22);
```

关键说明：

- Java 中的 `Person p1` 不是对象本身，而是**对象引用**（类似 C++ 指针，但无地址运算、无指针操作）。
    
- C++ 栈对象在栈上分配内存，Java 无栈对象（基本类型是栈存储，对象均在堆上）。
    
- Java 无 `delete` 关键字，对象引用置为 `null` 后，若无人引用，GC 会在合适时机回收。
    

### 3.2 空指针（NullPointerException）

C++ 空指针（NULL）：本质是 0，可通过指针运算规避崩溃（如判断指针非空再访问），但访问空指针会导致段错误（崩溃）。

Java 空指针（null）：专门表示“无引用对象”，访问 null 引用的成员（方法/变量）会抛出 `NullPointerException`（异常），程序不会直接崩溃，可通过 try-catch 捕获处理。

示例（空指针对比）：

```cpp
// C++：访问空指针，崩溃（未定义行为）
Person* p = NULL;
p->show(); // 段错误
```

```java
// Java：访问空指针，抛出异常（可捕获）
Person p = null;
p.show(); // 抛出 NullPointerException
```

## 4. 静态成员（static）与 C++ 对比

Java static 关键字用法与 C++ 类似，用于修饰“属于类、不属于单个对象”的成员，但有细微差异：

- 静态成员变量：所有对象共享，初始化时需在类外（或类内直接赋值），无 C++ 的“类内静态常量必须类外初始化”的限制。
    
- 静态成员方法：无 `this` 指针（类似 C++ 静态方法无 `this`），不能访问非静态成员（变量/方法），可通过“类名.方法名”直接调用。
    
- Java 无 C++ 的“静态局部变量”（函数内 static 变量），仅支持类级别的 static 成员。
    

示例（Java 静态成员）：

```java
public class Student {
    private String name;
    // 静态成员变量：所有学生共享同一个班级名称
    public static String className = "Java班";

    // 静态成员方法：无需创建对象即可调用
    public static void showClassName() {
        // 不能访问非静态成员 name（报错）
        System.out.println("班级：" + className);
    }

    public Student(String n) {
        name = n;
    }
}

// 调用静态成员（无需new对象）
Student.showClassName(); // 输出：班级：Java班
System.out.println(Student.className); // 输出：Java班
```

## 5. 核心总结（C++ 视角）

1. Java 类访问控制多了“包访问权限”，无友元，私有成员需通过 getter/setter 访问。
    
2. 构造函数无返回值、无默认参数，析构函数被 GC 替代，无需手动管理内存。
    
3. 对象只有堆创建方式，引用类似 C++ 指针但无指针操作，空指针抛出异常而非崩溃。
    
4. static 成员用法类似 C++，但无静态局部变量，静态方法不能访问非静态成员。





Java 笔记（共10篇，第三篇）

# Java 笔记 03：面向 C++ 开发者的 Java 继承与多态

## 1. 继承基础（对比 C++，核心差异）

Java 继承与 C++ 继承本质一致，都是“代码复用、类的扩展”，但语法、访问控制、继承规则有显著差异，且 Java 仅支持**单继承**（C++ 支持多继承）。

### 1.1 继承语法（最直观差异）

C++ 继承语法（支持多继承）：

```cpp
// 单继承
class Student : public Person {
private:
    int score;
public:
    Student(string n, int a, int s) : Person(n, a), score(s) {}
};

// 多继承（Java 不支持）
class A {};
class B {};
class C : public A, public B {};
```

Java 继承语法（仅单继承，用 extends 关键字）：

```java
// 单继承：Student 继承自 Person
public class Student extends Person {
    // 子类新增成员变量
    private int score;

    // 子类构造函数：必须先调用父类构造（默认调用父类无参构造）
    public Student(String n, int a, int s) {
        super(n, a); // 调用父类双参构造，类似 C++ 初始化列表 Person(n,a)
        this.score = s;
    }
}
```

### 1.2 核心差异：单继承 vs 多继承

- Java 仅支持**单继承**：一个子类只能有一个直接父类（避免 C++ 多继承的菱形继承、二义性问题）。
    
- C++ 支持多继承：一个子类可继承多个父类，易出现二义性（需用作用域解析符 :: 区分），Java 无此问题。
    
- Java 用“接口（interface）”替代多继承的功能（后续笔记详解），实现多接口达到类似多继承的效果，但无多继承的副作用。
    

### 1.3 父类构造函数的调用（关键差异）

相同点：子类构造时，必须先调用父类构造函数，再执行子类构造。

核心差异：

- C++：通过“初始化列表”调用父类构造，若不写，默认调用父类无参构造。
    
- Java：通过 `super()`关键字调用父类构造，若不写，默认调用父类无参构造；若父类无无参构造，必须显式写 `super(参数)` 调用父类有参构造（否则编译报错）。
    

示例（父类无无参构造，子类必须显式调用父类有参构造）：

```java
// 父类：无无参构造，只有双参构造
public class Person {
    private String name;
    private int age;

    public Person(String n, int a) { // 无无参构造
        name = n;
        age = a;
    }
}

// 子类：必须显式调用父类双参构造，否则编译报错
public class Student extends Person {
    private int score;

    public Student(String n, int a, int s) {
        super(n, a); // 必须写，否则报错（父类无无参构造）
        score = s;
    }
}
```

## 2. 方法重写（Override）与 C++ 对比

Java 方法重写与 C++ 虚函数重写本质一致：子类重写父类的方法，实现“子类特有行为”，但语法、限制更严格。

### 2.1 语法差异

C++：用`virtual` 关键字修饰父类方法，子类无需特殊关键字，只要方法签名一致即可重写（协变返回值除外）。

```cpp
class Person {
public:
    virtual void show() { // 虚函数，允许子类重写
        cout << "我是Person" << endl;
    }
};

class Student : public Person {
public:
    void show() override { // override 可选，用于检查重写是否正确
        cout << "我是Student" << endl;
    }
};
```

Java：无需给父类方法加 virtual，子类重写时，需满足“方法签名完全一致”，且可加 `@Override` 注解（可选，但推荐，用于检查重写正确性）。

```java
public class Person {
    // 父类方法，无需加virtual，默认允许重写
    public void show() {
        System.out.println("我是Person");
    }
}

public class Student extends Person {
    // 重写父类show方法，@Override注解用于校验（写错签名会报错）
    @Override
    public void show() {
        System.out.println("我是Student");
    }
}
```

### 2.2 重写的限制（Java 特有）

Java 方法重写有“访问权限不能缩小”的限制（C++ 无此限制）：

- 父类方法是 public，子类重写时必须是 public（不能是 protected/private）。
    
- 父类方法是 protected，子类重写时可以是 protected 或 public（不能是 private）。
    
- 父类方法是 private，子类无法重写（因为 private 仅当前类可访问，子类看不到该方法）。
    

示例（错误重写）：

```java
public class Person {
    public void show() { // 父类public
        System.out.println("我是Person");
    }
}

public class Student extends Person {
    // 错误：子类重写方法访问权限缩小（public→private）
    @Override
    private void show() {
        System.out.println("我是Student");
    }
}
```

## 3. 多态（核心差异：无指针，靠引用实现）

Java 多态与 C++ 多态原理一致（动态绑定），但实现方式不同：C++ 靠“虚函数 + 指针”实现多态，Java 靠“方法重写 + 对象引用”实现（无指针）。

### 3.1 多态实现示例（对比 C++）

C++ 多态（虚函数 + 指针）：

```cpp
Person* p1 = new Person();
Person* p2 = new Student(); // 父类指针指向子类对象（多态核心）
p1->show(); // 输出：我是Person
p2->show(); // 输出：我是Student（动态绑定，调用子类重写方法）
delete p1;
delete p2;
```

Java 多态（对象引用 + 方法重写）：

```java
Person p1 = new Person();
Person p2 = new Student(); // 父类引用指向子类对象（多态核心，无指针）
p1.show(); // 输出：我是Person
p2.show(); // 输出：我是Student（动态绑定，调用子类重写方法）
// 无需delete，GC自动回收
```

### 3.2 多态的核心条件（Java 与 C++ 一致）

1. 存在继承关系（子类继承父类）。
    
2. 子类重写父类的方法（方法签名一致、访问权限不缩小）。
    
3. 父类引用（C++ 是父类指针）指向子类对象。
    

### 3.3 Java 多态的特有细节

- Java 无指针，所有对象都是通过“引用”操作，多态的实现更简洁（无需担心指针越界、空指针崩溃，仅抛出异常）。
    
- Java 方法默认支持重写（无 C++ 的 non-virtual 方法限制），除非方法被 `final` 修饰（禁止重写）。
    
- 子类对象可以向上转型（父类引用指向子类对象，自动完成），也可以向下转型（子类引用指向父类对象，需强制转换，类似 C++ 指针强制转换）。
    

示例（向下转型）：

```java
Person p = new Student("张三", 20, 90); // 向上转型（自动）
// 向下转型：需强制转换，否则编译报错
Student s = (Student)p;
s.show(); // 调用子类重写方法
```

## 4. final 关键字（Java 特有，对比 C++ const）

Java `final` 关键字功能比 C++ `const` 更丰富，可修饰类、方法、变量，核心用于“禁止修改”。

|final 修饰对象|功能说明|C++ 对应功能|
|---|---|---|
|类|禁止被继承（最终类），如 String 类是 final 类，不能有子类|无直接对应（C++ 无禁止继承的关键字）|
|方法|禁止被重写（子类不能重写该方法）|非虚函数（non-virtual 方法），子类无法重写|
|变量|常量（值不能修改），基本类型常量初始化后不可改，引用类型常量不能指向新对象|const 变量（基本类型）、const 指针（引用类型）|

示例（final 用法）：

```java
// 1. final 类：禁止继承
final class Person {}
// class Student extends Person {} // 错误：无法继承final类

// 2. final 方法：禁止重写
public class Person {
    public final void show() {}
}
public class Student extends Person {
    // @Override
    // public void show() {} // 错误：无法重写final方法
}

// 3. final 变量：常量
public class Demo {
    final int num = 10; // 基本类型常量，不可修改
    final Person p = new Person(); // 引用类型常量，不能指向新对象
    // num = 20; // 错误
    // p = new Person(); // 错误
}
```

## 5. 核心总结（C++ 视角）

1. Java 仅支持单继承，用接口替代多继承，避免二义性，子类构造必须通过 super() 调用父类构造。
    
2. 方法重写无需 virtual 关键字，推荐加 @Override 注解校验，且访问权限不能缩小。
    
3. 多态靠“父类引用指向子类对象”实现，无指针，更简洁，支持向上/向下转型。
    
4. final 关键字功能比 C++ const 丰富，可禁止类继承、方法重写、变量修改。





Java 笔记（共10篇，第四篇）

# Java 笔记 04：面向 C++ 开发者的 Java 接口与抽象类

## 1. 抽象类（abstract class）—— 对比 C++ 纯虚函数类

Java 抽象类与 C++ 包含纯虚函数的类（抽象基类）本质一致，核心作用是“定义规范、供子类继承实现”，语法和限制有细微差异，无本质功能区别。

### 1.1 语法对比（核心差异）

C++ 抽象基类（含纯虚函数，无法实例化）：

```cpp
// 抽象基类：含纯虚函数，不能创建对象
class Shape {
public:
    // 纯虚函数：只有声明，无实现，子类必须重写
    virtual void draw() = 0; 
    // 普通虚函数：可有实现，子类可重写
    virtual void show() {
        cout << "这是一个图形" << endl;
    }
};

// 子类必须重写纯虚函数，否则子类也是抽象类
class Circle : public Shape {
public:
    void draw() override {
        cout << "绘制圆形" << endl;
    }
};
```

Java 抽象类（用 abstract 关键字修饰，无法实例化）：

```java
// 抽象类：用abstract修饰，不能创建对象
public abstract class Shape {
    // 抽象方法：用abstract修饰，只有声明，无实现，子类必须重写
    public abstract void draw();
    
    // 普通方法：有实现，子类可重写、可直接继承
    public void show() {
        System.out.println("这是一个图形");
    }
}

// 子类必须重写抽象方法，否则子类需也声明为abstract
public class Circle extends Shape {
    @Override
    public void draw() {
        System.out.println("绘制圆形");
    }
}
```

### 1.2 核心差异与注意事项

- 关键字差异：Java 用 `abstract` 修饰类和抽象方法；C++ 无 abstract 关键字，通过“纯虚函数（=0）”标记抽象基类。
    
- 实例化限制：两者均无法直接实例化（C++ 抽象基类、Java 抽象类都不能 new 对象），只能通过子类实例化。
    
- 方法差异：Java 抽象方法必须无实现（无方法体），且必须用 abstract 修饰；C++ 纯虚函数无实现，普通虚函数可有实现（与 Java 抽象类的普通方法一致）。
    
- 继承限制：Java 抽象类的子类，若不重写所有抽象方法，必须将自身也声明为 abstract 类；C++ 子类若不重写纯虚函数，也是抽象类，无法实例化。
    

## 2. 接口（interface）—— Java 特有，替代 C++ 多继承

接口是 Java 特有概念，核心作用是“定义行为规范、实现多行为扩展”，弥补 Java 单继承的不足，类似 C++ 多继承的“行为复用”，但无多继承的二义性问题。

核心理解：接口是“纯抽象”的规范，只定义方法（无实现）、常量（默认 public static final），不包含普通方法、成员变量（除常量外）。

### 2.1 接口语法（对比 C++ 多继承）

C++ 多继承（实现多行为，易有二义性）：

```cpp
// 行为1
class Runable {
public:
    virtual void run() = 0;
};

// 行为2
class Flyable {
public:
    virtual void fly() = 0;
};

// 多继承：同时拥有两个行为，易有二义性（若两个父类有同名方法）
class Bird : public Runable, public Flyable {
public:
    void run() override { cout << "鸟会跑" << endl; }
    void fly() override { cout << "鸟会飞" << endl; }
};
```

Java 接口（实现多行为，无多继承副作用）：

```java
// 接口1：定义“可运行”行为（用interface修饰）
public interface Runable {
    // 接口方法：默认是 public abstract（可省略不写），无实现
    void run();
}

// 接口2：定义“可飞行”行为
public interface Flyable {
    void fly();
}

// 单继承 + 多实现：继承一个类，实现多个接口（替代C++多继承）
public class Bird extends Animal implements Runable, Flyable {
    // 必须实现所有接口的所有方法
    @Override
    public void run() {
        System.out.println("鸟会跑");
    }

    @Override
    public void fly() {
        System.out.println("鸟会飞");
    }
}
```

### 2.2 接口的核心特性（与 C++ 对比）

- 无实现：接口中的方法默认是 `public abstract`，必须无方法体（Java 8 后可加 default 方法、static 方法，有实现，后续详解）。
    
- 多实现：一个类可以实现多个接口（用 implements 关键字，逗号分隔），解决 Java 单继承的局限性，替代 C++ 多继承。
    
- 无成员变量：接口中只能有“常量”，默认是 `public static final`（可省略不写），不能有普通成员变量（C++ 抽象基类可有成员变量）。
    
- 无构造函数：接口不能有构造函数，无法实例化（类似 C++ 抽象基类），只能被类实现、被其他接口继承。
    
- 接口继承：接口可以继承多个接口（用 extends 关键字），但不能继承类；C++ 抽象基类只能单继承或多继承其他类。
    

示例（接口常量与接口继承）：

```java
// 接口1：含常量和方法
public interface A {
    // 常量：默认public static final，可省略
    int NUM = 10;
    void methodA();
}

// 接口2：继承接口A，同时新增方法
public interface B extends A {
    void methodB();
}

// 实现接口B，需实现A和B的所有方法
public class C implements B {
    @Override
    public void methodA() {
        System.out.println("实现methodA，使用常量：" + NUM);
    }

    @Override
    public void methodB() {
        System.out.println("实现methodB");
    }
}
```

## 3. 抽象类与接口的核心区别（对比 C++）

Java 抽象类和接口容易混淆，结合 C++ 知识对比，核心区别如下（重点记忆）：

|   |   |   |   |
|---|---|---|---|
|对比维度|Java 抽象类（abstract class）|Java 接口（interface）|C++ 对应概念|
|继承/实现|单继承（extends），一个类只能继承一个抽象类|多实现（implements），一个类可实现多个接口|抽象类=抽象基类；接口=多抽象基类组合|
|方法|可包含抽象方法、普通方法（有实现）|默认只有抽象方法（无实现）；Java8+ 可有 default、static 方法|抽象基类可含纯虚函数、普通虚函数|
|成员变量|可包含普通成员变量、常量|只能包含常量（public static final）|抽象基类可含任意成员变量|
|构造函数|有构造函数（供子类调用）|无构造函数，无法实例化|抽象基类有构造函数|
|核心作用|代码复用 + 规范定义（既有共性实现，又有抽象规范）|纯规范定义（仅定义行为，无任何实现，实现多行为扩展）|抽象基类=复用+规范；多继承=多行为扩展|

## 4. Java 8+ 接口新特性（可选，了解即可）

Java 8 及以后，接口新增 default 方法和 static 方法（有实现），打破“接口无实现”的传统，方便接口扩展（无需修改所有实现类），与 C++ 抽象基类的普通方法类似。

```java
public interface Runable {
    // 抽象方法（无实现）
    void run();

    // default方法：有实现，子类可重写、可直接继承
    default void show() {
        System.out.println("可运行的对象");
    }

    // static方法：有实现，只能通过接口名调用，子类无法重写
    static void info() {
        System.out.println("这是Runable接口");
    }
}

public class Dog implements Runable {
    @Override
    public void run() {
        System.out.println("狗会跑");
    }

    // 可选：重写default方法
    @Override
    public void show() {
        System.out.println("狗是可运行的动物");
    }
}

// 调用static方法
Runable.info(); // 输出：这是Runable接口
```

## 5. 核心总结（C++ 视角）

1. Java 抽象类 ≈ C++ 含纯虚函数的抽象基类，用 abstract 修饰，可含普通方法和成员变量，单继承，无法实例化。
    
2. Java 接口是特有概念，替代 C++ 多继承，纯规范定义（默认无实现），多实现，无成员变量（仅常量），无构造函数。
    
3. 抽象类侧重“代码复用+规范”，接口侧重“纯规范+多行为扩展”，二者可结合使用（类继承抽象类，同时实现多个接口）。
    
4. Java 8+ 接口可加 default、static 方法（有实现），类似 C++ 抽象基类的普通虚函数，方便接口扩展。









Java 笔记（共10篇，第五篇）

# Java 笔记 05：面向 C++ 开发者的 Java 封装与访问控制详解

## 1. 封装的核心思想（与 C++ 一致，语法差异）

封装的本质的是“隐藏实现细节、暴露安全接口”，Java 与 C++ 封装思想完全一致，但访问控制更严格，且无 C++ 的友元机制，依赖 getter/setter 方法访问私有成员。

核心逻辑：将类的成员变量私有化（private），禁止外部直接访问，通过公共的（public）getter（获取值）、setter（设置值）方法访问，在方法中添加校验逻辑，保证数据安全。

### 1.1 语法对比（C++ vs Java）

C++ 封装（支持友元，可直接访问私有成员）：

```cpp
class Person {
private:
    string name;
    int age; // 私有成员，外部无法直接访问
public:
    // 友元函数：可直接访问私有成员（Java 无此机制）
    friend void setAge(Person& p, int a);
    
    // getter/setter 方法（可选，友元可替代）
    string getName() { return name; }
    void setName(string n) { name = n; }
};

// 友元函数：直接访问私有成员age
void setAge(Person& p, int a) {
    if (a > 0 && a < 150) { // 数据校验
        p.age = a;
    }
}
```

Java 封装（无友元，必须通过 getter/setter 访问私有成员）：

```java
public class Person {
    // 私有成员：外部无法直接访问（与 C++ private 一致）
    private String name;
    private int age;

    // getter 方法：获取私有成员的值
    public String getName() {
        return name;
    }

    // setter 方法：设置私有成员的值，添加校验逻辑
    public void setName(String name) {
        // 校验：姓名不能为null或空字符串
        if (name != null && !name.trim().isEmpty()) {
            this.name = name;
        } else {
            System.out.println("姓名输入无效");
        }
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        // 校验：年龄必须在0-150之间
        if (age > 0 && age < 150) {
            this.age = age;
        } else {
            System.out.println("年龄输入无效");
        }
    }
}

// 外部访问：必须通过getter/setter，无法直接访问name、age
public class Test {
    public static void main(String[] args) {
        Person p = new Person();
        // p.name = "张三"; // 错误：private成员，外部无法直接访问
        p.setName("张三"); // 正确：通过setter设置
        p.setAge(20);     // 正确：通过setter设置
        System.out.println(p.getName() + "," + p.getAge()); // 正确：通过getter获取
    }
}
```

### 1.2 核心差异：友元机制的缺失与替代方案

- C++ 有友元（friend）：友元函数、友元类可直接访问类的私有成员，无需通过 getter/setter，适合特定场景（如运算符重载），但破坏了封装的严谨性。
    
- Java 无友元：为了保证封装的纯粹性，禁止任何外部类/方法直接访问私有成员，若需跨类访问私有成员，只能通过以下方式：
    
    - 提供 public 的 getter/setter 方法（推荐，最规范）；
        
    - 将访问类与当前类放在同一个包下，使用“包访问权限”（默认权限，不推荐，破坏封装）；
        
    - 使用反射（高级特性，后续笔记详解，不推荐日常使用）。
        

## 2. Java 访问控制修饰符详解（对比 C++）

Java 有4种访问控制修饰符（C++ 仅3种），分别控制类、成员变量、成员方法的访问范围，核心差异是“包访问权限”，这是 Java 特有（C++ 无包概念）。

先明确：Java 中的“包（package）”类似 C++ 的“命名空间（namespace）”，用于分类管理类，避免类名冲突，但访问控制与命名空间完全不同。

### 2.1 四种访问权限对比（从宽到窄）

|   |   |   |   |
|---|---|---|---|
|修饰符|访问范围|C++ 对应功能|使用场景|
|public|所有类可访问（跨包、同包）|public（完全一致）|类、getter/setter、对外暴露的方法|
|protected|同包类 + 子类（无论子类是否同包）|protected（差异：C++ 仅子类可访问，同包无关类不可）|子类需要继承的成员（变量/方法）|
|默认（无修饰符）|仅同包类可访问（子类不同包也不可）|无对应功能（C++ 无包访问权限）|同包内类之间共享的成员，不对外暴露|
|private|仅当前类可访问|private（完全一致）|类的私有成员变量、内部辅助方法|

### 2.2 关键注意事项（C++ 开发者易踩坑）

- 类的访问修饰符：只有 public 和默认两种（private/protected 不能修饰类），若类用 public 修饰，类名必须与文件名一致（Java 强制规则，C++ 无此要求）。
    
- protected 差异：Java 中，同包的无关类可以访问 protected 成员；C++ 中，只有子类能访问 protected 成员，同包无关类不能访问。
    
- 默认权限：无任何修饰符时，访问范围是“同包”，这是 Java 特有，C++ 无此概念，容易忽略导致访问权限错误。
    
- 访问修饰符的作用范围：仅作用于“成员（变量/方法）”和“类”，不作用于局部变量（局部变量无访问修饰符）。
    

示例（protected 权限差异）：

```java
// 包1：com.test1
package com.test1;
public class Person {
    protected String name; // protected成员
}

// 包1的无关类（同包）：可访问protected成员（Java特有）
package com.test1;
public class Test1 {
    public static void main(String[] args) {
        Person p = new Person();
        p.name = "张三"; // 正确：同包，可访问protected
    }
}

// 包2：com.test2（不同包）的子类：可访问protected成员
package com.test2;
import com.test1.Person;
public class Student extends Person {
    public void show() {
        System.out.println(name); // 正确：子类，可访问protected
    }
}

// 包2的无关类（不同包）：不可访问protected成员
package com.test2;
import com.test1.Person;
public class Test2 {
    public static void main(String[] args) {
        Person p = new Person();
        // p.name = "李四"; // 错误：不同包、非子类，不可访问
    }
}
```

## 3. 封装的进阶：Java Bean 规范（对比 C++ 结构体/类）

Java 中，封装的标准实现是“Java Bean 规范”，类似 C++ 中“只包含成员变量和 getter/setter 的结构体/类”，但有明确的规则要求，是日常开发中最常用的封装形式。

### 3.1 Java Bean 核心规范

1. 类必须是 public 修饰，且有公共的无参构造函数（默认或手动定义）。
    
2. 成员变量必须是 private 修饰。
    
3. 为每个私有成员变量提供 public 的 getter/setter 方法：
    
    1. getter 方法：命名规范为 get + 成员变量首字母大写（如 getName()、getAge()）；
        
    2. setter 方法：命名规范为 set + 成员变量首字母大写（如 setName()、setAge()）；
        
    3. 若成员变量是 boolean 类型，getter 方法可写为 is + 首字母大写（如 isMale()，替代 getMale()）。
        

### 3.2 示例（标准 Java Bean）

```java
// 标准 Java Bean：封装用户信息
public class User {
    // 私有成员变量
    private String username;
    private String password;
    private int age;
    private boolean isVip; // boolean类型

    // 公共无参构造（必须有）
    public User() {}

    // 公共有参构造（可选，方便创建对象）
    public User(String username, String password, int age, boolean isVip) {
        this.username = username;
        this.password = password;
        this.age = age;
        this.isVip = isVip;
    }

    // getter/setter 方法（遵循命名规范）
    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    // boolean类型：getter 可写为 isVip（推荐），也可写为 getIsVip（不推荐）
    public boolean isVip() {
        return isVip;
    }

    public void setVip(boolean vip) {
        isVip = vip;
    }
}
```

对比 C++ 类似实现（无严格规范）：

```cpp
// C++ 类似 Java Bean 的类（无严格规范）
class User {
private:
    string username;
    string password;
    int age;
    bool isVip;
public:
    User() {} // 无参构造
    User(string u, string p, int a, bool v) : username(u), password(p), age(a), isVip(v) {}

    // 无严格命名规范，可随意命名
    string getUname() { return username; }
    void setUname(string u) { username = u; }
    bool getVip() { return isVip; }
    void setVip(bool v) { isVip = v; }
};
```

## 4. 核心总结（C++ 视角）

1. Java 封装思想与 C++ 一致，核心是“私有成员 + 公共接口”，但无友元机制，必须通过 getter/setter 访问私有成员。
    
2. Java 多了“包访问权限”（默认权限），这是 C++ 没有的，需重点注意同包、跨包的访问规则。
    
3. protected 权限与 C++ 有差异：Java 中同包无关类可访问，C++ 中仅子类可访问。
    
4. Java Bean 是封装的标准实现，有严格的命名和结构规范，类似 C++ 中“纯数据封装类”，但规则更严谨，是日常开发的核心规范。








Java 笔记（共10篇，第六篇）

# Java 笔记 06：面向 C++ 开发者的 Java 字符串与常用工具类

## 1. Java String 类——对比 C++ string 类（核心差异）

Java 中的 String 类用于表示字符串，与 C++ string 类功能相似（均用于字符串操作），但底层实现、不可变性、常用方法有显著差异，是 C++ 开发者易踩坑的点。

核心差异：Java String 是**不可变字符串**（一旦创建，内容无法修改），C++ string 是可变字符串（可直接修改内容）。

### 1.1 字符串创建与底层差异

C++ string （可变，底层是字符数组，可直接修改）：

```cpp
#include <string>
using namespace std;

int main() {
    string str1 = "hello"; // 直接创建，可变
    string str2("world");  // 构造函数创建
    str1[0] = 'H';         // 直接修改字符（允许）
    str1 += " java";       // 拼接字符串，直接修改原对象
    cout << str1; // 输出：Hello java
    return 0;
}
```

Java String （不可变，底层是 char 数组，被 final 修饰，无法修改）：

```java
public class Test {
    public static void main(String[] args) {
        // 两种创建方式（本质不同，后续详解）
        String str1 = "hello"; // 常量池创建
        String str2 = new String("world"); // 堆内存创建
        
        // str1[0] = 'H'; // 错误：无法直接修改字符（不可变）
        str1 = str1 + " java"; // 拼接字符串，并非修改原对象，而是创建新对象
        System.out.println(str1); // 输出：hello java
    }
}
```

### 1.2 核心差异：不可变性的影响

- C++ string：可变，修改操作（如 [] 赋值、+=、insert）直接修改原字符串对象，效率高（无需创建新对象）。
    
- Java String：不可变，任何修改操作（拼接、替换、截取）都不会修改原对象，而是创建一个新的 String 对象，原对象不变（若无人引用，会被 GC 回收）。
    
- 补充：Java 为了优化字符串操作，提供了 StringBuffer（线程安全）、StringBuilder（非线程安全，效率高）类，用于可变字符串操作（类似 C++ string）。
    

示例（Java 可变字符串：StringBuilder）：

```java
public class Test {
    public static void main(String[] args) {
        // 类似 C++ string，可变字符串
        StringBuilder sb = new StringBuilder("hello");
        sb.setCharAt(0, 'H'); // 直接修改字符（允许）
        sb.append(" java");    // 拼接字符串，修改原对象
        System.out.println(sb); // 输出：Hello java
    }
}
```

### 1.3 String 两种创建方式对比（Java 特有）

Java String 有两种创建方式，这是 C++ 没有的（C++ string 只有一种创建逻辑），核心差异在于“是否使用常量池”。

```java
public class Test {
    public static void main(String[] args) {
        // 方式1：常量池创建（推荐）
        String str1 = "hello";
        String str2 = "hello";
        System.out.println(str1 == str2); // true（指向同一个常量池对象）
        
        // 方式2：堆内存创建（不推荐，浪费内存）
        String str3 = new String("hello");
        String str4 = new String("hello");
        System.out.println(str3 == str4); // false（两个不同的堆对象）
        
        // 注意：== 比较地址（C++ 中 == 比较字符串内容，Java 需用 equals()）
        System.out.println(str1.equals(str3)); // true（比较字符串内容）
    }
}
```

对比 C++ string 比较：

```cpp
string str1 = "hello";
string str2 = "hello";
string str3 = new string("hello"); // C++ 也可new，但极少用
cout << (str1 == str2); // true（比较内容，C++ string 重载了 ==）
```

### 1.4 常用方法对比（C++ vs Java）

|  a | b  |  c |
| -------- | --------- | --- |
|功能需求 | C++ string 方法 | Java String 方法|
|获取长度|str.size() / str.length()|str.length()|
|拼接字符串|str1 + str2 / str.append(str2)|str1 + str2 / str1.concat(str2)（均创建新对象）|
|比较内容|str1 == str2 / str1.compareTo(str2)|str1.equals(str2)（区分大小写）/ str1.equalsIgnoreCase(str2)（不区分）|
|截取字符串|str.substr(起始索引, 长度)|str.substring(起始索引) / str.substring(起始索引, 结束索引)（左闭右开）|
|查找字符/子串|str.find(目标)（返回索引，无则返回string::npos）|str.indexOf(目标)（返回索引，无则返回-1）|
|替换字符/子串|str.replace(起始索引, 长度, 替换内容)|str.replace(目标, 替换内容)（创建新对象）|

## 2. Java 常用工具类——对比 C++ 标准库

Java 提供了大量内置工具类，用于简化开发，类似 C++ 标准库（如 `<string>`、`<algorithm>`），但用法更简洁，无需手动引入（除了部分包）。

重点讲解 3 个最常用的工具类，均为 static 类（无需创建对象，直接通过类名调用方法）。

### 2.1 Arrays 工具类（数组操作，对比 C++ `<algorithm>`）

Java Arrays 类用于操作数组（排序、查找、填充等），类似 C++ 标准库中的 sort()、find() 等函数，用法更简洁。

```java
import java.util.Arrays; // 需导入包（C++ 无需导入 <algorithm>）

public class Test {
    public static void main(String[] args) {
        int[] arr = {3, 1, 4, 1, 5, 9};
        
        // 1. 排序（类似 C++ sort(arr.begin(), arr.end())）
        Arrays.sort(arr);
        System.out.println(Arrays.toString(arr)); // 输出：[1, 1, 3, 4, 5, 9]
        
        // 2. 二分查找（类似 C++ binary_search()，需先排序）
        int index = Arrays.binarySearch(arr, 4);
        System.out.println(index); // 输出：3（找到，返回索引）
        
        // 3. 数组填充（类似 C++ fill()）
        Arrays.fill(arr, 0);
        System.out.println(Arrays.toString(arr)); // 输出：[0, 0, 0, 0, 0, 0]
        
        // 4. 数组复制（类似 C++ copy()）
        int[] arr2 = Arrays.copyOf(arr, 3); // 复制前3个元素
        System.out.println(Arrays.toString(arr2)); // 输出：[0, 0, 0]
    }
}
```

对比 C++ 数组操作：

```cpp
#include <algorithm>
#include <iostream>
using namespace std;

int main() {
    int arr[] = {3, 1, 4, 1, 5, 9};
    int n = sizeof(arr)/sizeof(arr[0]);
    
    sort(arr, arr + n); // 排序
    for (int i = 0; i < n; i++) cout << arr[i] << " "; // 输出：1 1 3 4 5 9
    
    // 二分查找
    bool exists = binary_search(arr, arr + n, 4);
    cout << exists; // 输出：1（true）
    
    // 填充
    fill(arr, arr + n, 0);
    return 0;
}
```

### 2.2 Math 工具类（数学运算，对比 C++ `<cmath>`）

Java Math 类提供常用数学运算（绝对值、平方根、三角函数等），类似 C++ `<cmath>` 库，所有方法均为 static，无需创建对象。

```java
public class Test {
    public static void main(String[] args) {
        // 1. 绝对值（类似 C++ abs()）
        System.out.println(Math.abs(-5)); // 输出：5
        
        // 2. 平方根（类似 C++ sqrt()）
        System.out.println(Math.sqrt(16)); // 输出：4.0
        
        // 3. 取整（类似 C++ floor()、ceil()）
        System.out.println(Math.floor(3.7)); // 输出：3.0（向下取整）
        System.out.println(Math.ceil(3.2));  // 输出：4.0（向上取整）
        
        // 4. 随机数（类似 C++ rand()，但更简洁）
        // 生成 [0.0, 1.0) 之间的随机数
        double random = Math.random();
        // 生成 [1, 100] 之间的随机整数
        int randomInt = (int)(Math.random() * 100 + 1);
        System.out.println(randomInt);
    }
}
```

### 2.3 Objects 工具类（对象操作，C++ 无对应类）

Java Objects 类是 JDK 1.7 新增，用于简化对象的常见操作（非空判断、对象比较等），C++ 无对应工具类，需手动实现。

```java
import java.util.Objects;

public class Test {
    public static void main(String[] args) {
        String str = null;
        String str2 = "hello";
        
        // 1. 非空判断（避免空指针异常，类似 C++ if (str != NULL)）
        // 若 str 为 null，抛出异常；否则返回 str
        String s1 = Objects.requireNonNull(str, "字符串不能为null");
        
        // 2. 安全的对象比较（避免空指针，类似 C++ 手动判断 null）
        // 若两个对象都为null，返回true；一个为null，返回false；否则调用 equals()
        boolean equal = Objects.equals(str, str2);
        System.out.println(equal); // 输出：false
        
        // 3. 获取对象哈希值（类似 C++ hash()）
        System.out.println(Objects.hashCode(str2));
    }
}
```

## 3. 核心总结（C++ 视角）

1. Java String 是不可变字符串，修改操作会创建新对象，C++ string 是可变字符串，直接修改原对象；Java 可用 StringBuilder/StringBuffer 实现可变字符串。
    
2. Java String 有两种创建方式（常量池、堆内存），== 比较地址，equals() 比较内容；C++ string == 直接比较内容。
    
3. Java 常用工具类（Arrays、Math、Objects）类似 C++ 标准库，用法更简洁，均为 static 类，无需创建对象。
    
4. Arrays 类简化数组操作，Math 类处理数学运算，Objects 类简化对象非空判断、比较，C++ 需手动实现部分功能。




Java 笔记（共10篇，第七篇）

# Java 笔记 07：面向 C++ 开发者的 Java 异常处理机制

## 1. 异常机制核心认知（对比 C++ 异常）

Java 异常机制与 C++ 异常本质一致，都是“程序运行时出现的错误（非编译错误），用于捕获错误、避免程序崩溃，提高程序健壮性”，但语法、异常分类、处理规则有显著差异。

核心差异：Java 异常是**面向对象**的（所有异常都是类的对象），有严格的异常分类和处理规范；C++ 异常可以是任意类型（int、string、自定义类等），规范相对松散。

### 1.1 异常的本质与分类（Java 特有）

Java 中所有异常都继承自 `Throwable` 类，分为两大派系（C++ 无此严格分类）：

1. **Error（错误）**：JVM 级别的错误，程序无法处理，如内存溢出（OutOfMemoryError）、栈溢出（StackOverflowError），无需捕获，也无法捕获，只能通过优化程序避免。
    
2. **Exception（异常）**：程序运行时可预测、可处理的错误，分为两类：
    
    1. 编译时异常（受检异常，Checked Exception）：编译时必须处理（捕获或声明抛出），否则编译报错，如 IOException、SQLException（C++ 无此分类）。
        
    2. 运行时异常（非受检异常，Unchecked Exception）：编译时无需处理，运行时才可能出现，如空指针异常（NullPointerException）、数组越界异常（ArrayIndexOutOfBoundsException），类似 C++ 中常见的异常。
        

对比 C++ 异常：C++ 异常无严格分类，可抛出任意类型（int、string、自定义类），编译时无需声明异常，也无“受检/非受检”的区别。

### 1.2 异常处理关键字对比（C++ vs Java）

|   |   |   |   |
|---|---|---|---|
|功能|Java 关键字|C++ 关键字|差异说明|
|抛出异常|throw|throw|用法一致，但 Java 只能抛出 Throwable 子类对象|
|声明异常|throws（方法上声明）|无|Java 受检异常必须声明，C++ 无需声明|
|捕获异常|try-catch-finally|try-catch-throw|Java 有 finally（无论是否异常都执行），C++ 无 finally，可用 catch(...) 兜底|
|兜底捕获|catch (Exception e)（捕获所有异常）|catch (...)（捕获所有异常）|用法类似，均用于捕获未明确处理的异常|

## 2. 异常处理语法对比（核心差异）

### 2.1 C++ 异常处理（无受检异常，无 finally）

```cpp
#include <iostream>
using namespace std;

// C++ 无需声明异常，可直接抛出任意类型
void divide(int a, int b) {
    if (b == 0) {
        // 抛出 int 类型异常（C++ 可抛出任意类型）
        throw 1001; 
    }
    cout << a / b << endl;
}

int main() {
    try {
        divide(10, 0);
    } catch (int e) { // 捕获 int 类型异常
        cout << "异常：除数为0，错误码：" << e << endl;
    } catch (...) { // 兜底捕获，捕获所有未处理的异常
        cout << "未知异常" << endl;
    }
    // C++ 无 finally，需手动释放资源
    return 0;
}
```

### 2.2 Java 异常处理（受检/非受检异常，finally）

分两种场景：运行时异常（无需声明）、编译时异常（必须声明或捕获）。

```java
public class ExceptionTest {
    // 1. 运行时异常（非受检）：无需声明 throws
    public static void divide(int a, int b) {
        if (b == 0) {
            // 抛出运行时异常（NullPointerException、ArrayIndexOutOfBoundsException 均属于此类）
            throw new ArithmeticException("除数不能为0");
        }
        System.out.println(a / b);
    }

    // 2. 编译时异常（受检）：必须声明 throws，否则编译报错
    public static void readFile() throws IOException {
        // 模拟读取文件，IOException 是编译时异常
        FileInputStream fis = new FileInputStream("test.txt");
        fis.close();
    }

    public static void main(String[] args) {
        // 处理运行时异常（可选，不处理会导致程序崩溃）
        try {
            divide(10, 0);
        } catch (ArithmeticException e) {
            // 打印异常信息（类似 C++ 捕获异常后的处理）
            System.out.println("异常信息：" + e.getMessage());
            e.printStackTrace(); // 打印异常堆栈（Java 特有，便于调试）
        }

        // 处理编译时异常（必须处理：要么 catch，要么 throws）
        try {
            readFile();
        } catch (IOException e) {
            System.out.println("文件读取异常：" + e.getMessage());
        } finally {
            // 无论是否发生异常，都会执行（用于释放资源，类似 C++ 手动释放）
            System.out.println("异常处理结束，释放资源");
        }
    }
}
```

### 2.3 核心差异总结

- 异常类型：Java 仅能抛出 Throwable 子类对象（面向对象），C++ 可抛出任意类型（int、string 等）。
    
- 受检异常：Java 编译时异常必须处理（声明或捕获），C++ 无此要求，编译时不检查异常。
    
- finally 关键字：Java 有 finally，用于释放资源（无论是否异常都执行）；C++ 无 finally，需在 catch 中手动释放资源，或用 RAII 机制（如智能指针）。
    
- 异常堆栈：Java 可通过 `printStackTrace()` 打印异常堆栈，便于调试；C++ 需手动输出异常信息。
    

## 3. 异常处理的注意事项（C++ 开发者易踩坑）

### 3.1 异常捕获顺序（Java 与 C++ 一致）

捕获异常时，必须“先捕获子类异常，再捕获父类异常”，否则子类异常会被父类异常捕获，导致子类异常无法被针对性处理。

```java
// 错误示例：先捕获父类 Exception，再捕获子类 ArithmeticException
try {
    divide(10, 0);
} catch (Exception e) { // 父类异常，会捕获所有 Exception 子类
    System.out.println("未知异常");
} catch (ArithmeticException e) { // 无法执行，被父类捕获
    System.out.println("除数为0");
}

// 正确示例：先子类，后父类
try {
    divide(10, 0);
} catch (ArithmeticException e) {
    System.out.println("除数为0");
} catch (Exception e) {
    System.out.println("未知异常");
}
```

C++ 也有相同规则，若先捕获父类异常，子类异常会被兜底捕获。

### 3.2 编译时异常的强制处理（Java 特有）

Java 中，编译时异常（如 IOException、SQLException）必须处理，有两种处理方式：

1. 用 try-catch 捕获，直接处理异常；
    
2. 在方法上用 throws 声明，将异常抛给调用者处理（最终必须有一处捕获，否则程序无法编译）。
    

C++ 无此强制要求，即使方法抛出异常，也无需声明，编译时不会报错。

### 3.3 finally 与 return 的执行顺序（Java 特有）

Java 中，finally 块会在 return 之前执行（无论 try 或 catch 中是否有 return），这是 C++ 没有的特性，需重点注意。

```java
public static int testFinally() {
    try {
        return 1; // 不会直接返回，先执行 finally
    } catch (Exception e) {
        return 2;
    } finally {
        System.out.println("finally 执行"); // 先执行
        // finally 中不能有 return（会覆盖 try/catch 中的 return，不推荐）
    }
}

public static void main(String[] args) {
    System.out.println(testFinally()); // 输出：finally 执行 → 1
}
```

### 3.4 自定义异常（对比 C++）

Java 和 C++ 都支持自定义异常，但 Java 必须继承 Throwable 类（或其子类），C++ 可自定义任意类作为异常。

```java
// Java 自定义异常（继承 Exception → 编译时异常；继承 RuntimeException → 运行时异常）
public class MyException extends Exception {
    // 构造函数（类似 C++ 自定义异常的构造）
    public MyException() {}
    public MyException(String message) {
        super(message); // 调用父类构造，传递异常信息
    }
}

// 使用自定义异常
public class Test {
    public static void checkAge(int age) throws MyException {
        if (age < 0 || age > 150) {
            throw new MyException("年龄无效：" + age);
        }
    }

    public static void main(String[] args) {
        try {
            checkAge(200);
        } catch (MyException e) {
            System.out.println(e.getMessage()); // 输出：年龄无效：200
        }
    }
}
```

C++ 自定义异常：

```cpp
#include <iostream>
#include <string>
using namespace std;

// C++ 可自定义任意类作为异常
class MyException {
private:
    string message;
public:
    MyException(string msg) : message(msg) {}
    string getMessage() { return message; }
};

void checkAge(int age) {
    if (age < 0 || age > 150) {
        throw MyException("年龄无效");
    }
}

int main() {
    try {
        checkAge(200);
    } catch (MyException e) {
        cout << e.getMessage() << endl;
    }
    return 0;
}
```

## 4. 核心总结（C++ 视角）

1. Java 异常是面向对象的（继承 Throwable），分 Error 和 Exception，Exception 又分编译时（受检）和运行时（非受检）异常；C++ 异常无严格分类，可抛出任意类型。
    
2. Java 编译时异常必须处理（catch 或 throws），C++ 无需声明异常，编译时不检查。
    
3. Java 有 finally 块（释放资源，必执行），C++ 无此关键字，需手动释放资源或用 RAII 机制。
    
4. 自定义异常：Java 必须继承 Throwable 子类，C++ 可自定义任意类；捕获异常时，两者都需遵循“先子类后父类”的顺序。
    
5. Java 异常调试更便捷（printStackTrace() 打印堆栈），C++ 需手动输出异常信息。







````

# Python



````markdown title:"Python"
































````

# 分布式


````markdown title:"分布式"

````












