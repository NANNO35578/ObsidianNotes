
std::`vector<bool>` is a possibly space-efficient specialization of [std::vector](https://www.en.cppreference.com/w/cpp/container/vector.html "cpp/container/vector") for the type bool.

The manner in which std::`vector<bool>` is made space efficient (as well as whether it is optimized at all) is implementation defined. One potential optimization involves coalescing vector elements such that each element occupies a single bit instead of sizeof(bool) bytes.

std::`vector<bool>` behaves similarly to [std::vector](https://www.en.cppreference.com/w/cpp/container/vector.html "cpp/container/vector"), but in order to be space efficient, it:

- Does not necessarily store its elements as a contiguous array.
- Exposes class std::`vector<bool>`::[`reference`](https://www.en.cppreference.com/w/cpp/container/vector_bool/reference.html "cpp/container/vector bool/reference") as a method of accessing individual bits. In particular, objects of this class are returned by [operator[]](https://www.en.cppreference.com/w/cpp/container/vector/operator_at.html "cpp/container/vector/operator at") by value.
- Does not use std::`allocator_traits`::construct to construct bit values.
- Does not guarantee that different elements in the same container can be modified concurrently by different threads.

[std::`vector<bool>`](https://www.en.cppreference.com/w/cpp/container/vector_bool.html)


---
[LeetCode](https://leetcode.cn/discuss/post/3156079/jiao-liu-yuan-ma-fen-xi-stdvectorbool-sh-9zbw/)

---

下面通过几个对比示例，说明 `std::vector<bool>` 与普通的 `std::vector<int>` 在以下几个方面的差异及潜在问题：

1. 元素是否连续存储 — 无法取地址
2. `operator[]` 返回代理类而非真引用
3. 不能像 C 数组那样做指针运算 / 与 C 接口互操作
4. 并发写入不同下标也可能冲突

---

## 1. 连续存储与取地址

```cpp
#include <vector>

void test_address() {
    std::vector<int> vi = {1,2,3};
    int* pi = &vi[0];       // OK: vi[0] 返回 int&, 可以取地址
    pi[1] = 42;             // OK: 连续存储

    std::vector<bool> vb = {true,false,true};
    // bool* pb = &vb[0];   // Error: vb[0] 返回 proxy（临时对象），不能取 bool*
    // *(vb.data() + 1) = false; // 不一定编译：data() 不返 bool*
}
```

- `std::vector<int>` 保证元素按 sizeof(int) 连续存储，`&v[0]` 是有效的 `int*`。
- `std::vector<bool>` 用位打包（bit‐packed），`vb[0]` 返回一个临时的 proxy 对象（`vector<bool>::reference`），不能取地址，也不保证内存是按一个 `bool` 连续存储的。

---

## 2. `operator[]` 返回代理类

```cpp
#include <type_traits>
#include <vector>

void test_type() {
    std::vector<int>  vi = {0,1,2};
    static_assert(
        std::is_same<decltype(vi[0]), int&>::value,
        "vi[0] is int&"
    );

    std::vector<bool> vb = {0,1,0};
    // vb[0] 的类型是 std::vector<bool>::reference，而不是 bool&
    static_assert(
        !std::is_same<decltype(vb[0]), bool&>::value,
        "vb[0] is not bool&"
    );
}
```

- `vi[0]` 是 `int&`，一切如常；
- `vb[0]` 是一个“代理”临时对象，用来读／写对应的位。

---

## 3. 与 C 接口互操作

```cpp
#include <algorithm>
#include <cstring>    // for memset
#include <vector>

void test_memset() {
    std::vector<int> vi(100);
    // 快速清零
    std::memset(vi.data(), 0, vi.size()*sizeof(int));  // OK

    std::vector<bool> vb(100);
    // std::memset(vb.data(), 0, vb.size()*sizeof(bool)); 
    // Error: vb.data() 不是 bool*，位打包后无法直接 memset
    // 应使用 std::fill(vb.begin(), vb.end(), false);
}
```

- 对 `vector<int>` 可直接用 `memset` / `memcpy`；
- `vector<bool>` 由于位打包，必须走 STL 算法（`std::fill`、`std::copy`）才能保证正确性。

---

## 4. 并发修改不同下标也可能冲突

```cpp
#include <iostream>
#include <thread>
#include <vector>

void race_on_int() {
    std::vector<int> vi(64, 0);
    std::thread t1([&]{ for(int i=0;i<1'000'000;i++) vi[0]++; });
    std::thread t2([&]{ for(int i=0;i<1'000'000;i++) vi[1]++; });
    t1.join(); t2.join();
    // vi[0] 和 vi[1] 在不同内存单元，若无锁也*可能*安全（取决于平台对单独 int 的原子性保证）
    std::cout << vi[0] << " " << vi[1] << "\n";
}

void race_on_bool() {
    std::vector<bool> vb(64, false);
    std::thread t1([&]{ for(int i=0;i<1'000'000;i++) vb[0] = !vb[0]; });
    std::thread t2([&]{ for(int i=0;i<1'000'000;i++) vb[7] = !vb[7]; });
    t1.join(); t2.join();
    // vb[0] 与 vb[7] 虽是不同下标，但很可能打包在同一个机器字(word)中
    // 同时写会发生数据竞争，结果未定义
    std::cout << vb[0] << " " << vb[7] << "\n";
}

int main(){
    race_on_int();
    race_on_bool();
}
```

- 对 `vector<int>`，不同下标通常是不同的 `int` 存储单元；
- 对 `vector<bool>`，多个 bit 可能共用一个字，跨线程写入不同 bit 也会争用同一个内存位置，产生数据竞争。

---

### 小结

使用 `std::vector<bool>` 带来的“位打包”优化虽然节省空间，但也引入了：

- 不能像常规数组那样取元素地址、做指针运算
- `operator[]` 返回代理对象而非 `bool&`
- 与 C 风格接口（`memset`/`memcpy`）不兼容
- 并发写不同元素也可能冲突

若上述特性对你很重要，建议使用 `std::vector<char>`/`std::vector<unsigned char>` 或第三方的**压缩位图库**（例如 Boost.DynamicBitset）来替代。


---
---

`std::vector<bool>` 在C++中有一个特殊的实现，它会进行空间压缩，和其他类型的 `std::vector`（如 `std::vector<int>` 或 `std::vector<double>`）有所不同。

### 为什么 `std::vector<bool>` 会压缩空间？

`std::vector<bool>` 并不像其他标准容器那样直接存储每个元素。因为 `bool` 只需要一个位来表示其值（`true` 或 `false`），但是标准的 `std::vector<T>` 会为每个元素分配一个完整的字节（8位）。为了节省空间，`std::vector<bool>` 采用了位压缩技术，将多个 `bool` 值存储在同一个字节中。

### 如何实现空间压缩？

- `std::vector<bool>` 将多个 `bool` 值按位存储在一个字节或更大的数据块中。比如，假设你存储了 8 个 `bool` 值，它们将共同占用 1 个字节的空间，而不是 8 个字节。
    
- 这种方式的好处是节省了大量的空间，尤其是在存储大量 `bool` 值时。
    
- 但是这种实现方式也带来了一些缺点，尤其是在访问单个 `bool` 元素时，它可能会导致额外的计算开销，因为需要对存储块中的特定位进行操作。
    

### 存储方式的实现

- **常规的 `std::vector<bool>` 实现：** 通常会用一个 `std::vector<uint8_t>` 或类似的数据结构，存储压缩后的 `bool` 值集合。每个 `bool` 值不再占用单独的字节，而是将多个 `bool` 值压缩成 1 个字节或更大单元的一部分。
    
- **性能影响：** 这种方式的性能可能较差，尤其在进行频繁访问时。因为对于每次访问，程序需要计算出相应的位位置并进行位操作（位与、位或等），这比简单的字节访问慢。
    

### 为什么 `std::vector<bool>` 设计成这样？

在C++的设计中，`std::vector<bool>` 是为了高效存储大量布尔值而进行空间优化的容器。它在节省内存方面表现出色，尤其是在大规模布尔数组时（如表示位图、布尔矩阵等）。但是，由于其位压缩的方式，它不再是简单的数组操作，可能导致性能上的开销。

### 是否有替代方案？

如果你希望避免 `std::vector<bool>` 的空间压缩，并且需要更高效的随机访问，可以考虑以下替代方案：

- **`std::bitset`**：适用于固定大小的位集合，通常不需要动态调整大小。
    
- **`std::vector<uint8_t>` 或 `std::vector<uint32_t>`**：使用其他整数类型来存储布尔值，每个 `bool` 值使用 1 个字节或更大的数据块进行存储，不会进行位压缩。
    
- **第三方库**：如 Boost 的 `boost::dynamic_bitset`，它提供了灵活且高效的位集合，能够像 `std::vector<bool>` 一样动态扩展，但提供更高效的访问性能。
    

总结来说，`std::vector<bool>` 确实会对存储的 `bool` 值进行空间压缩，从而减少内存占用，但这也带来了一些性能上的代价。