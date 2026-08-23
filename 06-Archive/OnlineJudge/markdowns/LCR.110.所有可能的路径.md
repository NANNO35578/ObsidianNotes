[LCR 110. 所有可能的路径](https://leetcode.cn/problems/bP4bmD/)

1. **`result`**：用于存储所有从起点到终点的路径。
2. **`path`**：用于存储当前正在探索的路径。
3. **`dfs` 函数**：
    - 将当前节点加入路径。
    - 如果当前节点是终点（`n-1`），将当前路径加入结果。
    - 否则，遍历当前节点的所有邻接节点，并递归调用 `dfs`。
    - 在回溯时，移除当前节点。
4. **从节点 0 开始搜索**：调用 `dfs(0)` 开始深度优先搜索。

```cpp
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> result; // 用于存储所有路径
        vector<int> path;           // 用于存储当前路径
        int n = graph.size();       // 图的节点数

        // 定义深度优先搜索函数
        function<void(int)> dfs = [&](int node) {
            path.push_back(node); // 将当前节点加入路径
            if (node == n - 1) {  // 如果到达终点
                result.push_back(path); // 将当前路径加入结果
            } else {
                // 遍历当前节点的所有邻接节点
                for (int neighbor : graph[node]) {
                    dfs(neighbor); // 递归调用DFS
                }
            }
            path.pop_back(); // 回溯，移除当前节点
        };

        dfs(0); // 从节点 0 开始搜索
        return result;
    }
```







