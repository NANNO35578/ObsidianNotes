# 开发周志 —— 第7周：数据库物理设计与 ES 索引设计

## 本周 TODO
- [ ] 完成 MariaDB 物理建库脚本与初始化数据
- [ ] 设计数据库索引策略（主键、外键、普通索引、全文索引）
- [ ] 设计 Elasticsearch 索引结构（notes 索引、mapping、IK 分词器）
- [ ] 编写 schema.md 与 schema_new_dump.sql
- [ ] 后端使用 drogon_ctl 生成 ORM 模型类

## 工作内容概括

本周将逻辑模型落地为物理实现。MariaDB 侧完成 `calcite` 数据库创建（utf8mb4），执行 13 张表的建表脚本，补充外键约束与联合主键。针对高频查询场景添加索引：
- `note` 表：`FULLTEXT KEY ft_content (title, content)` 支持 MariaDB 侧备用检索；
- `search_history` 表：`idx_user_time (user_id, created_at)`；
- `user_action` 表：`idx_user`、`idx_note`、`idx_action` 三索引支撑行为统计；
- `user_tag_stat` 表：`idx_user_score (user_id)` 支撑推荐查询。

Elasticsearch 侧设计 `notes` 索引，mapping 中包含 `title`、`content`、`summary`、`tags`、`user_id`、`is_public` 等字段，并为中文内容配置 `ik_max_word` 与 `ik_smart` 两种分词模式。后端通过 `drogon_ctl create model .` 自动生成 ORM 模型类，禁止手写，确保与表结构严格同步。

## 关键产出
- schema.md（数据库设计文档）
- schema_new_dump.sql（完整建库脚本）
- ES notes 索引 mapping 定义
- Drogon ORM 模型类（models/ 目录）

---

## 工作记录（精简版）

### 一、MariaDB 物理设计

```sql
CREATE DATABASE IF NOT EXISTS calcite CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

**索引策略汇总**：

| 表 | 索引 | 用途 |
|----|------|------|
| `note` | `FULLTEXT KEY ft_content(title, content)` | MariaDB 备用全文检索 |
| `search_history` | `idx_user_time(user_id, created_at)` | 用户搜索历史时序查询 |
| `user_action` | `idx_user`, `idx_note`, `idx_action` | 行为统计与推荐计算 |
| `user_tag_stat` | `idx_user_score(user_id)` | 推荐系统 Top N 查询 |
| `note_folder` | `idx_parent(parent_id)` | 树形子文件夹查询 |

**字符集**：统一 `utf8mb4`，支持 Emoji 与生僻字；排序规则 `utf8mb4_unicode_ci`。

### 二、Elasticsearch 索引设计

索引名：`notes`

```json
{
  "mappings": {
    "properties": {
      "title": { "type": "text", "analyzer": "ik_max_word", "search_analyzer": "ik_smart" },
      "content": { "type": "text", "analyzer": "ik_max_word" },
      "summary": { "type": "text", "analyzer": "ik_max_word" },
      "tags": { "type": "keyword" },
      "user_id": { "type": "long" },
      "is_public": { "type": "boolean" },
      "created_at": { "type": "date" }
    }
  }
}
```

**关键决策**：
- `title` 使用 `ik_max_word` 索引、`ik_smart` 搜索，兼顾召回率与精确度
- `tags` 为 `keyword` 类型，不参与分词，用于精确过滤与聚合（热门标签）
- 笔记删除时同步删除 ES 文档，软删除笔记通过 `is_public=false` + 用户 ID 过滤隔离

### 三、ORM 模型生成

```bash
cd calcite_server/calcite
# 配置 orm_config.json 指向 MariaDB
# 执行生成
drogon_ctl create model . 
```

生成 `models/` 目录下 13 个模型类（如 `User.h`、`Note.h`、`FileResource.h`），每个类包含：
- 与表字段一一对应的成员变量
- `insert()` / `update()` / `deleteByPrimaryKey()` 等 ORM 方法
- `findByPrimaryKey()` / `findBy` 等查询方法

**约束**：禁止手写 Model 类，表结构变更后重新生成，确保代码与 DB 严格同步。

### 四、物理脚本产出

| 文件 | 说明 |
|------|------|
| `schema.md` | 数据库设计文档，含 13 张表的 CREATE TABLE 与字段说明 |
| `schema_new_dump.sql` | 完整建库脚本，可直接在 MariaDB 执行初始化 |
| `insertTag.sql` | 标签种子数据（如「算法」「前端」「读书笔记」等初始标签） |

### 五、本周问题

- **ES mapping 与 DB 字段不一致风险**：DB 新增字段后，ES mapping 需手动更新。暂时通过代码层面控制写入字段列表，避免向 ES 写入未定义字段导致报错。后续可考虑动态 mapping，但需防范字段爆炸。
- **drogon_ctl 生成模型后编译失败**：生成的 `FileResource.h` 中 `status` 字段为 ENUM 类型，Drogon ORM 将其映射为 `std::string`，需在 Service 层手动校验合法值（`processing`/`done`/`failed`）。

### 六、会议备忘

自查结论：MariaDB 脚本已在本地与实验室服务器双环境验证通过；ES 索引创建成功，IK 分词器中文测试通过（「机器学习」被切分为「机器」「学习」）；ORM 模型类编译无报错，下一步可进入 Controller 开发。
