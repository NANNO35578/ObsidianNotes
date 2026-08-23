*基数排序(Radix Sort)* 是一种非比较型的整数排序算法，利用数字的位数进行排序。它通常适用于对整数或字符串进行排序，尤其是当数据范围不大时，效率较高。

> 首先对个位分组 保持先后顺序 依次放入0~9桶中 然后从0~9桶中拿出 
> 然后对十位分组 ......
> 

- MSD : 高位优先
- LSD : 低位优先


---
---

### 基本原理

基数排序的基本思想是将整数按位分成多个“桶”，从最低位开始进行排序，依次向高位排序。它通常采用稳定的排序算法（如计数排序）作为子排序算法。

### 基数排序的步骤

1. **确定最大位数**：找到待排序数组中最大数字的位数。
2. **从最低位到最高位排序**：依次对每一位进行排序，使用稳定的排序算法。
3. **合并结果**：在每一位排序后，将结果合并到一个数组中。

### 时间复杂度

- **时间复杂度**：\(O(n \* k)\)，其中 \(n\) 是待排序元素的数量，\(k\) 是数字的位数。
- **空间复杂度**：\(O(n + k)\)。

### 示例代码

下面是基数排序的 C++  实现示例。

#### C++ 示例

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 计数排序，用于基数排序的辅助函数
void countingSort(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);

    // 计算每个数字在当前位上的出现次数
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    // 计算累积频率
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // 构建输出数组
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // 将排序结果复制回原数组
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

// 基数排序函数
void radixSort(vector<int>& arr) {
    // 找到最大值
    int maxVal = *max_element(arr.begin(), arr.end());

    // 对每个位进行计数排序
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSort(arr, exp);
    }
}

int main() {
    vector<int> arr = {170, 45, 75, 90, 802, 24, 2, 66};

    cout << "原始数组: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    radixSort(arr);

    cout << "排序后数组: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
```

### 总结

基数排序是一种高效的排序算法，适合于处理特定范围的整数或字符串。它的实现依赖于稳定的子排序算法（如计数排序），并通过逐位排序来达到最终的排序结果。适用于数据量大且位数较少的情况。



---
---

Next *Sort* : [[1010 Count Sort]]