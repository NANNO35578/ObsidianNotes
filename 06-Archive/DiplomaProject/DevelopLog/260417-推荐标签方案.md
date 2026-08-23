# 推荐算法

一、新用户冷启动推荐算法
$$
\begin{aligned}
\text{基础标签集合 } S_{\text{base}} &= \text{Top3(用户笔记标签频率)} \cup \text{Top2(用户最新标签)} \\
\text{搜索标签集合 } S_{\text{search}} &= \text{Top2(搜索结果标签频率)} \\
\text{热门标签集合 } S_{\text{hot}} &= \text{Top3(全局热门标签)} \\
\text{最终推荐标签集合 } S_{\text{rec}} &= S_{\text{base}} \cup S_{\text{search}} \cup S_{\text{hot}} \\
\text{推荐笔记列表 } N_{\text{rec}} &= \text{Top50}\left( \text{公开笔记}, \text{标签} \in S_{\text{rec}}, \text{创建时间降序} \right)
\end{aligned}
$$

二、老用户标签兴趣评分算法
$$
\begin{aligned}
% 1. 行为加权标签得分（含行为权重、出现次数、时间衰减）
\mathrm{tag\_score}(t)
&= \Big(1\cdot N_{\text{view}}(t) + 3\cdot N_{\text{like}}(t) + 7\cdot N_{\text{collect}}(t)\Big)
\\
&\quad \times C(t)
\times e^{-\lambda \Delta t}, \quad \lambda=0.1
\\
% 2. 按行为得分取Top-K标签
S_{\text{behavior}} &= \mathrm{Top}_K\big(\{t\}, \mathrm{tag\_score}(t)\big)
\\
% 3. 最近搜索高频标签
S_{\text{search}} &= \mathrm{Top}_2(\text{最近搜索结果标签频率})
\\
% 4. 最终用户兴趣标签集合
S_{\text{user}} &= S_{\text{behavior}} \cup S_{\text{search}}
\\
% 5. 推荐笔记
N_{\text{rec}} &= \mathrm{Top}_{50}\big(\text{公开笔记}, \text{tag}\in S_{\text{user}}, \text{时间降序}\big)
\end{aligned}
$$
符号说明
$$
\begin{aligned}
N_{\text{view}}(t)   &: \text{标签 } t \text{ 对应的用户浏览次数} \\
N_{\text{like}}(t)   &: \text{标签 } t \text{ 对应的用户点赞次数} \\
N_{\text{collect}}(t)&: \text{标签 } t \text{ 对应的用户收藏次数} \\
C(t)                 &: \text{标签 } t \text{ 在推荐列表中出现的次数} \\
\Delta t             &: \text{用户最近一次与该标签交互的时间差} \\
S_{\text{behavior}}  &: \text{用户行为计算得到的兴趣标签集合} \\
S_{\text{search}}    &: \text{最近搜索结果中出现最多的2个标签} \\
S_{\text{user}}      &: \text{融合搜索意图后的最终用户兴趣标签集合}
\end{aligned}
$$

````markdown title:"# 算法补充. 已补充"

#### ⚠️ 1. C(t) 定义有问题 *这个就不用了, 新用户前5次使用算法1即可*
你现在写的是：
> 标签在推荐列表中出现次数

👉 这是**循环依赖**（推荐结果还没出）
##### 建议改成：
`C(t) = 标签在全局热门笔记中的出现频率` ==> 或直接用：`C(t) = log(1 + note_count(tag))`

---

#### ⚠️ 2. 时间衰减 Δt 用错对象了 *有道理, 问题是怎么算? 新建的表可以算*
你写的是：
> 笔记发布时间

但你在算的是：👉 **用户兴趣，不是内容热度**
##### 应该改为：
`Δt = 用户最近一次与该标签交互的时间差`

👉 否则会出现：
* 老用户兴趣被“新笔记”误导

---

#### ⚠️ 3. 冷启动策略可以再补一刀 *有道理*
你现在只有：
* 用户笔记标签
* 搜索标签

👉 问题：
👉 **新用户可能啥都没有**

##### 建议加：
`S_hot = 全局热门标签 TopK` ==> `S_rec = S_base ∪ S_search ∪ S_hot`
````

---



# 表新建

## 用户行为表
```sql title:"用户行为日志表（推荐：统一日志表）"
CREATE TABLE user_action (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT NOT NULL,
    note_id BIGINT NOT NULL,
    action_type TINYINT NOT NULL, 
    -- 1=view, 2=like, 3=collect

    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    INDEX idx_user (user_id),
    INDEX idx_note (note_id),
    INDEX idx_action (action_type),

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (note_id) REFERENCES note(id)
);
```


## 搜索记录表
```sql
CREATE TABLE search_history (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT NOT NULL,
    query VARCHAR(255),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    INDEX idx_user_time (user_id, created_at)
);
```

👉 用途：
* 计算 `S_search`
* 识别用户短期兴趣（非常关键）



## 点赞表&收藏表

点赞表（可选：是否冗余 user_action）
如果你需要快速判断“用户是否点赞”，建议单独建：
```sql
CREATE TABLE note_like (
    user_id BIGINT,
    note_id BIGINT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id, note_id)
);
```

同理收藏：
```sql
CREATE TABLE note_collect (
    user_id BIGINT,
    note_id BIGINT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id, note_id)
);
```

👉 说明：
* `user_action` 用于统计
* `like/collect` 表用于**去重 + 快速查询**

## 标签统计表
- 强烈建议，否则你会被性能打爆
你的公式：
```
N_view(t), N_like(t), N_collect(t)
```

如果每次都 join + count： 👉 **直接炸数据库**. 所以要做聚合表：
```sql
CREATE TABLE user_tag_stat (
    user_id BIGINT,
    tag_id BIGINT,

    view_count INT DEFAULT 0,
    like_count INT DEFAULT 0,
    collect_count INT DEFAULT 0,

    last_action_time DATETIME,

    PRIMARY KEY (user_id, tag_id),
    INDEX idx_user_score (user_id)
);
```

👉 每次用户行为：
* 写 `user_action`
* **异步更新这个表（推荐消息队列）**


# 表修改
## ✅ 1. note 表

你已经意识到了：
```sql
ALTER TABLE note ADD COLUMN is_public TINYINT DEFAULT 0;
```
```sql title:"👉 建议补充："
ALTER TABLE note ADD COLUMN view_count INT DEFAULT 0;
ALTER TABLE note ADD COLUMN like_count INT DEFAULT 0;
ALTER TABLE note ADD COLUMN collect_count INT DEFAULT 0;
```

用途：
* 排序（热度）
* C(t) 计算（你公式里用到了）

---

## ❗ 2. tag 表（删 user_id 是对的）

改成：
```sql
CREATE TABLE tag (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) UNIQUE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```
👉 否则会出现：
* C++ / cpp / C plus plus 多份重复（你前面已经问过）

---

##### ~~✅ 3. note_tag（建议加索引）~~
~~本来就有, 不用了~~
```sql
CREATE INDEX idx_tag ON note_tag(tag_id);
CREATE INDEX idx_note ON note_tag(note_id);
```

---


# 
# 
# 

# its ans

你这个推荐算法**整体方向是对的**，而且已经比大多数毕业设计强不少了，但如果直接按现在的表结构去做，会卡在一个核心问题：

👉 **你的算法是“行为驱动”的，但数据库几乎没有“行为数据”**

也就是说：
现在的表只能支持“基于内容（标签/时间）推荐”，**但你的公式需要用户行为日志 + 聚合数据**。

下面我帮你把**必须补的数据库设计**和**算法层面的小坑**一起梳理清楚。

---

# 一、先指出一个关键问题（不改会直接做不出来）

你现在的公式里有：

* `N_view(t)`
* `N_like(t)`
* `N_collect(t)`
* `最近搜索标签`
* 时间衰减 `Δt`

👉 但当前表：

* ❌ 没有浏览记录
* ❌ 没有点赞表
* ❌ 没有收藏表
* ❌ 没有搜索记录
* ❌ 没有标签统计表

👉 结论：**必须新增行为数据层**

---

# 二、数据库必须新增的表（核心）

## 1️⃣ 用户行为日志表（推荐：统一日志表）

这是最关键的一张：

```sql
CREATE TABLE user_action (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT NOT NULL,
    note_id BIGINT NOT NULL,
    action_type TINYINT NOT NULL, 
    -- 1=view, 2=like, 3=collect

    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    INDEX idx_user (user_id),
    INDEX idx_note (note_id),
    INDEX idx_action (action_type),

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (note_id) REFERENCES note(id)
);
```

### 为什么这样设计？

统一行为表优点：

* 简化扩展（以后加 share / comment 不用加表）
* 支持实时统计
* 方便做埋点分析

---

## 2️⃣ 搜索记录表（冷启动 & intent）

```sql
CREATE TABLE search_history (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT NOT NULL,
    query VARCHAR(255),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    INDEX idx_user_time (user_id, created_at)
);
```

👉 用途：

* 计算 `S_search`
* 识别用户短期兴趣（非常关键）

---

## 3️⃣ 点赞表（可选：是否冗余 user_action）

如果你需要快速判断“用户是否点赞”，建议单独建：

```sql
CREATE TABLE note_like (
    user_id BIGINT,
    note_id BIGINT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id, note_id)
);
```

同理收藏：

```sql
CREATE TABLE note_collect (
    user_id BIGINT,
    note_id BIGINT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id, note_id)
);
```

👉 说明：

* `user_action` 用于统计
* `like/collect` 表用于**去重 + 快速查询**

---

## 4️⃣ 标签统计表（强烈建议，否则你会被性能打爆）

你的公式：

```
N_view(t), N_like(t), N_collect(t)
```

如果每次都 join + count：

👉 **直接炸数据库**

所以要做聚合表：

```sql
CREATE TABLE user_tag_stat (
    user_id BIGINT,
    tag_id BIGINT,

    view_count INT DEFAULT 0,
    like_count INT DEFAULT 0,
    collect_count INT DEFAULT 0,

    last_action_time DATETIME,

    PRIMARY KEY (user_id, tag_id),
    INDEX idx_user_score (user_id)
);
```

👉 每次用户行为：

* 写 `user_action`
* **异步更新这个表（推荐消息队列）**

---

# 三、现有表需要修改的点

## ✅ 1. note 表

你已经意识到了：

```sql
ALTER TABLE note ADD COLUMN is_public TINYINT DEFAULT 0;
```

👉 建议补充：

```sql
ALTER TABLE note ADD COLUMN view_count INT DEFAULT 0;
ALTER TABLE note ADD COLUMN like_count INT DEFAULT 0;
ALTER TABLE note ADD COLUMN collect_count INT DEFAULT 0;
```

用途：

* 排序（热度）
* C(t) 计算（你公式里用到了）

---

## ❗ 2. tag 表（删 user_id 是对的）

改成：

```sql
CREATE TABLE tag (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) UNIQUE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

👉 否则会出现：

* C++ / cpp / C plus plus 多份重复（你前面已经问过）

---

## ✅ 3. note_tag（建议加索引）

```sql
CREATE INDEX idx_tag ON note_tag(tag_id);
CREATE INDEX idx_note ON note_tag(note_id);
```

---

# 四、算法层面需要补充的关键点

## ⚠️ 1. C(t) 定义有问题

你现在写的是：

> 标签在推荐列表中出现次数

👉 这是**循环依赖**（推荐结果还没出）

### 建议改成：

```math
C(t) = 标签在全局热门笔记中的出现频率
```

👉 或直接用：

```math
C(t) = log(1 + note_count(tag))
```

---

## ⚠️ 2. 时间衰减 Δt 用错对象了

你写的是：

> 笔记发布时间

但你在算的是：

👉 **用户兴趣，不是内容热度**

### 应该改为：

```math
Δt = 用户最近一次与该标签交互的时间差
```

👉 否则会出现：

* 老用户兴趣被“新笔记”误导

---

## ⚠️ 3. 冷启动策略可以再补一刀

你现在只有：

* 用户笔记标签
* 搜索标签

👉 问题：

👉 **新用户可能啥都没有**

### 建议加：

```math
S_hot = 全局热门标签 TopK
```

最终：

```math
S_rec = S_base ∪ S_search ∪ S_hot
```

---

# 五、推荐系统完整架构（你现在其实缺这个）

你现在是“SQL驱动推荐”，但应该升级为：

### 分层：

```
行为采集层
    ↓
日志表 (user_action)
    ↓
聚合层（user_tag_stat） ← 异步计算
    ↓
推荐服务（计算标签）
    ↓
ES + MySQL 召回笔记
```

---

# 六、总结（你需要改的点）

## ✅ 必加

* user_action（核心）
* search_history
* user_tag_stat（强烈建议）

## ✅ 建议加

* note_like / note_collect
* note 热度字段

## ✅ 修改

* note.is_public
* tag 去 user_id + 唯一化

## ⚠️ 算法修正

* C(t) 改为全局统计
* Δt 改为“行为时间”
* 冷启动加热门标签

---

# 如果你想更进一步

我可以帮你直接升级到：

👉 **“工业级推荐系统简化版”设计（含 ES + 倒排索引 + 排序模型）**

或者帮你写：

👉 Drogon + C++ 推荐服务实现（含SQL +缓存 +接口设计）

你现在这套已经接近“能写进论文的水平”了，再补一层就很漂亮了。


----

# Other QA


````markdown title:"More"
第1点你这么处理是合理的：**直接把 C(t) 删掉 + 冷启动走算法1**，可以明显降低复杂度，而且对毕业设计是加分的（更可解释）。

---

重点说第2点：**Δt 怎么算**

你现在其实面临的是一个经典问题：

👉 **时间衰减到底绑定“内容”还是“用户行为”？**

你已经选对方向了：
✔ 应该绑定“用户行为”

---

# 一、正确的 Δt 定义（推荐用这个）

直接给你一个可以落地的版本：

```math
\Delta t = \text{当前时间} - \text{用户最后一次与标签 t 交互的时间}
```

也就是：

```text
Δt = now - last_action_time(tag)
```

---

# 二、数据库如何支持这个计算

刚才建议你的表：

```sql
user_tag_stat
```

这里有个关键字段：

```sql
last_action_time DATETIME
```

👉 这个字段就是为 Δt 准备的

---

# 三、行为如何更新 Δt

当用户发生行为时（view / like / collect）：

### 1️⃣ 找到该 note 的所有 tag

```sql
SELECT tag_id FROM note_tag WHERE note_id = ?
```

---

### 2️⃣ 更新 user_tag_stat

```sql
INSERT INTO user_tag_stat (user_id, tag_id, last_action_time)
VALUES (?, ?, NOW())
ON DUPLICATE KEY UPDATE
    view_count = view_count + (action_type == 1),
    like_count = like_count + (action_type == 2),
    collect_count = collect_count + (action_type == 3),
    last_action_time = NOW();
```

👉 关键点：

* **每次行为都会刷新 last_action_time**
* 不需要存每条记录的时间（否则太重）

---

# 四、Δt 在公式里的最终形态

你原来的：

```math
e^{-\lambda \Delta t}
```

可以直接用，但要注意单位👇

---

## ⚠️ 建议单位：小时

```math
\Delta t = \frac{\text{now} - \text{last\_action\_time}}{3600}
```

---

## 推荐 λ 取值

你现在写的是：

```math
\lambda = 0.1
```

👉 含义：

* 10小时 → 权重 ≈ 0.37
* 24小时 → 权重 ≈ 0.09

👉 **衰减很快（偏短期兴趣）**

---

## 如果你想更稳一点（推荐）

```math
\lambda = 0.03
```

效果：

* 24小时 → 0.48
* 3天 → 0.17

👉 更符合“笔记类产品”（不像短视频那么快）

---

# 五、一个更工程化的写法（建议你论文写这个）

可以稍微改写一下公式，让它更“系统化”：

```math
\mathrm{tag\_score}(t)
=
\Big(
1 \cdot view
+ 3 \cdot like
+ 7 \cdot collect
\Big)
\times
\exp\Big(-\lambda \cdot (now - last\_action\_time)\Big)
```

---

# 六、你现在方案的一个隐患（我帮你提前指出）

👉 你现在用的是：

```text
last_action_time（最后一次行为）
```

问题：

👉 **会丢失“历史兴趣”**

比如：

* 用户一个月前疯狂看 C++
* 最近点了一次 Java

👉 Java 会直接把 C++ 干掉（因为时间更新）

---

# 七、如果你想优化（进阶方案）

可以换成：

## ✅ “加权时间衰减”模型

不只存一个时间，而是：

👉 每次行为都影响 score，而不是覆盖

简化版（不用改表结构）：

```math
score = score * decay + new_action_weight
```

👉 但这个需要你：

* 定时衰减（或实时计算）
* 稍微复杂一点

---

# 八、结论（你该怎么做）

## 最简单可落地版本（推荐你现在用）

* 使用 `user_tag_stat`
* 使用 `last_action_time`
* Δt = 当前时间 - last_action_time
* 用指数衰减

👉 优点：

* 简单
* 好实现（SQL就能搞）
* 论文容易写

---

## 一句话总结

👉 **Δt 本质就是：用户对这个标签“多久没兴趣了”**

---

如果你下一步想继续优化，我可以帮你把这一套升级成：

👉 **“带长期兴趣 + 短期兴趣融合”的推荐模型（类似抖音/小红书那种）**

这个会直接让你毕业设计从“合格”→“优秀”。

````



#### 我问这问题显得我像个傻子

````markdown title:""
不对，而且**这么做反而会把系统设计复杂化，还容易出错**。

你现在这个推荐模型，本质是：

👉 **“基于用户当前兴趣标签 → 去召回笔记”**

它是一个**无状态（stateless）推荐**，不是那种必须依赖“上一轮结果”的系统。

---

# 一、为什么不需要记录“上次推荐列表”

你的公式里用到的只有：

* 用户行为（view / like / collect）
* 标签统计（user_tag_stat）
* 搜索记录
* 笔记时间

👉 **没有任何一项依赖“上一次推荐了什么”**

所以：

```text
当前推荐 = f(当前用户兴趣)
```

而不是：

```text
当前推荐 = f(上一次推荐)
```

---

# 二、什么时候“必须记录推荐列表”？

只有以下几种情况才需要：

## ✅ 1. 避免重复推荐（短期去重）

比如：

> 刷推荐流，不想连续看到一样的内容

👉 这时候你可以记录：

```sql
user_recent_recommend
```

但注意：

👉 **这是优化体验，不是算法必须**

---

## ✅ 2. 做曝光 / 点击率（CTR）分析

比如：

* 推荐了100条
* 用户点了10条

👉 用于：

* 调整权重（1 / 3 / 7）
* 做AB测试

---

## ✅ 3. 做“探索 vs 利用”（进阶推荐系统）

比如：

* 80% 推兴趣
* 20% 推新内容

👉 这时候需要知道“用户看过什么”

---

# 三、你现在真正需要的不是“推荐记录”，而是这个👇

👉 **“用户看过什么”**

也就是你已经在做的：

```sql
user_action
```

---

# 四、如果你不做任何处理，会出现什么问题？

👉 用户可能会：

* 反复看到同一批笔记（尤其是Top50）

---

# 五、推荐你加一个“轻量去重机制”（够用就好）

不用复杂表，直接在查询时过滤：

## 方法1：基于行为去重（推荐）

```sql
WHERE note.id NOT IN (
    SELECT note_id FROM user_action
    WHERE user_id = ?
    AND created_at > NOW() - INTERVAL 3 DAY
)
```

👉 含义：

* 最近3天看过的，不再推荐

---

## 方法2：只过滤“已点赞/收藏”

```sql
WHERE note.id NOT IN (
    SELECT note_id FROM note_like WHERE user_id = ?
)
```

👉 更简单，但效果弱一点

---

# 六、如果你执意想记录“推荐列表”（不太建议）

可以这么设计：

```sql
CREATE TABLE user_recommend_log (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT,
    note_id BIGINT,
    recommended_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    INDEX idx_user_time (user_id, recommended_at)
);
```

但我建议：

👉 **毕业设计阶段，不要上这个**

原因：

* 复杂度上升
* 你现在的模型用不到
* 老师也看不出明显价值

---

# 七、最合理的结论

👉 ❌ 不需要记录“上次推荐列表”来计算这次推荐
👉 ✅ 需要记录“用户行为”来更新兴趣
👉 ⚠️ 可以做“去重”优化体验（可选）

---

# 一句话总结

👉 **推荐系统核心是“记住用户做了什么”，而不是“记住你推了什么”**

---

如果你下一步想优化，我可以帮你设计一个：

👉 **“不重复 + 有新鲜感”的推荐策略（只用SQL就能实现）**

这个在答辩时会非常加分。

````






