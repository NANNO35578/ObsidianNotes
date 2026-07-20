
````markdown title:"Work Begin"
当前笔记系统的推荐算法设计如下, 需要设计相关API.
## 推荐算法
#### 一、新用户冷启动推荐算法
$$
\begin{aligned}
\text{基础标签集合 } S_{\text{base}} &= \text{Top3(用户笔记标签频率)} \cup \text{Top2(用户最新标签)} \\
\text{搜索标签集合 } S_{\text{search}} &= \text{Top2(搜索结果标签频率)} \\
\text{热门标签集合 } S_{\text{hot}} &= \text{Top3(全局热门标签)} \\
\text{最终推荐标签集合 } S_{\text{rec}} &= S_{\text{base}} \cup S_{\text{search}} \cup S_{\text{hot}} \\
\text{推荐笔记列表 } N_{\text{rec}} &= \text{Top50}\left( \text{公开笔记}, \text{标签} \in S_{\text{rec}}, \text{创建时间降序} \right)
\end{aligned}
$$

#### 二、老用户标签兴趣评分算法
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
## 数据库相关表设计

```sql
--
-- Table structure for table `note`
--

DROP TABLE IF EXISTS `note`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `note` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `user_id` bigint(20) NOT NULL,
  `title` varchar(255) DEFAULT NULL,
  `content` longtext DEFAULT NULL,
  `summary` text DEFAULT NULL,
  `folder_id` bigint(20) DEFAULT NULL,
  `is_deleted` tinyint(4) DEFAULT 0,
  `updated_at` datetime DEFAULT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  `is_public` tinyint(4) DEFAULT 0,
  `view_count` int(11) DEFAULT 0,
  `like_count` int(11) DEFAULT 0,
  `collect_count` int(11) DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`),
  FULLTEXT KEY `ft_content` (`title`,`content`),
  CONSTRAINT `note_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=123 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `note_collect`
--

DROP TABLE IF EXISTS `note_collect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `note_collect` (
  `user_id` bigint(20) NOT NULL,
  `note_id` bigint(20) NOT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`user_id`,`note_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `note_like`
--

DROP TABLE IF EXISTS `note_like`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `note_like` (
  `user_id` bigint(20) NOT NULL,
  `note_id` bigint(20) NOT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`user_id`,`note_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `note_tag`
--

DROP TABLE IF EXISTS `note_tag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `note_tag` (
  `note_id` bigint(20) NOT NULL,
  `tag_id` bigint(20) NOT NULL,
  PRIMARY KEY (`note_id`,`tag_id`),
  KEY `tag_id` (`tag_id`),
  CONSTRAINT `note_tag_ibfk_1` FOREIGN KEY (`note_id`) REFERENCES `note` (`id`),
  CONSTRAINT `note_tag_ibfk_2` FOREIGN KEY (`tag_id`) REFERENCES `tag` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `search_history`
--

DROP TABLE IF EXISTS `search_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `search_history` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `user_id` bigint(20) NOT NULL,
  `query` varchar(255) DEFAULT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `idx_user_time` (`user_id`,`created_at`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tag`
--

DROP TABLE IF EXISTS `tag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `tag` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_tag_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=2631 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `user_action`
--

DROP TABLE IF EXISTS `user_action`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `user_action` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `user_id` bigint(20) NOT NULL,
  `note_id` bigint(20) NOT NULL,
  `action_type` tinyint(4) NOT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `idx_user` (`user_id`),
  KEY `idx_note` (`note_id`),
  KEY `idx_action` (`action_type`),
  CONSTRAINT `user_action_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`),
  CONSTRAINT `user_action_ibfk_2` FOREIGN KEY (`note_id`) REFERENCES `note` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_tag_stat`
--

DROP TABLE IF EXISTS `user_tag_stat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `user_tag_stat` (
  `user_id` bigint(20) NOT NULL,
  `tag_id` bigint(20) NOT NULL,
  `view_count` int(11) DEFAULT 0,
  `like_count` int(11) DEFAULT 0,
  `collect_count` int(11) DEFAULT 0,
  `last_action_time` datetime DEFAULT NULL,
  PRIMARY KEY (`user_id`,`tag_id`),
  KEY `idx_user_score` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

```

## 需求
1. 请设计推荐系统相关的后端API. 并给出大致实现逻辑.
	例如: `/api/note/like POST ...`
2. 根据算法, 需要记录当前已经为用户推荐了几次, 这个记录可以考虑存放在user表加一个字段即可吗.
3. 可根据ES获取hot标签
````
## GPT ANS
````markdown title:""
你的算法设计本身是清晰的，但**API设计如果只是“围绕推荐结果返回”会不够用**——推荐系统的核心不是“查”，而是“持续采集行为 + 增量更新特征 + 服务推荐结果”。
所以 API 必须分成三层：**行为采集层 / 特征层 / 推荐服务层**，否则后面一定会乱。

---

# 一、推荐系统 API 设计

## （1）推荐结果接口（核心接口）

### ✅ 获取推荐笔记

```http
GET /api/recommend/notes
```

### Query

```json
{
  "page": 1,
  "size": 20
}
```

### Response

```json
{
  "notes": [...],
  "tags": ["C++", "Linux", "算法"],
  "has_more": true
}
```

### 实现逻辑

```text
1. 判断用户是否为新用户
   - user_tag_stat 是否为空
   - 或行为数 < 阈值（如 < 20）

2. if 新用户：
      走冷启动算法
   else：
      走兴趣评分算法

3. 得到 S_rec / S_user

4. ES 查询：
   - tag in S
   - is_public = true
   - sort: created_at desc
   - size = 50

5. 返回分页结果
```

👉 注意：**推荐一定要走 ES，不要走 MySQL**

---

## （2）推荐标签接口（可选但建议）

```http
GET /api/recommend/tags
```

返回：

```json
{
  "tags": ["C++", "后端", "MySQL"]
}
```

### 用途

* 前端展示“猜你感兴趣”
* 调试推荐系统

---

## （3）行为上报接口（非常关键）

你现在有 `user_action`，但缺 API，会导致推荐系统“没数据”。

---

### ✅ 浏览笔记（曝光 + 点击）

```http
POST /api/note/view
```

```json
{
  "note_id": 123
}
```

### 逻辑

```text
1. 写 user_action (view)
2. note.view_count++
3. 更新 user_tag_stat:
   - 找 note 对应 tag
   - view_count++
   - last_action_time = now
```

---

### ✅ 点赞

```http
POST /api/note/like
```

### 逻辑

```text
1. 写 note_like
2. 写 user_action (like)
3. note.like_count++
4. 更新 user_tag_stat.like_count
```

---

### ✅ 收藏

```http
POST /api/note/collect
```

逻辑同上（权重最高）

---

### ❗关键点

👉 **user_tag_stat 是推荐系统的核心表**
👉 所有行为都必须实时更新它（或异步更新）

---

## （4）搜索接口（影响推荐）

```http
GET /api/search
```

```json
{
  "query": "C++ 并发"
}
```

### 逻辑

```text
1. ES 搜索笔记
2. 记录 search_history
3. 统计结果中的 tag（Top2）
4. 可缓存到 Redis（推荐用）
```

---

## （5）热门标签接口（ES）

```http
GET /api/tag/hot
```

### 实现

ES aggregation：

```json
{
  "size": 0,
  "aggs": {
    "hot_tags": {
      "terms": {
        "field": "tags.keyword",
        "size": 10
      }
    }
  }
}
```

---

# 二、“推荐次数”这个设计，建议你别这么做

你问：

> 是否在 user 表加一个字段记录推荐次数？

👉 **不太行，原因如下：**

### ❌ 问题

1. 推荐是“会话级”的，不是“用户级”的
2. 你无法区分：

   * 今天第3次推荐
   * 昨天第3次推荐
3. 无法支持：

   * AB测试
   * 刷新推荐
   * 多策略切换

---

## ✅ 正确做法（推荐）

### 方案1：不记录（最简单）

直接用：

```text
是否新用户 = user_tag_stat 是否为空
```

👉 够用了（你现在这个阶段）

---

### 方案2：记录行为数量（更合理）

在 user 表：

```sql
action_count int
```

判断：

```text
action_count < 20 → 新用户
```

👉 比“推荐次数”靠谱很多

---

### 方案3（进阶）：推荐日志表

```sql
recommend_log
```

```sql
user_id
tags_used
created_at
```

👉 用于：

* debug 推荐效果
* A/B测试

---

# 三、核心流程总结（你后端要实现的）

## 推荐链路

```text
用户打开首页
    ↓
/api/recommend/notes
    ↓
判断用户类型
    ↓
计算标签集合 S
    ↓
ES 查询
    ↓
返回结果
```

---

## 数据流（非常重要）

```text
用户行为（view / like / collect）
    ↓
user_action
    ↓
user_tag_stat（实时更新）
    ↓
推荐算法读取
    ↓
推荐结果
```

---

# 四、你当前设计的优点 & 可以优化的点

## 👍 优点

* 已经有 user_tag_stat（非常关键）
* 行为权重设计合理（1/3/7）
* 有时间衰减（不错）
* 搜索融入推荐（加分项）

---

## ⚠️ 可以优化

### 1. C(t) 这个项

你写了：

```text
× C(t)
```

👉 但你现在数据库**没有这个数据来源**

建议：

```text
先删掉 C(t)
```

否则实现会很别扭

---

### 2. Δt 粒度问题

你现在：

```text
last_action_time
```

👉 OK，但注意：

```text
Δt = now - last_action_time
```

可以直接算，不需要额外字段

---

### 3. ES字段设计建议

note index 增加：

```json
"tags": {
  "type": "keyword"
}
```

否则你没法高效 filter

---

# 五、最终给你一个“最小可落地版本”

你现在只需要实现：

### 必做 API

* ✅ `/api/recommend/notes`
* ✅ `/api/note/view`
* ✅ `/api/note/like`
* ✅ `/api/note/collect`
* ✅ `/api/search`

### 可选

* `/api/recommend/tags`
* `/api/tag/hot`

---

# 如果你下一步要继续做

我可以帮你直接给出：

* Drogon C++ 推荐接口代码
* ES 查询 DSL（完整）
* user_tag_stat 更新 SQL（高效写法）
* 标签去重（C++ / cpp / C plus plus）

直接说你下一步想落哪一块。

````



# 新增API列表 PROMPT

- note相关. 行为上报
#### Prompt
````markdown title:"Prompt"
你是一名资深的drogon后端restful API开发人员. 你需要新增一些API的实现.

### 需要新增的API列表

| 接口                   | 方法     | 说明   | 参数      | 实现代码文件                                  |
| -------------------- | ------ | ---- | ------- | --------------------------------------- |
| `/api/note/view`     | POST   | 浏览笔记 | note_id | `./calcite/controllers/NoteContoller.*` |
| `/api/note/like`     | POST   | 点赞笔记 | note_id | `./calcite/controllers/NoteContoller.*` |
| `/api/note/collect`  | POST   | 收藏笔记 | note_id | `./calcite/controllers/NoteContoller.*` |
| `/api/notes/like`    | DELETE | 取消点赞 | note_id | `./calcite/controllers/NoteContoller.*` |
| `/api/notes/collect` | DELETE | 取消收藏 | note_id | `./calcite/controllers/NoteContoller.*` |


### 实现细节
#### `/api/note/view` POST 浏览笔记
1. 写 user_action (view)
2. note.view_count++
3. 更新 user_tag_stat:
   - 找 note 对应 tag
   - view_count++
   - last_action_time = now

#### `/api/note/like` POST 点赞笔记
1. 写 note_like
2. 写 user_action (like)
3. note.like_count++
4. 更新 user_tag_stat.like_count

#### `/api/note/collect` POST 收藏笔记
逻辑同上（权重最高）

#### 取消点赞/收藏
1. 删除note_like与note_collect的相应行数据
2. 更新user_tag_stat表数据
3. 更新note表数据

### 参考文档
1. `./docs/api.md` API文档
2. `./docs/schema.md` 数据库表定义
3. model类已经在`./calcite/models`中生成好了, 可直接使用

### 输出
1. 完成API实现后, 更新`./docs/api.md` API文档
2. 将你的操作在./docs中形成以260418_为前缀的文档

#### 要求
1. 尽量不修改`./calcite/controllers/NoteContoller.*`和`./docs/api.md`以外的文件
2. 编写API文档时仿照已有API格式编写
3. 你自己列好TODO, 按照TODO处理任务.
````

#### ANS
````markdown title:"还可以"
所有任务已完成，以下是工作总结：

---

### ✅ 已完成的 API 实现

| 接口 | 方法 | 状态 |
|---|---|---|
| `/api/note/view` | POST | ✅ 浏览笔记 |
| `/api/note/like` | POST | ✅ 点赞笔记 |
| `/api/note/collect` | POST | ✅ 收藏笔记 |
| `/api/notes/like` | DELETE | ✅ 取消点赞 |
| `/api/notes/collect` | DELETE | ✅ 取消收藏 |

### 📝 修改的文件

1. **`calcite/controllers/NoteController.h`** — 注册 5 条新路由、声明 5 个公有 API 方法及 2 个私有辅助方法。
2. **`calcite/controllers/NoteController.cc`** — 完整实现上述 API，核心逻辑包括：
   - 使用 `INSERT IGNORE` / `DELETE ... WHERE` 保证点赞/收藏的幂等性
   - 使用原子 SQL (`view_count + 1` / `GREATEST(like_count - 1, 0)`) 避免并发竞争
   - 异步查询 `note_tag` 后批量更新 `user_tag_stat`（`INSERT ... ON DUPLICATE KEY UPDATE`）
   - 辅助行为表 `user_action` 写入（`action_type`: 1=view, 2=like, 3=collect）
3. **`docs/api.md`** — 仿照已有格式补充了 2.9 ~ 2.13 的完整接口文档。
4. **`docs/260418_NoteActionAPI.md`** — 操作记录文档，涵盖实现细节、设计考量与编译验证说明。

### 🔨 编译验证

已在 `calcite/build` 下执行 `cmake .. && make -j4`，编译通过，无报错。
````


#### 测试

public note ids:124~140

- [x] `/api/note/view`     POST 浏览笔记 note_id 
- [x] `/api/note/like`     POST 点赞笔记 note_id 
- [x] `/api/note/collect`  POST 收藏笔记 note_id 
- [x] `/api/notes/like`    DELETE 取消点赞 note_id 
- [x] `/api/notes/collect` DELETE 取消收藏 note_id 

----
# 修改已有API
## 搜索需要写入库
```http
GET /api/search
```

```json
{
  "query": "C++ 并发"
}
```

### 逻辑

```text
1. ES 搜索笔记
2. 记录 search_history
3. 统计结果中的 tag（Top2）
4. 可缓存到 Redis（推荐用）
```


## 




----

# 推荐核心


````markdown


你是一名资深的drogon后端restful API开发人员. 你需要新增推荐API的实现.
部分计算方法需要用到`./calcite/utils/EsClient.*`.

### 推荐算法如下

#### 一、新用户冷启动推荐算法
$$
\begin{aligned}
\text{基础标签集合 } S_{\text{base}} &= \text{Top3(用户笔记标签频率)} \cup \text{Top2(用户最新标签)} \\
\text{搜索标签集合 } S_{\text{search}} &= \text{Top2(搜索结果标签频率)} \\
\text{热门标签集合 } S_{\text{hot}} &= \text{Top3(全局热门标签)} \\
\text{最终推荐标签集合 } S_{\text{rec}} &= S_{\text{base}} \cup S_{\text{search}} \cup S_{\text{hot}} \\
\text{推荐笔记列表 } N_{\text{rec}} &= \text{Top50}\left( \text{公开笔记}, \text{标签} \in S_{\text{rec}}, \text{创建时间降序} \right)
\end{aligned}
$$

#### 二、老用户标签兴趣评分算法
$$
\begin{aligned}
% 1. 行为加权标签得分（含行为权重、出现次数、时间衰减）
\mathrm{tag\_score}(t)
&= \Big(1\cdot N_{\text{view}}(t) + 3\cdot N_{\text{like}}(t) + 7\cdot N_{\text{collect}}(t)\Big)
\\
&\quad 
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

\Delta t             &: \text{用户最近一次与该标签交互的时间差} \\
S_{\text{behavior}}  &: \text{用户行为计算得到的兴趣标签集合} \\
S_{\text{search}}    &: \text{最近搜索结果中出现最多的2个标签} \\
S_{\text{user}}      &: \text{融合搜索意图后的最终用户兴趣标签集合}
\end{aligned}
$$

### 需要新增的API列表

| 接口                     | 方法  | 说明     | 创建文件                                                                                        |
| ---------------------- | --- | ------ | ------------------------------------------------------------------------------------------- |
| `/api/recommend/notes` | GET | 获取推荐笔记 | `./calcite/controllers/RecommendContoller.h`, `./calcite/controllers/RecommendContoller.cc` |

- 如果你觉得EsClient中实现的搜索方法不足当前需求, 可自行拓展出一个*自定义查询函数*添加到EsClient的代码中

### 实现细节
#### `/api/recommend/notes` GET 获取推荐笔记
1. 判断用户是否为新用户
   - user_action 表行为数 < 20行
1. if 新用户：
      走冷启动算法
   else：
      走兴趣评分算法
2. 得到 S_rec / S_user
3. ES 查询：
   - tag in S
   - is_public = true
   - sort: created_at desc
   - size = 10
5. 返回分页结果

#### 冷启动算法各个表达式计算方式

除了$S_{\text{search}}$ 外, 所有表达式均可通过ElasticSearch的EsClient来获取.
- 如果你觉得EsClient中实现的搜索方法不足当前需求, 可自行拓展出一个*自定义查询函数*添加到EsClient的代码中
$S_{\text{search}}$ : 查询`search_history`表, 使用下面SQL:
```sql
SELECT query, COUNT(*) AS freq
 FROM calcite.search_history WHERE user_id = ? 
 GROUP BY query 
 ORDER BY freq DESC 
 LIMIT 2;
```


#### 兴趣评分算法各个表达式计算方式

$N_{\text{view}}(t)$, $N_{\text{like}}(t)$, $N_{\text{collect}}(t)$ : 均可通过查询`user_tag_stat`表获取.
$\Delta t$ : now - `user_tag_stat.last_action_time`
$S_{\text{search}}$ : 查询`search_history`表, 与冷启动算法采用相同sql.


### 参考文档
1. `./docs/api.md` API文档
2. `./docs/schema.md` 数据库表定义, 例如`search_history`表, `user_tag_stat`表
3. model类已经在`./calcite/models`中生成好了, 可直接使用

### 输出
1. 完成API实现后, 更新`./docs/api.md` API文档
2. 将你的操作在./docs中形成以260419_为前缀的文档

#### 要求
1. 编写API文档时仿照已有API格式编写
2. 你自己列好TODO, 按照TODO处理任务.

````


# 老大哥啊

你是一名资深的drogon后端restful API开发人员. 你需要新增推荐API的实现.
部分计算方法需要用到`./calcite/utils/EsClient.*`.

### 实现目标
新增推荐API：`GET /api/recommend/notes`

支持分页推荐笔记列表。

---

### 请求参数
- page (int, 默认1)
- page_size (int, 默认10, 最大50)

---

### 推荐逻辑

#### 1. 用户分类
新用户：最近30天 user_action 行为数 < 20
否则为老用户

#### 2. 标签集合计算

##### 新用户（冷启动） $S_{\text{rec}}$

$S_{\text{base}}$: $\text{Top3(用户笔记使用的标签)} \cup \text{Top2(用户最新标签)}$
- 可查询ES获取.

$S_{\text{search}}$:
- 从 `search_history` 获取 Top2 query
- query 直接作为 tag 使用
- 使用这个sql: `SELECT query, COUNT(*) AS freq  FROM calcite.search_history WHERE user_id = ?  GROUP BY query  ORDER BY freq DESC  LIMIT 2;`

$S_{\text{hot}}$: 全局热门标签 Top3（ES 获取）

$S_{\text{rec}} = S_{\text{base}} \cup S_{\text{search}} \cup S_{\text{hot}}$

##### 老用户（兴趣模型） $S_{\text{user}}$

从 `user_tag_stat` 表获取：
${tag\_score}(t)= \Big(1\cdot N_{\text{view}}(t) + 3\cdot N_{\text{like}}(t) + 7\cdot N_{\text{collect}}(t)\Big) \times e^{-\lambda \Delta t}, \quad \lambda=0.1$

说明：
$\Delta t$ = 当前时间 - last_action_time（单位：天）
$S_{\text{behavior}}$: 按 score 排序取 TopK（建议K=5）
$S_{\text{search}}$: 同冷启动

$S_{\text{user}} = S_{\text{behavior}} \cup S_{\text{search}} \cup S_{\text{hot}}$

---

### 3. ES 查询

条件：
- terms: tags ∈ S
- is_public = true

排序：
- created_at desc

分页：
- `from = (page-1)*page_size`
- `size = page_size`

---

### 4. EsClient 扩展

如EsClient中现有方法不足，新增：
- 按标签查询 notes
- 获取热门标签
- 标签聚合统计

---

### 5. 输出要求

1. 实现： `RecommendController.h / .cc`
2. 更新：`./docs/api.md`
3. 新增文档：`./docs/260419_recommend_api.md`

---

### 参考文档
1. `./docs/api.md` API文档
2. `./docs/schema.md` 数据库表定义, 例如`search_history`表, `user_tag_stat`表
3. `./calcite/utils/EsClient.*` EsClient的实现
4. model类已经在`./calcite/models`中生成好了, 可直接使用

### 6. 强制执行步骤（必须按顺序）

TODO:
1. 阅读 ./docs/schema.md 确认表结构
2. 检查 EsClient 能力
3. 设计缺失接口
4. 实现标签计算逻辑
5. 实现 ES 查询
6. 实现分页
7. 添加兜底逻辑
8. 编写 API 文档
9. 编写设计文档

# Kimi Recommend Ans

````markdown title:""
推荐 API 已全部实现完毕，编译通过。以下是完成内容汇总：

---

### 1. EsClient 扩展 (`calcite/utils/EsClient.*`)

新增两个同步方法：

| 方法 | 功能 |
|---|---|
| `searchByTagsSync` | 按标签列表查询公开笔记，支持分页与排序（`created_at desc`）；**标签为空时自动查询所有公开笔记**，直接作为兜底查询复用 |
| `getHotTagsSync` | 基于 ES 聚合查询近 7 天公开笔记的 `tags` 字段，返回 TopN 热门标签 |

---

### 2. 推荐控制器 (`calcite/controllers/RecommendController.*`)

- **接口**：`GET /api/recommend/notes`
- **参数**：`page`（默认1）、`page_size`（默认10，最大50）
- **新老用户判定**：最近30天 `user_action` 行为数 < 20 为新用户

**标签计算流程：**

| 用户类型 | 标签来源 | SQL/ES 说明 |
|---|---|---|
| 新用户 | `S_base` | SQL 联合 `note_tag`+`tag`+`note` 取 **Top3 常用标签** 与 **Top2 最新标签** |
| 新用户 | `S_search` | SQL `search_history` 按频率取 Top2 query |
| 新用户 | `S_hot` | ES `getHotTagsSync(3)` |
| 老用户 | `S_behavior` | SQL `user_tag_stat` JOIN `tag`，按 `(1×view + 3×like + 7×collect) × e^(-0.1×Δt)` 排序取 Top5 |
| 老用户 | `S_search` | 同上 |
| 老用户 | `S_hot` | 同上 |

**兜底逻辑**：若标签集合为空，或 ES `terms` 查询无结果，则自动降级为查询所有公开笔记并按时间倒序返回。

---

### 3. 文档更新

- **`docs/api.md`**：在 API 总览和笔记管理章节中新增 `2.14 推荐笔记` 接口文档
- **`docs/260419_recommend_api.md`**：完整设计文档，包含算法公式、SQL 实现、流程图、测试示例与后续优化建议

---

### 4. 编译验证

```bash
cd calcite/build && make -j4
# [100%] Built target calcite
# [100%] Built target calcite_test
```

编译通过，无错误。
````

----



# 查询API


你是一名资深的drogon后端restful API开发人员. 你需要新增一些API的实现.

### 需要新增的API列表

| 接口                      | 方法  | 说明        |
| ----------------------- | --- | --------- |
| `/api/user/likes`       | GET | 获取已点赞笔记列表 |
| `/api/user/collections` | GET | 获取已收藏笔记列表 |


### 实现细节
#### `/api/user/likes` GET 获取已点赞笔记列表
1. 写 user_action (view)
2. note.view_count++
3. 更新 user_tag_stat:
   - 找 note 对应 tag
   - view_count++
   - last_action_time = now

#### `/api/note/like` POST 点赞笔记
1. 写 note_like
2. 写 user_action (like)
3. note.like_count++
4. 更新 user_tag_stat.like_count

#### `/api/note/collect` POST 收藏笔记
逻辑同上（权重最高）

#### 取消点赞/收藏
1. 删除note_like与note_collect的相应行数据
2. 更新user_tag_stat表数据
3. 更新note表数据

### 参考文档
1. `./docs/api.md` API文档
2. `./docs/schema.md` 数据库表定义
3. model类已经在`./calcite/models`中生成好了, 可直接使用

### 输出
1. 完成API实现后, 更新`./docs/api.md` API文档
2. 将你的操作在./docs中形成以260418_为前缀的文档

#### 要求
1. 尽量不修改`./calcite/controllers/NoteContoller.*`和`./docs/api.md`以外的文件
2. 编写API文档时仿照已有API格式编写
3. 你自己列好TODO, 按照TODO处理任务.


