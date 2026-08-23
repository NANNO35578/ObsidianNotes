# C++中左值和右值
在 C++ 中，左值（lvalue）和右值（rvalue）是两个重要的概念，理解它们有助于更好地掌握 C++ 的内存管理和对象生命周期。

### 左值（lvalue）
- **定义**：左值是指可以在赋值语句的左侧的表达式，表示一个持久的对象。
- **特征**：
    - 具有持久的内存地址。
    - 可以被修改。
- **例子**：
```cpp
    int x = 10;  // x 是左值
    x = 20;      // 可以被赋值
```

### 右值（rvalue）
- **定义**：右值是指不能在赋值语句的左侧的表达式，表示一个临时的值。
- **特征**：
    - 没有持久的内存地址，通常是临时对象。
    - 通常用于表达式的计算结果。
- **例子**：
    ```cpp
    int getValue() { return 42; }
    int y = getValue();  // getValue() 返回一个右值
    ```

### C++11 中的右值引用
C++11 引入了右值引用（rvalue reference），通过 `&&` 符号实现。右值引用允许开发者以更高效的方式转移资源，特别是在实现移动语义时。
- **例子**：
    ```cpp
    void process(int &&x) {
        // x 是一个右值引用
    }
    process(10);  // 10 是右值
    ```

---
# 运算符重载`=`

- 一般所说的“赋值操作”。它和拷贝构造函数类似，不过功能有差异。
##### 定义和语法
- 赋值运算符重载用于自定义对象之间的赋值行为，允许你控制如何将一个对象的值赋给另一个对象。语法格式为：
```cpp
class ClassName {
public:
    ClassName& operator=(const ClassName& other) {
        if (this != &other) {
            // 执行赋值操作
        }
        return *this;
    }
};
```
##### 调用场景
- 当一个已存在的对象被赋值为另一个同类型对象时，会调用赋值运算符重载函数。
```cpp
ClassName obj1, obj2;
obj1 = obj2; // 调用赋值运算符重载函数
```
##### 默认赋值运算符
如果没有为类定义赋值运算符重载函数，编译器会自动生成一个默认的赋值运算符。它执行的是浅赋值，即简单地将一个对象的成员变量的值复制给另一个对象。
##### 浅赋值与深赋值
- *浅赋值*：只复制对象的成员变量值，对于指针成员，只复制指针的值而不复制指针指向的内容，可能导致多个对象共享同一块内存，后续可能引发悬空指针或内存泄漏问题。
- *深赋值*：不仅复制对象的成员变量值，还会为指针成员分配新的内存空间，并复制指针指向的内容，避免浅赋值带来的问题。
```cpp

class MyClass {
    int* data;
public:
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            delete[] data; // 释放原有内存
            data = new int[/* 合适的大小 */];
            // 复制数据
        }
        return *this;
    }
};
```
 
##### 与拷贝构造函数区别
- 拷贝构造函数用于在创建新对象时，用一个已存在的对象初始化它；
- 赋值运算符重载用于对已存在的对象进行赋值操作。

---
# Qt的`connect`函数以及连接方式
在 Qt 中，`connect` 函数用于连接信号和槽，是 Qt 信号与槽机制的核心。信号是对象状态变化的通知，而槽是响应信号的函数。

### `connect` 函数的基本定义

```cpp
QObject::connect(sender, signal, receiver, slot);
```

- **sender**：发出信号的对象。
- **signal**：要连接的信号。
- **receiver**：接收信号的对象。
- **slot**：响应信号的槽函数。

### 五种连接方式

1. **默认连接（AutoConnection）**
    - 根据对象所在的线程自动选择连接方式。
    - 如果发送信号的对象和接收信号的对象在同一线程，使用直接连接；否则，使用队列连接。
`QObject::connect(sender, SIGNAL(signalName()), receiver, SLOT(slotName()));`
2. **直接连接（DirectConnection）**
    - 信号发出时，立即调用槽函数。
    - 通常用于同一线程中的连接。
`QObject::connect(sender, SIGNAL(signalName()), receiver, SLOT(slotName()), Qt::DirectConnection);
3. **队列连接（QueuedConnection）**
    - 信号发出时，不立即调用槽，而是将槽放入接收对象的事件队列中。
    - 常用于不同线程之间的连接。
`QObject::connect(sender, SIGNAL(signalName()), receiver, SLOT(slotName()), Qt::QueuedConnection);
4. **强连接（UniqueConnection）**
    - 确保信号和槽之间只存在一个连接。如果已有连接，则不会建立新的连接。
    - 避免重复连接。
`QObject::connect(sender, SIGNAL(signalName()), receiver, SLOT(slotName()), Qt::UniqueConnection);
5. **自定义连接（CustomConnection）**
    - 通过使用 `Qt::ConnectionType` 枚举可以自定义连接方式，结合上述方式来满足特定需求。

---

#多态
# 多态主要分为哪两类
C++中的多态是面向对象编程的重要特性之一，它*允许程序在运行时根据对象的实际类型来决定调用哪个函数*。这种机制主要通过虚函数实现。多态的主要形式有两种：编译时多态（静态多态）和运行时多态（动态多态）。

### 1. 编译时多态
编译时能够确定调用哪个函数，编译时多态通常通过函数重载和运算符重载实现。
速度快，效率高，缺乏灵活性
- **函数重载**：同一个函数名可以有不同的参数列表。
```cpp
    void print(int i) {
        std::cout << "整数: " << i << std::endl;
    }
    
    void print(double d) {
        std::cout << "浮点数: " << d << std::endl;
    }
```
- **运算符重载**：允许用户定义自己的运算符行为。
```cpp
    class Complex {
    public:
        double real, imag;
        Complex operator+(const Complex &c) {
            return Complex{real + c.real, imag + c.imag};
        }
    };
```
- 模板

### 2. 运行时多态
编译时不能确定调用的哪个函数，而在程序运行过程中才动态的确定具体操作对象，运行时多态通过基类指针或引用来调用派生类的重写函数，主要使用虚函数。
- **虚函数**：在基类中声明为`virtual`，在派生类中重写。
    ```cpp
    class Base {
    public:
        virtual void show() {
            std::cout << "Base class" << std::endl;
        }
        virtual ~Base() {}  // 虚析构函数
    };
    
    class Derived : public Base {
    public:
        void show() override {
            std::cout << "Derived class" << std::endl;
        }
    };
    ```
- **使用示例**：
    ```cpp
    void display(Base *b) {
        b->show();
    }
    
    int main() {
        Base *b = new Derived();
        display(b);  // 输出: Derived class
        delete b;
        return 0;
    }
    ```

### 3. 多态的优点
- **灵活性**：可以在运行时决定调用哪个函数，提升代码的扩展性和灵活性。
- **代码重用**：通过基类和派生类的关系，可以重用代码，减少冗余。
- **接口一致性**：通过统一的接口，增加了代码的可读性和可维护性。
### 4. 注意事项
- **虚函数表**：每个类有一个虚函数表，存放虚函数的指针。在对象创建时，会生成指向该表的指针。
- **性能开销**：多态会引入一定的性能开销，因为需要通过虚函数表进行查找。
- **对象切片**：当基类对象被赋值为派生类对象时，可能会丢失派生类特有的数据和行为。
### 总结

C++中的多态是实现灵活和可扩展代码的重要机制。通过理解静态和动态多态的区别，以及掌握虚函数的使用，可以有效地利用多态性来设计更好的程序。


---
# 什么是虚函数？它是如何工作的？
> 虚函数的[工作机制](https://zhida.zhihu.com/search?content_id=235377780&content_type=Article&match_order=1&q=%E5%B7%A5%E4%BD%9C%E6%9C%BA%E5%88%B6&zhida_source=entity)是通过虚函数表（vtable）实现的。每一个含有虚函数的类都有一个[虚函数表](https://zhida.zhihu.com/search?content_id=235377780&content_type=Article&match_order=2&q=%E8%99%9A%E5%87%BD%E6%95%B0%E8%A1%A8&zhida_source=entity)，这个表是一个存储[函数指针](https://zhida.zhihu.com/search?content_id=235377780&content_type=Article&match_order=1&q=%E5%87%BD%E6%95%B0%E6%8C%87%E9%92%88&zhida_source=entity)的数组。当我们调用一个虚函数时，程序会查找对象的虚函数表，找到相应的函数指针，并执行函数。

虚函数是C++中用于实现运行时多态的机制。它允许在基类中声明一个函数为虚函数，并在派生类中重写该函数。当通过基类指针或引用调用该函数时，程序会根据对象的实际类型来决定调用哪个版本的函数。
- **虚函数表（Vtable）**：  
    编译器为每个包含虚函数的类创建一个虚函数表（Vtable），表中存放指向该类虚函数的指针。每个对象会有一个指向其类的Vtable的指针（Vptr）。
    
- **动态绑定**：  
    当通过基类指针或引用调用虚函数时：
    
    - 程序查找对象的Vptr，找到对应的Vtable。
    - 根据Vtable中的指针调用实际的函数实现。

---
# 多态如何与继承关系工作？
> 多态和继承紧密相关。通过继承，派生类继承了基类的属性和方法。当我们在派生类中重写基类的虚函数时，就实现了多态。这允许我们使用基类的引用或指针来调用派生类的方法。
- **基类引用或指针**：我们可以使用基类的引用或指针来存储派生类的对象。当通过这个引用或指针调用一个虚函数时，将会执行对象实际类型对应的方法。 
- **[动态绑定](https://zhida.zhihu.com/search?content_id=235377780&content_type=Article&match_order=1&q=%E5%8A%A8%E6%80%81%E7%BB%91%E5%AE%9A&zhida_source=entity)**：多态的实现依赖于动态绑定。即在运行时根据对象的实际类型来决定调用哪个版本的虚函数。

### 1. 继承的基本概念
- **基类和派生类**：在C++中，继承允许一个类（派生类）从另一个类（基类）继承属性和方法。派生类可以重写基类的方法。
### 2. 虚函数与重写
- **虚函数**：在基类中声明为`virtual`的函数可以在派生类中重写。这样，调用该函数时，会根据对象的实际类型（而不是指针或引用的类型）来决定执行哪个版本的函数。这就是多态

---
# 覆盖（override）和隐藏（hide）有什么区别？
- **覆盖（Override）**：当派生类重写基类的虚函数时，这称为覆盖。派生类的函数必须与基类的虚函数有相同的签名。覆盖发生在基类和派生类之间的虚函数上。  
- **隐藏（Hide）**：如果派生类声明了一个与基类同名的函数，无论[参数列表](https://zhida.zhihu.com/search?content_id=235377780&content_type=Article&match_order=1&q=%E5%8F%82%E6%95%B0%E5%88%97%E8%A1%A8&zhida_source=entity)是否相同，都会隐藏基类中所有同名的函数。这称为隐藏。即使基类中的函数是虚函数，隐藏也会发生。
##### 关键区别
- **动态与静态绑定**：   
    - 覆盖使用动态绑定，允许在运行时根据实际对象类型决定调用哪个函数。
    - 隐藏使用静态绑定，调用取决于指针或引用的类型。
- **影响范围**：
    - 覆盖只影响同名的虚函数。
    - 隐藏会使所有同名的函数在派生类中不可见。

---
# 如何避免在派生类中隐藏基类的同名函数
##### 1. 使用 `using` 声明
- **`using` 声明**：在派生类中使用`using`关键字，可以将基类的某个函数引入到派生类中。这样，基类的同名函数就不会被隐藏。
```cpp
    class Base {
    public:
        void display(int) {
            std::cout << "Base display with int" << std::endl;
        }
    };
    
    class Derived : public Base {
    public:
        using Base::display;  // 引入Base中的display
    
        void display(double) {
            std::cout << "Derived display with double" << std::endl;
        }
    };
```
##### 2. 确保函数签名相同
- **相同签名**：在派生类中定义的函数如果与基类中的函数有完全相同的签名（包括参数类型和数量），则会覆盖基类的函数，而不是隐藏它。
    ```cpp
    class Derived : public Base {
    public:
        void display(int) override {  // 与基类的display具有相同签名
            std::cout << "Derived display with int" << std::endl;
        }
    };
    ```
---
# 什么是[对象切片](https://zhida.zhihu.com/search?content_id=235377780&content_type=Article&match_order=1&q=%E5%AF%B9%E8%B1%A1%E5%88%87%E7%89%87&zhida_source=entity)（Object Slicing）？如何避免？
> 对象切片是一种现象，在将派生类对象赋值给基类对象时，派生类特有的数据和行为会被截断（或“切片”），只保留基类部分。这会导致派生类的特性丢失。
- 使用基类的指针或引用来存储派生类的对象。
- 避免将派生类对象赋值给基类对象。
### 如何避免对象切片？
1. **使用基类指针或引用**：  
    通过基类的指针或引用来存储派生类对象，避免直接赋值。 
    ```cpp
    Base* b = new Derived();
    b->show();  // 正确调用Derived的show()
    delete b;   // 释放内存
    ```
2. **使用智能指针**：  
    使用智能指针（如`std::unique_ptr`或`std::shared_ptr`）来管理对象，可以避免内存泄漏，并保持对象的完整性。
    ```cpp
    std::unique_ptr<Base> b = std::make_unique<Derived>();
    b->show();  // 正确调用Derived的show()
    ```
3. **避免值传递**：  
    在函数参数中使用基类指针或引用，而非对象本身，避免切片的发生。
    ```cpp
    void process(Base* b) {
        b->show();
    }
    ```
---
# 什么是虚析构函数？为什么需要虚析构函数？

> 虚析构函数是一个在基类中声明为虚的[析构函数](https://zhida.zhihu.com/search?content_id=235377780&content_type=Article&match_order=5&q=%E6%9E%90%E6%9E%84%E5%87%BD%E6%95%B0&zhida_source=entity)。当我们使用基类的指针或引用来删除派生类的对象时，虚析构函数确保调用正确的析构函数，从而正确地释放资源。
> 
> 如果不使用虚析构函数，当通过基类指针删除派生类对象时，只会调用基类的析构函数，不会调用派生类的析构函数，这可能导致派生类中分配的资源泄露
> 
> ```cpp
> class Base {
> public:
>     virtual ~Base() { cout << "Base Destructor" << endl; }
> };
> 
> class Derived : public Base {
> public:
>     ~Derived() { cout << "Derived Destructor" << endl; }
> };
> 
> int main() {
 >    Base* obj = new Derived();
>     delete obj;  // 输出: Derived Destructor 和 Base Destructor
>     return 0;
> }
> ```
> 在这个例子中，`Derived`类继承自`Base`类，两者都有析构函数。由于`Base`类的析构函数是虚的，当我们通过`Base`类的指针删除`Derived`类的对象时，会先调用`Derived`类的析构函数，然后调用`Base`类的析构函数。

---
# 如何在C++中重载运算符？
在C++中，重载运算符是通过定义一个特殊的成员函数或全局函数来实现的。以下是重载运算符的基本步骤和示例。
### 1. 成员函数重载
运算符可以在类内部作为成员函数重载。成员函数的第一个参数是隐含的`this`指针，表示对象的实例。
##### 示例：重载 `+` 运算符
```cpp
#include <iostream>

class Complex {
public:
    double real;
    double imag;

    Complex(double r, double i) : real(r), imag(i) {}

    // 重载 + 运算符
    Complex operator+(const Complex &c) {
        return Complex(real + c.real, imag + c.imag);
    }

    void display() const {
        std::cout << real << " + " << imag << "i" << std::endl;
    }
};

int main() {
    Complex c1(1.0, 2.0);
    Complex c2(3.0, 4.0);
    Complex c3 = c1 + c2;  // 使用重载的 + 运算符
    c3.display();           // 输出: 4.0 + 6.0i
    return 0;
}
```
##### 2. 全局函数重载
某些运算符（如`<<`和`>>`）通常以全局函数的形式重载，因为它们需要访问类的私有成员。
#### 例：重载 `<<` 运算符
```cpp
#include <iostream>

class Complex {
public:
    double real;
    double imag;

    Complex(double r, double i) : real(r), imag(i) {}

    // 友元函数重载 << 运算符
    friend std::ostream& operator<<(std::ostream &out, const Complex &c) {
        out << c.real << " + " << c.imag << "i";
        return out;
    }
};

int main() {
    Complex c(1.0, 2.0);
    std::cout << c << std::endl;  // 输出: 1.0 + 2.0i
    return 0;
}
```
##### 3. 重载运算符的注意事项
- **返回类型**：重载的运算符通常返回一个对象，以便能够进行链式操作。
- **常量性**：如果重载函数不修改对象中的数据，应该声明为`const`。
- **友元函数**：对于需要访问私有成员的全局函数重载，可以将其声明为友元。
##### 4. 支持的运算符
以下是可以重载的运算符的一些示例：
- 算术运算符：`+`, `-`, `*`, `/`, `%`
- 关系运算符：`==`, `!=`, `<`, `>`, `<=`, `>=`
- 逻辑运算符：`&&`, `||`, `!`
- 位运算符：`&`, `|`, `^`, `~`, `<<`, `>>`
- 其他：`[]`, `()`, `->`, `++`, `--`

### 总结

通过在类中定义成员函数或全局友元函数，可以重载运算符，使得对象之间的操作更加直观和自然。这种特性增强了代码的可读性和可维护性。

---
# `new`能否进行运算符重载，如何操作，举一个必要重载`new`运算符的例子
在C++中，可以重载`new`和`delete`运算符，以自定义内存分配和释放的行为。重载这两个运算符通常用于实现自定义内存管理，或在分配内存时记录信息。
##### 如何重载 `new` 运算符
1. **成员函数重载**：可以在类内重载`new`运算符。
2. **全局函数重载**：可以在类外重载全局的`new`和`delete`运算符。
##### 示例：重载 `new` 运算符
下面是一个重载`new`运算符的示例，用于跟踪内存分配。
```cpp
#include <iostream>

class MyClass {
public:
    int value;

    // 重载 new 运算符
    void* operator new(size_t size) {
        std::cout << "Allocating " << size << " bytes" << std::endl;
        void* p = ::operator new(size);  // 调用全局 new
        return p;
    }

    // 重载 delete 运算符
    void operator delete(void* p) {
        std::cout << "Deallocating memory" << std::endl;
        ::operator delete(p);  // 调用全局 delete
    }

    MyClass(int val) : value(val) {}
};

int main() {
    MyClass* obj = new MyClass(10);  // 使用重载的 new
    std::cout << "Value: " << obj->value << std::endl;
    delete obj;  // 使用重载的 delete
    return 0;
}
```
##### 输出
```
Allocating 4 bytes
Value: 10
Deallocating memory
```
##### 解释
1. **重载 `new`**：
    - `operator new` 函数接受一个参数（请求的字节大小），并在分配内存前输出分配的大小。
    - 使用全局的`::operator new`进行实际的内存分配。
2. **重载 `delete`**：
    - `operator delete` 函数在释放内存前输出释放的消息，并调用全局的`::operator delete`。

##### 何时重载 `new` 和 `delete`
- **监控内存使用**：在调试时查看内存分配情况。
- **自定义内存池**：实现特定的内存管理策略。
- **对象计数**：在分配和释放对象时跟踪对象数量。
##### 总结
重载`new`和`delete`运算符可以帮助实现更灵活和可控的内存管理，特别是在需要监控和优化内存使用的情况下。










