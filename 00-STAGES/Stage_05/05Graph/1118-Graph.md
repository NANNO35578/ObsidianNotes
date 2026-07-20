# 迪杰斯特拉算法 `Dijkstra's Algorithm`

> __有向正权图 求两顶点之间最短路径__

> _简介_ : 迪杰斯特拉算法是一种用于寻找加权图中单源最短路径的算法。它可以有效地解决从一个节点到其他所有节点的最短路径问题，适用于边权为非负的情况。

1. 选取点A 为起始顶点
2. 依据邻接矩阵初始化该点到其他点的距离 Array
3. 选出最小路径的中转点
4. 依据邻接矩阵中中转点到其他(非已走过的点)各点的距离与Array中距离比较
5. 更优(距离更短;由不通到连通)则更新A到该点距离 重复步骤 3
6. 当所有顶点都已走过则Array为点A到各个点的最短路径
### 算法步骤
1. **初始化**：设置起始节点的距离为0，其余节点的距离为无穷大。
2. **选择**：从未处理的节点中选择距离起始节点最近的节点。
3. **更新**：检查该节点的邻接节点，更新它们的最短路径估计值。
4. **标记处理**：将选择的节点标记为已处理。
5. **重复**：重复步骤2至4，直到所有节点都被处理。

---
---
> __图的最小生成树__ 将图连通但不产生环

# 克鲁斯卡尔算法 `Kruskal's Algorithm`

_简介_ :克鲁斯卡尔算法是一种用于寻找加权无向图的最小生成树的算法。它通过将图的边按权重排序，然后逐步添加边，确保不会形成环，直到连接所有顶点。

1. 将边排序
2. 选取最小边，连通两顶点
3. 选取下一条最小边，连通两顶点，若与已连通顶点产生环，抛弃该选择
4. 重复步骤 3 ，连通整个图 ==生成树包含(V - 1)条边== 结束
### 算法步骤 
1. **边排序**：将图中的所有边按权重从小到大排序。
2. **初始化**：创建一个空的生成树。
3. **逐边处理**：
   - 遍历排序后的边，逐一检查每条边。
   - 如果当前边的两个端点不在同一个集合中（即添加此边不会形成环），则将这条边添加到生成树中。
4. **结束条件**：当生成树中包含 \( V-1 \) 条边（其中 \( V \) 是图中的顶点数）时，算法结束。
---
## Prim算法

_简介_ :Prim算法用于寻找加权无向图的最小生成树。它从一个起始节点开始，逐步添加最短边，直到连接了所有节点。

1. 选取一点作为起始节点
2. 从邻接点中选取
### 算法步骤
1. **初始化**：选择一个起始节点，标记为已连接。
2. **选择**：在已连接的节点中，选择一条最小边连接到未连接的节点。
3. **更新**：将新节点标记为已连接，重复选择过程。
4. **结束**：直到所有节点都已连接。
---


### 总结
克鲁斯卡尔算法是构建最小生成树的有效方法，特别适用于稀疏图。通过优先考虑边的权重并使用并查集来管理集合，确保可以高效地构建所需的最小生成树。


### 总结
上述代码分别展示了迪杰斯特拉算法和Prim算法的基本实现。通过这些算法，可以有效地解决最短路径和最小生成树的问题。


### 总结
有向无环图（DAG）是处理依赖关系的重要数据结构，而拓扑排序则是对DAG进行线性排序的一种有效方法。通过顶点的正确排序，可以确保在处理依赖时遵循合适的顺序。


---





# 拓扑排序
> 求得理想顺序
> 将项目中具备依赖关系的活动求得可执行的线性顺序


DAG 有向无环图


AOV网


1. 入度统计 Array
2. 取出入度为0的点放入队列
	1. 队首顶点的邻接点的入度更新 产生新的入度为0的点放入队列
	2. `pop()` 
	3. 回到 1 直到队列为空



## 有向无环图（DAG）

_简介_ :有向无环图(Directed Acyclic Graph, DAG)是一种特殊的图结构，其中每条边都有方向，并且不存在从某个顶点出发能够回到自身的路径(即没有环)。DAG在许多领域都有广泛应用，如任务调度、版本控制、数据流分析等。

### 特点
1. **无环性**：DAG不包含任何环，因此从一个节点出发无法回到自身。
2. **拓扑排序**：可以对DAG进行拓扑排序，使得对于每一条边 $u\to v$，节点 $u$ 在节点 $v$ 之前。

## 拓扑排序(Topological Sorting)

_简介_ :拓扑排序是对DAG的一种线性排序，使得对于每一条边 $u\to v$，节点 $u$ 在排序中出现在节点 $v$ 之前。拓扑排序不是唯一的，可能存在多种有效的排序方式。

### 应用场景
1. **任务调度**：在任务依赖关系中，确保先完成某些任务后再开始依赖它们的任务。
2. **编译顺序**：在程序编译中，确保先编译所依赖的模块。
3. **版本控制**：在处理文件版本时，确保按照正确的顺序应用更改。

### 算法
拓扑排序有多种实现方法，常用的包括：
1. **Kahn算法**
2. **深度优先搜索（DFS）**

#### Kahn算法步骤
1. 计算每个节点的入度。
2. 将所有入度为0的节点加入队列。
3. 反复从队列中取出节点，输出并减少其邻接节点的入度。如果邻接节点的入度变为0，则将其加入队列。
4. 继续直到队列为空。

#### 深度优先搜索（DFS）方法
1. 对每个节点执行DFS，标记为访问中。
2. 当所有邻接节点访问完毕后，将该节点加入结果列表。
3. 最终反转结果列表以得到拓扑排序。

---
---

### 示例代码（Kahn算法）

```c
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int adj[MAX][MAX], inDegree[MAX];
int queue[MAX], front = 0, rear = 0;

void enqueue(int v) {
    queue[rear++] = v;
}

int dequeue() {
    return queue[front++];
}

void topologicalSort(int n) {
    for (int i = 0; i < n; i++) {
        inDegree[i] = 0;
    }

    // 计算入度
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (adj[u][v]) {
                inDegree[v]++;
            }
        }
    }

    // 将所有入度为0的节点入队
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            enqueue(i);
        }
    }

    printf("拓扑排序结果: ");

    while (front < rear) {
        int u = dequeue();
        printf("%d ", u);

        for (int v = 0; v < n; v++) {
            if (adj[u][v]) {
                inDegree[v]--;
                if (inDegree[v] == 0) {
                    enqueue(v);
                }
            }
        }
    }
}

int main() {
    int n = 6; // 节点数量
    // 构建邻接矩阵
    adj[0][1] = 1;
    adj[0][2] = 1;
    adj[1][3] = 1;
    adj[2][3] = 1;
    adj[3][4] = 1;
    adj[4][5] = 1;

    topologicalSort(n);

    return 0;
}
```

---

 `Kruskal's Algorithm`
```c
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    int parent[MAX];
    int rank[MAX];
} DisjointSet;

Edge edges[MAX];
int edgeCount = 0;

void initializeSet(DisjointSet* ds, int n) {
    for (int i = 0; i < n; i++) {
        ds->parent[i] = i;
        ds->rank[i] = 0;
    }
}

int find(DisjointSet* ds, int u) {
    if (ds->parent[u] != u) {
        ds->parent[u] = find(ds, ds->parent[u]);
    }
    return ds->parent[u];
}

void unionSets(DisjointSet* ds, int u, int v) {
    int rootU = find(ds, u);
    int rootV = find(ds, v);

    if (rootU != rootV) {
        if (ds->rank[rootU] > ds->rank[rootV]) {
            ds->parent[rootV] = rootU;
        } else if (ds->rank[rootU] < ds->rank[rootV]) {
            ds->parent[rootU] = rootV;
        } else {
            ds->parent[rootV] = rootU;
            ds->rank[rootU]++;
        }
    }
}

int compareEdges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

void kruskal(int n) {
    DisjointSet ds;
    initializeSet(&ds, n);

    qsort(edges, edgeCount, sizeof(edges[0]), compareEdges);

    printf("Edge \tWeight\n");

    for (int i = 0; i < edgeCount; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int weight = edges[i].weight;

        if (find(&ds, u) != find(&ds, v)) {
            unionSets(&ds, u, v);
            printf("%d - %d \t%d\n", u, v, weight);
        }
    }
}

int main() {
    int n = 4; // Number of vertices
    edges[edgeCount++] = (Edge){0, 1, 10};
    edges[edgeCount++] = (Edge){0, 2, 6};
    edges[edgeCount++] = (Edge){0, 3, 5};
    edges[edgeCount++] = (Edge){1, 3, 15};
    edges[edgeCount++] = (Edge){2, 3, 4};

    kruskal(n);

    return 0;
}
```

迪杰斯特拉算法

```c
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define V 9

int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int graph[V][V], int src) {
    int dist[V]; 
    bool sptSet[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; i++) {
        printf("%d \t\t %d\n", i, dist[i]);
    }
}

int main() {
    int graph[V][V] = { {0, 4, 0, 0, 0, 0, 0, 8, 0},
                        {4, 0, 8, 0, 0, 0, 0, 11, 0},
                        {0, 8, 0, 7, 0, 4, 0, 0, 2},
                        {0, 0, 7, 0, 9, 14, 0, 0, 0},
                        {0, 0, 0, 9, 0, 10, 0, 0, 0},
                        {0, 0, 4, 14, 10, 0, 2, 0, 0},
                        {0, 0, 0, 0, 0, 2, 0, 1, 6},
                        {8, 11, 0, 0, 0, 0, 1, 0, 7},
                        {0, 0, 2, 0, 0, 0, 6, 7, 0} };

    dijkstra(graph, 0);

    return 0;
}
```

Prim算法

```c
#include <stdio.h>
#include <limits.h>

#define V 5

int minKey(int key[], bool mstSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (mstSet[v] == false && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

void primMST(int graph[V][V]) {
    int parent[V];
    int key[V];
    bool mstSet[V];

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    key[0] = 0; 
    parent[0] = -1; 

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    printf("Edge \tWeight\n");
    for (int i = 1; i < V; i++) {
        printf("%d - %d \t%d\n", parent[i], i, graph[i][parent[i]]);
    }
}

int main() {
    int graph[V][V] = { {0, 2, 0, 6, 0},
                        {2, 0, 3, 8, 5},
                        {0, 3, 0, 0, 7},
                        {6, 8, 0, 0, 9},
                        {0, 5, 7, 9, 0} };

    primMST(graph);

    return 0;
}
```

