# 笔记API
## 现有API:

| 接口               | 方法   | 说明     |
| ---------------- | ---- | ------ |
| /api/note/create | POST | 新建笔记   |
| /api/note/update | POST | 更新笔记   |
| /api/note/delete | POST | 删除笔记   |
| /api/note/list   | GET  | 获取笔记列表 |
| /api/note/detail | GET  | 获取笔记详情 |
| /api/note/search | GET  | 全文搜索   |

**鉴权要求：** 所有笔记接口均需通过 Token 鉴权

## 预计修改:

| 接口               | 方法   | 说明     | 修改                                         |
| ---------------- | ---- | ------ | ------------------------------------------ |
| /api/note/create | POST | 新建笔记   | :RiCheckboxCircleLine: 减少summary,content参数 |
| /api/note/update | POST | 更新笔记   | :RiCheckboxCircleLine: 新增is_public参数       |
| /api/note/delete | POST | 删除笔记   | :RiCheckboxCircleLine:                     |
| /api/note/list   | GET  | 获取笔记列表 | :RiCheckboxCircleLine: 减少tag参数             |
| /api/note/detail | GET  | 获取笔记详情 | :RiCheckboxCircleLine: 响应增加is_public字段     |
| /api/note/search | GET  | 全文搜索   | :RiCheckboxCircleLine: 新增is_public参数       |


### 2.4 获取笔记列表 GET /api/note/list

**请求方式：**
Header: `Authorization: Bearer {token}`

**请求参数：**

| 参数       | 类型   | 必填 | 说明                                        |
| ---------- | ------ | ------ | -------------------------------------- |
| folder_id | int64  | 否     | 文件夹ID，0表示所有笔记，不传表示未分类笔记       |



> 注：列表响应不包含 `content` 字段以减少网络流量，完整内容需调用 `/api/note/detail` 获取

### 2.2 更新笔记 POST /api/note/update

**请求方式：**
Header: `Authorization: Bearer {token}`

**请求参数：**
| 参数      | 类型   | 必填 | 说明                       |
| --------- | ------ | ------ | -------------------------- |
| note_id   | int64  | 是     | 笔记ID                     |
| title     | string | 否     | 更新标题                   |
| content   | string | 否     | 更新内容                   |
| summary   | string | 否     | 更新摘要                   |
| folder_id | int64  | 否     | 更新文件夹ID，>=0表示更新 |
| is_public | bool   | 否     | 更新笔记是否公开         |


### 2.6 全文搜索笔记 GET /api/note/search

**请求方式：**
Header: `Authorization: Bearer {token}`

**请求参数：**

| 参数      | 类型   | 必填 | 说明                        |
| -------- | ------ | ------ | ----------------------- |
| keyword  | string | 是     | 搜索关键词               |
| isPublic | int    | 否     | 有就搜索公开的             |
| from     | int    | 否     | 分页起始位置，默认0         |
| size     | int    | 否     | 每页数量，默认20，最大100   |


----

# 标签API

对于我的笔记系统的标签管理API, 现有标签API如下:

| 接口              | 方法   | 说明        |
| --------------- | ---- | --------- |
| /api/tag/create | POST | 创建标签      |
| /api/tag/list   | GET  | 获取标签列表    |
| /api/tag/bind   | POST | 绑定/解除笔记标签 |
| /api/tag/update | POST | 更新标签      |
| /api/tag/delete | POST | 删除标签      |

 目前数据库已修改TAG表为如下创建方式. 
 ```sql
 CREATE TABLE `tag` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_tag_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci
 ```
 
 请重新设计标签API, 后端预计通过大模型推荐笔记包含的tag, 同时tag池固定. 
 考虑大模型计费, 还需要分析何时更新笔记的标签.


````markdown title:"说这么多干啥"
你现在这套 API 设计，核心问题是：**它是“用户可编辑标签体系”**，但你已经把系统改成了**“标签池固定 + AI生成标签”**，两者是冲突的。

直说结论：
👉 你这 5 个 API 至少要砍掉一半，否则后面一定混乱（权限、数据一致性、成本都会炸）。

---

# 一、设计目标重新明确

你现在的标签系统本质应该是：

### 1️⃣ 标签是“标准化语义标签”

* 全局唯一（你已经用 UNIQUE name 做到了 👍）
* 不属于用户
* 不允许随意创建（否则 AI 去重白做）

### 2️⃣ 标签来源

* ✅ AI 自动生成（主）
* ✅ 用户可选择已有标签（辅）
* ❌ 用户自定义标签（建议禁止或严格限制）

---

# 二、推荐的 API 设计（精简版）

我直接给你一版更合理的👇

## 🔹 标签基础 API（只读为主）

| 接口            | 方法  | 说明              |
| ------------- | --- | --------------- |
| /api/tags     | GET | 获取标签列表（支持搜索/分页） |
| /api/tags/hot | GET | 热门标签（用于推荐）      |

👉 **删除：**

* ❌ create
* ❌ update
* ❌ delete

> 标签池应该由：
>
> * 离线脚本
> * 管理后台
> * 或 AI + 审核机制
>   来维护，而不是普通 API

---

## 🔹 笔记标签 API（核心）

| 接口                      | 方法   | 说明           |
| ----------------------- | ---- | ------------ |
| /api/notes/{id}/tags    | GET  | 获取笔记标签       |
| /api/notes/{id}/tags    | PUT  | 覆盖标签（用户手动调整） |
| /api/notes/{id}/tags/ai | POST | 触发 AI 生成标签   |

👉 注意：

* 不再需要 `/tag/bind`
* 改为 **资源归属设计（RESTful）**

---

## 🔹 AI 标签生成接口设计

```json
POST /api/notes/{id}/tags/ai

Response:
{
  "tags": ["C++", "并发编程", "STL"]
}
```

---

# 三、关键难点：AI 标签去重（你之前问的重点）

你现在 UNIQUE(name) 只是**物理去重**，不解决语义问题：

```
cpp
C++
c plus plus
```

### 推荐方案（工程可落地）

#### ✅ 方案1：标签标准化表（强烈推荐）

新增一张：

```sql
tag_alias (
  id,
  alias_name,
  tag_id
)
```

例如：

| alias_name  | tag_id |
| ----------- | ------ |
| cpp         | 1      |
| c++         | 1      |
| c plus plus | 1      |

### 流程：

1. AI 输出：`["cpp", "stl"]`
2. 后端处理：

   * 先查 alias 表
   * 找到标准 tag_id
3. 不存在：

   * ❌ 不直接创建（避免污染）
   * ✅ 走“候选标签池”（可选）

---

#### ✅ 方案2：AI 强约束（低成本）

直接在 prompt 里限制：

> “只能从以下标签中选择”

```json
["C++", "Java", "数据库", "操作系统"]
```

👉 优点：

* 成本最低
* 不需要 alias 表

👉 缺点：

* 标签池必须提前准备好

---

#### 🚫 不推荐

* 每次 embedding + 相似度匹配（成本高 + 慢）

---

# 四、AI 调用时机（重点：省钱）

你这个问题问得很好，这里是关键设计👇

---

## ❌ 不要这样做

* 每次修改笔记都调用 AI ❌（费用爆炸）

---

## ✅ 推荐策略（工业级）

### 1️⃣ 创建笔记时

```text
触发 AI（异步）
```

---

### 2️⃣ 内容“显著变化”才触发

比如：

```cpp
if (编辑字数变化 > 30%)
    触发AI
```

或：

```cpp
hash(old_content) != hash(new_content)
```

---

### 3️⃣ 用户手动触发（很重要）

```http
POST /notes/{id}/tags/ai
```

👉 用户点击：

> “重新生成标签”

---

### 4️⃣ 后台批处理（优化推荐系统）

* 定时任务（每天/每周）
* 只处理：

  * 热门笔记
  * 新增笔记

---

### 5️⃣ 加缓存（关键省钱点）

```sql
note_tag_ai_cache
```

存：

* content_hash
* tags

👉 如果内容没变 → 直接复用

---

# 五、数据库补充设计

你应该有这个表（如果还没有）：

```sql
note_tag (
  note_id,
  tag_id,
  source ENUM('ai', 'user'),
  PRIMARY KEY (note_id, tag_id)
)
```

👉 用来区分：

* AI打的标签
* 用户手动加的标签

---

# 六、最终结构总结（给你一个清晰图）

### 标签系统：

```
tag（标准标签池）
tag_alias（别名映射，可选）
```

### 关系：

```
note_tag（绑定关系 + 来源）
```

---

# 七、你当前 API 的处理建议

| 原 API  | 处理   |
| ------ | ---- |
| create | ❌ 删除 |
| list   | ✅ 保留 |
| bind   | ❌ 重构 |
| update | ❌ 删除 |
| delete | ❌ 删除 |

---

# 八、一点现实建议（很关键）

你这个系统如果目标是“推荐 + 搜索”：

👉 标签**宁少不乱**

比起：

```
10000 个脏标签
```

更有价值的是：

```
200 个高质量标签
```

---

如果你下一步要做：

👉 **Elasticsearch + 标签搜索 + 推荐排序**

我可以帮你把：

* tag 如何进 ES
* 如何做 tag boosting
* 如何做“相似笔记推荐”

这块直接给你设计一版工程级方案。

````

## 现有API:



| 接口              | 方法   | 说明        | 修改  |
| --------------- | ---- | --------- | --- |
| /api/tag/create | POST | 创建标签      | 删除  |
| /api/tag/list   | GET  | 获取标签列表    | 删除  |
| /api/tag/bind   | POST | 绑定/解除笔记标签 | 删除  |
| /api/tag/update | POST | 更新标签      | 删除  |
| /api/tag/delete | POST | 删除标签      | 删除  |

## 新增相关API

### 笔记标签 API

| 接口                      | 方法   | 说明         |
| ----------------------- | ---- | ---------- |
| /api/notes/{id}/tags    | GET  | 获取笔记标签     |
| /api/notes/{id}/tags/ai | POST | 触发 AI 生成标签 |

### 获取热门标签

| 接口            | 方法  | 说明   |
| ------------- | --- | ---- |
| /api/tags/hot | GET | 热门标签 |


你是一名资深的后端C++开发人员. 当前项目基于drogon框架.

## 你需要开发如下API:

| 接口                      | 方法   | 说明         | 可修改文件                                 |
| ----------------------- | ---- | ---------- | ------------------------------------- |
| /api/notes/{id}/tags    | GET  | 获取笔记标签     | `./calcite/controllers/NoteContoller` |
| /api/notes/{id}/tags/ai | POST | 触发 AI 生成标签 | `./calcite/controllers/NoteContoller` |
| /api/tags/hot           | GET  | 热门标签       | `./calcite/controllers/TagContoller`  |

删除原有TagController中的API, 现在已经被注释.

## 实现细节
 ### 1. `/api/notes/{id}/tags`
 在./calcite/controllers/NoteContoller中实现. 
 实现细节:  根据./calcite/model/Tag类读取数据库返回给客户端.

 ### 2. `/api/notes/{id}/tags/ai`
 在./calcite/controllers/NoteContoller中实现. 实现细节:
 通过./calcite/Services/DsService中`void recommendTags(const std::string& noteContent, std::function<void(const TagRecommendationResult&)> callback);`函数生成标签.
 将生成的标签写入数据库`note_tag`表. 同样通过./calcite/model/NoteTag实现数据库操作.
 将生成的标签通过./calcite/utils/EsClient的`updateDocument`方法为ES数据添加标签.

 ### 3. `/api/tags/hot`
 在./calcite/controllers/TagContoller中实现. 实现细节:
 通过ES聚合直接获取. 采用以下ES查询方式:
 ```http
 POST /notes/_search
{
  "size": 0,
  "query": {
    "bool": {
      "filter": [
        { "term": { "is_public": true }},
        {
          "range": {
            "created_at": {
              "gte": "now-7d/d"
            }
          }
        }
      ]
    }
  },
  "aggs": {
    "hot_tags": {
      "terms": {
        "field": "tags",
        "size": 10
      }
    }
  }
}
 ```

将结果返回给请求客户端.


## 参考文档
API文档:
./docs/api.md Line:429~589 原有的TAG相关API, 已弃用, 修改完后更新这部分API文档.
./docs/api.md Line:60~381 现有Note相关API, 修改完后你需要新增两个相关的API文档 


## 输出
将你的操作在./docs文件夹中生成以260418_为前缀的文档.
严禁修改除了`./calcite/controllers/NoteContoller`, `./calcite/controllers/TagContoller`以外的文件.

## 其他
忽略其他文件涉及到的错误, 其他错误我会修正, 你只需要完成我给你的任务.
若model类缺少函数(理论上不会缺少), 修改./calcite/models/model.json后, 使用以下命令重新生成model类
```sh
cd ./calcite/models
drogon_ctl create model . # 只能用 . 
```



# 在审一下吧
你是一名资深的后端 C++ 开发工程师，熟练使用 Drogon 框架进行 RESTful API 开发。

## 项目约束（严格遵守，不可违反）
1. 仅允许修改四个个文件：
   - ./calcite/controllers/NoteController.h
   - ./calcite/controllers/NoteController.cc
   - ./calcite/controllers/TagController.h
   - ./calcite/controllers/TagController.cc
1. 严禁修改任何其他文件、目录、配置、model 代码、工具类代码。
2. 忽略项目中其他所有编译/依赖错误，你只完成指定 API 开发任务。
3. 若 Model 类缺少函数，禁止手动修改, **必须**修改./calcite/models/model.json后, 用命令 cd ./calcite/models && drogon_ctl create model . 重新生成。

## 需要开发的 API 清单
| 接口路径                    | 请求方法 | 功能说明         | 实现位置                      |
| ----------------------- | ---- | ------------ | ------------------------- |
| /api/notes/{id}/tags    | GET  | 获取指定笔记的标签    | NoteController            |
| /api/notes/{id}/tags/ai | POST | AI 生成并保存笔记标签 | NoteController            |
| /api/tags/hot           | GET  | 获取热门标签       | TagController（清空原有注释 API） |

## 接口实现细节（严格按照以下逻辑编写）

### 1. GET /api/notes/{id}/tags
- 位置：NoteController
- 逻辑：
  1. 接收路径参数 id（笔记 ID）
  2. 使用 ./calcite/model/NoteTag 类从数据库查询该笔记的标签数据
  3. 以标准 JSON 格式返回给客户端
  4. 处理正常/异常返回

### 2. POST /api/notes/{id}/tags/ai
- 位置：NoteController
- 逻辑：
  1. 接收路径参数 id（笔记 ID）
  2. 调用 ./calcite/Services/DsService 的异步函数：
     void recommendTags(const std::string& noteContent, std::function<void(const TagRecommendationResult&)> callback);
  3. 在回调中获取 AI 推荐标签
  4. 使用 ./calcite/model/Tag 查询标签id.
  5. 使用 ./calcite/model/NoteTag 将笔记id与标签id写入数据库 note_tag 表
  6. 调用 ./calcite/utils/EsClient 的 updateDocument 方法为 ES 文档更新标签
  7. 返回生成成功的标签列表 JSON

### 3. GET /api/tags/hot
- 位置：TagController（删除原有注释的全部旧 API）
- 逻辑：
  1. 直接使用以下 ES 聚合查询获取近 7 天公开笔记的 Top10 热门标签
  2. 解析 ES 返回结果，封装为标准 JSON 返回客户端

#### ES 查询语句（直接使用）
POST /notes/_search
{
  "size": 0,
  "query": {
    "bool": {
      "filter": [
        { "term": { "is_public": true }},
        {
          "range": {
            "created_at": {
              "gte": "now-7d/d"
            }
          }
        }
      ]
    }
  },
  "aggs": {
    "hot_tags": {
      "terms": {
        "field": "tags",
        "size": 10
      }
    }
  }
}

## 文档输出要求
1. 在 ./docs 目录下生成**操作文档**，文件名必须以 260418_ 为前缀
2. 文档内容包含：
   - 三个 API 的完整实现说明
   - 请求/返回示例
   - 依赖类调用说明
3. 同时按照参考文档要求：
   - 更新 ./docs/api.md 429~589 行：废弃旧 TAG API, 更新新的API文档
   - 在 ./docs/api.md 60~381 行后：新增两个笔记标签 API 文档


````markdown title:"kimi又干活了"
所有指定任务已完成，仅修改了以下 4 个代码文件及相关文档：

### 1. 代码变更

**`calcite/controllers/NoteController.h`**
- 新增 `#include "../services/DsService.h"`
- 注册两条路由：
  - `GET /api/notes/{id}/tags` → `getNoteTagsHandler`
  - `POST /api/notes/{id}/tags/ai` → `generateNoteTagsByAi`
- 新增 `services::DsService dsService_` 成员
- 移除旧的无用私有方法 `getNoteTags` 声明

**`calcite/controllers/NoteController.cc`**
- 移除旧的 `getNoteTags` 实现
- 新增 `getNoteTagsHandler`：Token 鉴权 → 校验笔记权限 → `NoteTag` Mapper 查关联 → `Tag` Mapper `In` 查询批量取标签名 → 返回 JSON
- 新增 `generateNoteTagsByAi`：Token 鉴权 → 查笔记内容 → 调用 `DsService::recommendTags` → `Tag` Mapper 按名称查 ID → `NoteTag` Mapper 清除旧关联并逐条插入新关联 → `EsClient::updateDocument` 同步 ES → 返回生成标签列表

**`calcite/controllers/TagController.h`**
- 清空全部旧注释 API
- 注册新路由 `GET /api/tags/hot` → `getHotTags`
- 保留 `AuthService` 及通用辅助方法

**`calcite/controllers/TagController.cc`**
- 清空全部旧注释代码
- 新增 `getHotTags`：Token 鉴权 → 直接向 ES `POST /notes/_search` 执行聚合查询（近 7 天 `is_public=true` 公开笔记的 `tags` Top10）→ 解析 `aggregations.hot_tags.buckets` → 封装标准 JSON 返回

### 2. 文档变更

**新增 `./docs/260418_NoteTag_API.md`**
- 三个 API 的完整实现逻辑
- 请求/返回示例（含正常/异常）
- 依赖类调用链说明（`DsService`、`EsClient`、`NoteTag`、`Tag`、`Note` 等）

**更新 `./docs/api.md`**
- **60~381 行后**：新增 `### 2.7 获取笔记标签列表` 与 `### 2.8 AI 生成笔记标签` 文档
- **429~589 行**：废弃旧 Tag API（`/api/tag/create`、`/api/tag/list` 等），替换为 `### 3.1 获取热门标签 GET /api/tags/hot` 及 ES 聚合说明
- 同步更新了顶部总览表与笔记模块摘要表中的接口列表

> 注：工作目录中 `NoteController.cc` 等文件尚存在其他未提交的先前改动，本次操作仅针对上述四个文件及文档进行了增量修改，未触碰其他任何文件。项目当前因 `EsClient.h` 中默认参数顺序的既有编译错误导致整体构建失败，属于已有问题，已按指示忽略。
````

