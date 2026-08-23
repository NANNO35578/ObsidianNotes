> 对属于于 \[0,1\) 的*等长*小数(实型)排序
> 




---
---
桶排序是一种分布式排序算法，适用于数据分布均匀的情况。它的基本思想是将数据分到多个桶中，再对每个桶内的数据进行排序，最后将所有桶中的数据合并起来。

### 桶排序的步骤

1. **创建桶**：根据最大值和最小值，确定桶的数量和范围。
2. **分配数据**：将数据分配到各个桶中。
3. **排序每个桶**：对每个桶内的数据进行排序，通常使用快速排序、插入排序等。
4. **合并结果**：将所有桶中的数据合并成一个有序数组。

### 算法复杂度

- **时间复杂度**：最优情况为 \(O(n + k)\)，最坏情况为 \(O(n^2)\)，其中 \(n\) 是数据的数量，\(k\) 是桶的数量。
- **空间复杂度**：\(O(n + k)\)。

### C++ 示例代码

下面是一个简单的桶排序的 C++ 实现：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 桶排序函数
void bucketSort(vector<float>& arr) {
    // 1. 找到最大值和最小值
    float maxValue = *max_element(arr.begin(), arr.end());
    float minValue = *min_element(arr.begin(), arr.end());
    int bucketCount = static_cast<int>(maxValue - minValue) + 1;

    // 2. 创建桶
    vector<vector<float>> buckets(bucketCount);

    // 3. 将数据分配到桶中
    for (float num : arr) {
        int bucketIndex = static_cast<int>(num - minValue);
        buckets[bucketIndex].push_back(num);
    }

    // 4. 对每个桶进行排序并合并
    arr.clear();
    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end());
        arr.insert(arr.end(), bucket.begin(), bucket.end());
    }
}

int main() {
    vector<float> arr = {0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68};

    cout << "原始数组: ";
    for (float num : arr) {
        cout << num << " ";
    }
    cout << endl;

    bucketSort(arr);

    cout << "排序后数组: ";
    for (float num : arr) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
```

### 代码解析

1. **找到最大值和最小值**：这有助于确定桶的数量和范围。
2. **创建桶**：使用一个二维 `vector` 来存储每个桶中的数据。
3. **数据分配**：遍历原始数组，将每个元素放入相应的桶。
4. **排序和合并**：对每个桶使用 `sort` 函数进行排序，然后将排序后的数据合并回原数组。

### 使用注意

- 桶排序适用于数据均匀分布的情况，若数据分布不均，可能会导致某些桶过满，从而影响排序效率。
- 选择合适的桶数量对于算法性能至关重要。



---
---
---

Next Note : [[1010 Radix Sort]] 
