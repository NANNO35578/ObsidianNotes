

```sql
ALTER TABLE note ADD COLUMN is_public TINYINT DEFAULT 0;
```

### 3 笔记表 `note`
支持ES全文搜索.`summary` 用于智能摘要

```sql
CREATE TABLE note (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT NOT NULL,
    title VARCHAR(255),
    content LONGTEXT,
    summary TEXT,
    folder_id BIGINT,
    is_deleted TINYINT DEFAULT 0,
    updated_at DATETIME,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FULLTEXT KEY ft_content (title, content),
    FOREIGN KEY (user_id) REFERENCES user(id)
);
```


### 6 标签表 `tag`

```sql
CREATE TABLE tag (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    -- user_id BIGINT NOT NULL,
    name VARCHAR(50),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES user(id)
);
```


### 7 笔记-标签关联表 `note_tag`
支持一对多、多对多关系

```sql
CREATE TABLE note_tag (
    note_id BIGINT NOT NULL,
    tag_id BIGINT NOT NULL,
    PRIMARY KEY (note_id, tag_id),
    FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (tag_id) REFERENCES tag(id)
);
```


# 二、数据库应该怎么改（重点）

## ✅ 1. 新增：用户行为表（核心表）

```sql
CREATE TABLE user_behavior (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    user_id BIGINT NOT NULL,
    note_id BIGINT NOT NULL,

    action ENUM('view', 'like', 'collect', 'search') NOT NULL,
    weight FLOAT DEFAULT 1.0,

    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    INDEX idx_user (user_id),
    INDEX idx_note (note_id),
    INDEX idx_action (action)
);
```

- 需要设置外键

👉 说明：

* view（浏览）权重低
* like（点赞）权重高
* collect（收藏）更高

👉 你可以设计权重：
```
view = 1
like = 3
collect = 5
```

---

## ✅ 2. 新增：笔记统计表（加速推荐）

```sql
CREATE TABLE note_stat (
    note_id BIGINT PRIMARY KEY,
    view_count INT DEFAULT 0,
    like_count INT DEFAULT 0,
    collect_count INT DEFAULT 0,
    score FLOAT DEFAULT 0,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
```

👉 用于：
* 热门推荐
* 排序优化

---

## ✅ 3. （可选）推荐结果缓存

```sql
CREATE TABLE user_recommend_cache (
    user_id BIGINT PRIMARY KEY,
    note_ids TEXT, -- JSON数组
    updated_at DATETIME
);
```

👉 首页直接读，避免实时计算

---

## ✅ 4. （建议）标签增强

你的 tag 现在太简单了，可以加：

```sql
ALTER TABLE tag ADD COLUMN weight FLOAT DEFAULT 1.0;
```

👉 用于：

* AI生成标签权重
* 推荐排序

---