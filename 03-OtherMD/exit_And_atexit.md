
在C++中，`exit` 和 `atexit` 是用于程序终止和清理的函数。以下是它们的基本用法：

### `exit`

`exit` 函数用于立即终止程序。它接受一个整数参数，通常用于表示程序的退出状态：

```cpp
#include <iostream>
#include <cstdlib>

int main() {
    std::cout << "程序开始。" << std::endl;

    // 条件判断，决定是否退出
    if (/* some condition */) {
        std::cout << "发生错误，程序退出。" << std::endl;
        exit(EXIT_FAILURE); // 以失败状态退出
    }

    std::cout << "程序正常结束。" << std::endl;
    exit(EXIT_SUCCESS); // 正常退出
}
```

- `EXIT_SUCCESS` 和 `EXIT_FAILURE` 是标准宏，分别表示成功和失败的退出状态。

### `atexit`

`atexit` 函数用于注册一个在程序正常终止时调用的函数。可以用来进行清理工作，如释放资源等：

```cpp
#include <iostream>
#include <cstdlib>

void cleanup() {
    std::cout << "清理工作完成。" << std::endl;
}

int main() {
    // 注册清理函数
    atexit(cleanup);

    std::cout << "程序开始。" << std::endl;

    // 条件判断，决定是否退出
    if (/* some condition */) {
        std::cout << "发生错误，程序退出。" << std::endl;
        exit(EXIT_FAILURE); // 以失败状态退出
    }

    std::cout << "程序正常结束。" << std::endl;
    exit(EXIT_SUCCESS); // 正常退出
}
```

### 总结

- **`exit`**：用于立即终止程序，带有退出状态。
- **`atexit`**：用于注册清理函数，在程序正常终止时自动调用。

这些函数提供了一种有效的方法来管理程序的生命周期和资源。
