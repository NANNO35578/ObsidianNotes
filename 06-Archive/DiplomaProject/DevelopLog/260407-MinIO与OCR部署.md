
# 部署MinIO&PaddleOCR

````markdown title:""
很好，这一步是**整个多模态链路的基础设施搭建**，我直接给你一套**可落地、少踩坑、适合你当前环境（WSL + C++后端）**的流程。

分两部分讲：**MinIO（存储） → PaddleOCR（识别）**

---

# 一、MinIO 部署（对象存储）

## ✅ 推荐方式：Docker（最省事、最稳）

---

## 1️⃣ 拉取镜像

```bash
docker pull minio/minio
```

---

## 2️⃣ 启动 MinIO

```bash
docker run -d \
  -p 9000:9000 \
  -p 9001:9001 \
  --name minio \
  -e "MINIO_ROOT_USER=admin" \
  -e "MINIO_ROOT_PASSWORD=12345678" \
  -v /mnt/data/minio:/data \
  minio/minio server /data --console-address ":9001"
```

---

## 3️⃣ 访问控制台

打开浏览器：

```
http://localhost:9001
```

登录：

* user: `admin`
* password: `12345678`

---

## 4️⃣ 创建 Bucket

* 名称：`notes-files`
* 权限：**private（推荐）**

---

## 5️⃣ 获取访问参数（给 Drogon 用）

你需要这几个：

```text
endpoint: http://127.0.0.1:9000
access_key: admin
secret_key: 12345678
bucket: notes-files
```

---

## 6️⃣ C++ 接入建议

MinIO 是 S3 协议 → 你有两种方式：

### ✅ 推荐：直接用 S3 SDK（AWS SDK for C++）

优点：

* 成熟稳定
* 和未来云存储兼容

---

### 简单上传逻辑：

```cpp
PutObjectRequest request;
request.SetBucket("notes-files");
request.SetKey("user1/xxx.png");
request.SetBody(fileStream);
```

---

## ✅ 到这里你已经完成：

✔ 文件上传
✔ 文件持久化
✔ 可扩展存储

---

# 二、PaddleOCR 部署（重点）

你选的是：

> ✅ PaddleOCR v5_server（正确选择 👍）

---

## ❗部署建议（很重要）

👉 **不要直接嵌入 C++**
👉 一定要做成：**独立 OCR 服务（Python）**

---

# 1️⃣ 环境准备（WSL）

```bash
sudo apt update
sudo apt install python3-pip -y
```

---

## 2️⃣ 安装 PaddleOCR

```bash
pip install paddlepaddle -i https://mirror.baidu.com/pypi/simple
pip install paddleocr
```

---

## 3️⃣ 测试 OCR 是否可用

```bash
paddleocr --image_dir test.png --use_angle_cls true --lang ch
```

---

# 4️⃣ 下载 v5_server 模型（关键）

👉 官方模型（推荐）：

```bash
mkdir -p ~/models/paddleocr
cd ~/models/paddleocr
```

下载（示例）：

```bash
wget https://paddleocr.bj.bcebos.com/PP-OCRv5/chinese/ch_PP-OCRv5_server_infer.tar
tar -xvf ch_PP-OCRv5_server_infer.tar
```

---

# 5️⃣ 写一个 OCR 服务（强烈推荐）

用 FastAPI（简单 + 高效）

---

## 📌 OCR 服务示例

```python
from fastapi import FastAPI, UploadFile
from paddleocr import PaddleOCR
import shutil

app = FastAPI()

ocr = PaddleOCR(
    use_angle_cls=True,
    lang="ch",
    det_model_dir="models/...",
    rec_model_dir="models/..."
)

@app.post("/ocr")
async def do_ocr(file: UploadFile):
    path = f"/tmp/{file.filename}"
    
    with open(path, "wb") as buffer:
        shutil.copyfileobj(file.file, buffer)

    result = ocr.ocr(path)

    text = "\n".join([line[1][0] for line in result[0]])

    return {
        "text": text,
        "raw": result
    }
```

---

## 6️⃣ 启动 OCR 服务

```bash
uvicorn main:app --host 0.0.0.0 --port 8001
```

---

## 7️⃣ 测试

```bash
curl -X POST "http://localhost:8001/ocr" \
  -F "file=@test.png"
```

---

# 三、Drogon 如何调用 OCR

你只需要做一件事：

👉 发 HTTP 请求

---

## 调用流程

```text
Drogon
   ↓
上传文件到 MinIO
   ↓
下载 or 传URL
   ↓
POST /ocr
   ↓
拿到 text
```

---

## ❗优化建议（很关键）

### 不要传文件 → 传 URL

改成：

```json
{
  "url": "http://minio/xxx.png"
}
```

OCR 服务去下载

👉 优点：

* 避免重复传输
* 更快
* 更干净

---

# 四、你现在的最小可运行链路

## ✅ 已完成：

```text
[上传文件]
   ↓
[MinIO存储]
   ↓
[OCR服务识别]
   ↓
[返回文本]
```

---

# 五、常见坑（帮你避雷）

## ❗1. PaddleOCR 很慢

解决：

* 使用 server 模型（你已经选对）
* CPU建议开多线程

---

## ❗2. 中文乱码

确保：

```python
lang="ch"
```

---

## ❗3. 内存占用高

正常现象（模型加载）

👉 建议：

* OCR服务常驻
* 不要每次初始化

---

## ❗4. WSL 访问问题

如果访问不到：

```bash
curl localhost:8001
```

---

# 六、下一步建议（非常关键）

你现在应该做：

## ✅ Step 1

✔ MinIO 跑起来

## ✅ Step 2

✔ OCR 服务跑起来

## ✅ Step 3

✔ Drogon 能调 OCR

---

如果你下一步要继续，我建议直接做：

👉 **“Drogon + OCR + MinIO 打通代码”**

我可以帮你写：

* Drogon 上传文件代码
* S3 接入
* OCR 调用封装
* 异步任务队列

你只要说一句：
👉 **“打通三者代码”** 🚀

````


----

# 选型过程

````markdown title:"origin"

# 项目介绍
有一个智能笔记管理系统. 后端基于drogon框架, 笔记内容存储在MariaDB的表字段中. 接入了ElasticSearch, 处理了笔记写入数据库和ES的流程. 

# 后端设计的API

## 后端已开发的API如下

| 接口                 | 方法   | 说明        | 附加参数                |
| ------------------ | ---- | --------- | ------------------- |
| /api/auth/register | POST | 用户注册      |                     |
| /api/auth/login    | POST | 用户登录      |                     |
| /api/auth/logout   | POST | 退出登录      |                     |
| /api/user/profile  | GET  | 获取用户信息    |                     |
|                    |      |           |                     |
| /api/note/create   | POST | 新建笔记      |                     |
| /api/note/update   | POST | 更新笔记      |                     |
| /api/note/delete   | POST | 删除笔记      |                     |
| /api/note/list     | GET  | 获取笔记列表    | folder_id, tag_ids  |
| /api/note/detail   | GET  | 获取笔记详情    |                     |
| /api/note/search   | GET  | 全文搜索      | keyword, from, size |
|                    |      |           |                     |
| /api/tag/create    | POST | 创建标签      |                     |
| /api/tag/list      | GET  | 获取标签列表    | note_id             |
| /api/tag/bind      | POST | 绑定/解除笔记标签 |                     |
| /api/folder/create | POST | 创建文件夹     |                     |
| /api/folder/list   | GET  | 获取文件夹列表   | folder_id           |
|                    |      |           |                     |

## 设计的文件与 OCR API如下

| 接口                | 方法   | 说明    |
| ------------------ | ---- | ----- |
| /api/file/upload   | POST | 上传附件  |
| /api/file/list     | GET  | 获取附件  |
| /api/ocr/recognize | POST | 图片转文本 |

# 你需要考虑如下问题
目前系统准备融合OCR等人工智能技术，以解决现有笔记工具在多模态信息采集等方面的关键痛点。

利用OCR引擎（如Tesseract模型）识别图片与手写笔记中的文字；通过语音识别服务将录音转换为文本；

该模块通常以独立服务或微服务形式存在，供后端服务按需调用。

使用对象存储服务（如MinIO）保存图片、音频等非结构化数据。

多模态数据统一管理与智能处理集成的挑战：系统需同时处理文本、图片、语音等多种格式数据，并调用不同AI模型进行处理，架构复杂。解决方案：设计统一的数据模型与接口规范，将笔记内容与媒体文件分离存储并通过元数据关联。将OCR、NLP等智能处理单元封装为独立的微服务，通过消息队列进行异步任务调度，实现高内聚、低耦合的系统架构，确保智能功能稳定、高效地集成到核心业务流程中。

OCR模块可以考虑集成PaddleOCR C++ SDK与Tesseract，

针对多模态数据，设计统一的数据结构进行管理，文本内容直接存储，图片、语音文件使用对象存储，并将其识别后的文本与元数据关联入库。


# 你的任务
你只需要给出目前开发ORC以及文件处理的方法

````

````markdown title:"Doubao"
# 任务
请你为我的智能笔记管理系统，**自主分析并设计一套完整的文件处理 + OCR 多模态识别方案**。
我不提供固定技术选型，由你**综合评估后给出最优方案**。

# 项目背景
- 后端：Drogon（C++）
- 数据库：MariaDB
- 已接入：Elasticsearch 用于笔记全文搜索
- 业务：笔记系统，需要支持图片、文档、语音等多模态内容录入与管理
- 目标：解决多模态信息采集、存储、识别、检索问题

# 你的工作内容（只做这部分）
1. 文件存储方案选型
   - 分析：本地存储、对象存储、云存储等优缺点
   - 给出**最终推荐的存储方案**，并说明理由
2. OCR 文字识别方案选型
   - 可考虑：Tesseract、PaddleOCR、在线OCR、闭源SDK等
   - 结合 C++/Drogon 环境，给出**最合适的OCR方案**
3. 语音转文字方案（可选）
   - 简要给出是否接入、如何接入
4. 整体架构设计
   - 同步/异步处理
   - 是否拆微服务
   - 如何与现有笔记系统对接
   - 数据如何存储、关联、入库
5. 接口设计
   - 文件上传
   - OCR 识别
   - 结果回调/入库流程

# 要求
- 不要只罗列技术，要**给出明确结论：用什么、为什么、怎么用**
- 方案要可落地，适合 C++ 后端
- 不要实现代码，只给方案设计与选型理由
````

````markdown title:"ans"
你这个阶段已经不是“选工具”的问题了，而是**要搭一条完整的数据处理流水线（ingest → process → index → retrieve）**。我直接给你一套**可落地、偏工程化的最优方案**，不是泛泛而谈。

---

# 一、文件存储方案（结论先行）

## ✅ 最终推荐：**对象存储（MinIO / S3）+ 本地缓存**

### 为什么不用纯本地存储？

* ❌ 扩展性差（磁盘不够很麻烦）
* ❌ 多机部署困难（路径一致性问题）
* ❌ 不利于后续做分布式

### 为什么不用纯云存储（OSS/S3）？

* ❌ 成本不可控（你的毕业设计/初期项目不划算）
* ❌ 开发调试不方便（依赖外网）

---

## ✅ 最优组合

### 🧱 存储层设计

```
[客户端]
   ↓
[Drogon]
   ↓
[MinIO对象存储]  ←→  [本地缓存（可选）]
```

---

## 📌 具体方案

### 1. 文件存储

* 使用 **MinIO（自建 S3 兼容对象存储）**
* 每个文件存：

  * `bucket`: notes-files
  * `object key`: userId/yyyy/mm/dd/uuid.ext

### 2. 数据库存储（MariaDB）

只存元数据：

```sql
file_table
-----------
id
user_id
file_name
file_type
file_size
object_key
url
status (processing/done/failed)
created_at
```

---

## ✅ 优势总结

* ✔ 易扩展（未来直接换 AWS S3）
* ✔ 支持大文件
* ✔ 支持 CDN（后期）
* ✔ 和 ES 解耦（只存文本）

---

# 二、OCR 方案选型（重点）

## 🚫 先排除

### Tesseract

* ❌ 中文效果一般
* ❌ 对复杂场景（表格/手写）差
* ❌ C++友好但效果不够

---

### 在线 OCR（百度/阿里）

* ❌ 成本高
* ❌ 网络依赖
* ❌ 数据隐私问题

---

## ✅ 最终推荐：**PaddleOCR（本地部署）**

---

## 为什么是 PaddleOCR？

* ✔ 中文识别强（核心优势）
* ✔ 支持：

  * 文档
  * 表格
  * 手写
* ✔ 可本地部署（无费用）
* ✔ 推理速度快（CPU/GPU都行）
* ✔ 有服务化部署方案

---

## ❗关键问题：你是 C++ 后端

PaddleOCR是 Python为主 → 怎么接？

---

## ✅ 最优架构（非常关键）

### 👉 OCR 独立为“识别服务”（Python）

```
[Drogon(C++)]  →  [OCR Service (Python + PaddleOCR)]
                        ↓
                    返回JSON
```

---

## 📌 调用方式

* HTTP（推荐）
* gRPC（进阶）

---

## 📌 返回结果示例

```json
{
  "text": "识别出来的完整文本",
  "blocks": [
    {
      "text": "标题",
      "bbox": [x1, y1, x2, y2]
    }
  ]
}
```

---

## ✅ 优势

* C++不用折腾深度学习库
* OCR可以独立扩展
* 后期可以替换模型（甚至接入大模型）

---

# 三、语音转文字（建议接入）

## ✅ 推荐方案：**Whisper（OpenAI开源）**

---

## 方案选择：

| 方案        | 结论   |
| --------- | ---- |
| Whisper本地 | ✅ 推荐 |
| 科大讯飞API   | 可选   |
| 百度语音      | 可选   |

---

## 为什么选 Whisper？

* ✔ 离线可用
* ✔ 多语言
* ✔ 效果好
* ✔ 可服务化

---

## 架构和 OCR 一样：

```
[Drogon] → [ASR Service (Whisper)]
```

---

# 四、整体架构设计（核心）

## ✅ 总体架构（推荐）

```
                ┌──────────────┐
                │   Client     │
                └──────┬───────┘
                       ↓
                ┌──────────────┐
                │  Drogon API  │
                └──────┬───────┘
                       ↓
        ┌──────────────┼──────────────┐
        ↓              ↓              ↓
   [MinIO]     [Task Queue]     [MariaDB]
                     ↓
              ┌────────────┐
              │ OCR Service│
              └────────────┘
                     ↓
              ┌────────────┐
              │ ASR Service│
              └────────────┘
                     ↓
              ┌────────────┐
              │Elasticsearch│
              └────────────┘
```

---

## ✅ 是否异步？——必须异步

OCR/语音识别是：

* 耗时（1~5s+）
* CPU密集

---

### 👉 推荐：消息队列

* Redis Stream（轻量）
* RabbitMQ（更标准）

---

## 📌 流程

### 上传流程

1. 用户上传文件
2. Drogon：

   * 存 MinIO
   * 写 DB（status=processing）
   * 投递任务到队列
3. 返回：上传成功

---

### 后台处理

Worker：

1. 拉取任务
2. 判断文件类型：

   * 图片 → OCR
   * PDF → OCR
   * 音频 → ASR
3. 提取文本
4. 存入：

   * MariaDB（note_content）
   * Elasticsearch（全文索引）
5. 更新状态 = done

---

# 五、是否拆微服务？

## ✅ 建议：**轻微服务化（不是完全微服务）**

### 拆分：

| 模块         | 是否独立   |
| ---------- | ------ |
| Drogon API | 主服务    |
| OCR        | 独立服务 ✅ |
| ASR        | 独立服务 ✅ |
| Worker     | 可独立    |

---

👉 理由：

* 深度学习环境复杂（Python）
* 解耦C++后端
* 易扩展

---

# 六、数据设计（关键）

## 📌 note 表

```sql
note
-------
id
user_id
title
content_text   (OCR/ASR提取)
raw_file_id
created_at
```

---

## 📌 file 表

```sql
file
-------
id
user_id
object_key
file_type
status
```

---

## 📌 ES 存储

```json
{
  "note_id": 1,
  "content": "OCR提取全文",
  "title": "xxx"
}
```

---

# 七、接口设计（直接可用）

## 1️⃣ 文件上传

```
POST /api/file/upload
```

### 请求

* multipart file

### 返回

```json
{
  "file_id": 123,
  "status": "processing"
}
```

---

## 2️⃣ OCR触发（可选）

```
POST /api/file/recognize
```

```json
{
  "file_id": 123
}
```

---

## 3️⃣ 查询处理状态

```
GET /api/file/status?id=123
```

---

## 4️⃣ 获取识别结果

```
GET /api/note/detail?id=xxx
```

---

# 八、最终结论（帮你收敛）

## 🔥 最优技术组合

| 模块  | 选型                |
| --- | ----------------- |
| 存储  | MinIO             |
| OCR | PaddleOCR（独立服务）   |
| 语音  | Whisper           |
| 队列  | Redis Stream      |
| 搜索  | Elasticsearch（已用） |
| 后端  | Drogon（核心API）     |

---

## 🔥 核心设计思想

> **“文件存储解耦 + 识别异步化 + 文本统一入ES”**

---

# 如果你下一步要继续

我建议你下一步直接做：

👉 **“OCR服务 + Drogon接入”落地实现”**

我可以帮你把下面这些直接写出来：

* OCR服务（FastAPI版）
* Drogon调用代码
* Redis队列设计
* ES入库结构

你只要说一句：**“做OCR落地”**，我帮你推进到代码级别。

````


````markdown title:"Added"
# 任务
请基于我的智能笔记管理系统，自主分析并设计一套完整的
【图片/文件上传 → OCR 识别 → 大模型智能加工 → 生成结构化笔记】的完整方案。

# 项目背景
后端：Drogon（C++）
数据库：MariaDB
搜索引擎：Elasticsearch
OCR 方案：已确定使用 PaddleOCR v5_server 本地部署
存储：已确定对象存储方案

# 你的工作
1. 根据**本地部署、性能、成本、易用性、笔记整理场景**，
   自主评估并**推荐最合适的大模型**（可本地/可API），并说明理由。
2. 设计完整流程：
   - 用户上传图片/文件
   - 后端调用 OCR 得到原始文本
   - 将 OCR 结果送入大模型智能处理
   - 大模型输出**结构化、干净、可直接使用的笔记**
   - 返回给后端，存入 DB + ES
   - 前端展示，用户可自行编辑
3. 输出内容：
   - 大模型选型与对比（本地模型 / 在线API）
   - 推荐最终方案
   - 整体流程架构
   - 接口设计
   - 数据格式
   - 异步/同步处理策略

# 要求
- 不预设大模型，由你**自主评估选择**
- 方案必须可落地、适合笔记系统
- 结构清晰、工程化、可直接开发
````


````markdown title:"Ans"
这一步其实已经从“OCR系统”升级成**AI内容加工流水线**了，核心不在“识别”，而在**把脏乱 OCR 文本变成高质量笔记**。
我给你一套可以直接落地的工程方案（带取舍，不是全都要）。

---

# 一、大模型选型（先给结论）

## ✅ 最终推荐（分阶段）

> **当前最优：API大模型（主） + 预留本地模型扩展（辅）**

---

## 1️⃣ 候选方案对比

### 🟢 本地模型（Ollama / vLLM）

| 模型       | 优点  | 缺点       |
| -------- | --- | -------- |
| Qwen2.5  | 中文强 | 需要GPU才好用 |
| LLaMA3   | 通用强 | 中文略弱     |
| DeepSeek | 推理强 | 部署复杂     |
| ChatGLM  | 易用  | 效果一般     |

👉 结论：

* ❌ 你当前机器（i7-12700H）→ **CPU跑很慢**
* ❌ 长文本总结能力弱
* ❌ 输出结构稳定性不够

---

### 🔵 在线API（强烈推荐）

| 模型               | 特点      |
| ---------------- | ------- |
| GPT-4.1 / GPT-4o | 结构化能力极强 |
| Claude           | 长文本总结优秀 |
| 通义千问API          | 中文强     |
| DeepSeek API     | 成本低     |

---

## ✅ 最终推荐组合

### 🥇 主力模型（强烈推荐）

👉 **GPT-4.1 / GPT-4o（API）**

理由：

* ✔ 结构化输出稳定（关键！）
* ✔ OCR脏文本纠错能力强
* ✔ 能直接生成“笔记格式”
* ✔ 支持长上下文（适合PDF）

---

### 🥈 备选（降本）

👉 **DeepSeek API / 通义千问**

---

### 🧩 未来扩展（可选）

👉 本地 Qwen2.5（通过 Ollama）

用于：

* 离线模式
* 降成本

---

# 二、核心流程设计（重点）

## ✅ 完整流水线

```id="flow1"
[客户端上传]
      ↓
[Drogon]
      ↓
[对象存储 MinIO]
      ↓
[任务队列 Redis]
      ↓
[OCR服务（PaddleOCR）]
      ↓
[LLM加工服务]
      ↓
[结构化笔记]
      ↓
[MariaDB + Elasticsearch]
      ↓
[前端展示]
```

---

# 三、核心：LLM加工做什么？

不是简单“总结”，而是：

## ✅ 目标输出

```json id="note_struct"
{
  "title": "自动生成标题",
  "summary": "简要总结",
  "content": "整理后的正文（干净）",
  "outline": [
    "要点1",
    "要点2"
  ],
  "keywords": ["关键词1", "关键词2"],
  "qa": [
    {
      "q": "问题",
      "a": "答案"
    }
  ]
}
```

---

# 四、关键设计：Prompt工程（核心竞争力）

## ✅ 输入给大模型的内容

```id="input_prompt"
你是一个笔记整理助手，请将以下OCR识别的文本整理成结构化笔记：

要求：
1. 修复错别字
2. 去除无意义内容
3. 保持原意
4. 自动分段
5. 提取标题、摘要、关键词
6. 输出JSON格式

OCR文本：
{{text}}
```

---

## ❗关键点

* 必须 **强约束 JSON 输出**
* 否则后端很难解析

---

# 五、同步 or 异步？

## ✅ 必须：异步

原因：

* OCR：1~3秒
* LLM：2~10秒

---

## 推荐策略

| 操作        | 模式 |
| --------- | -- |
| 上传        | 同步 |
| OCR + LLM | 异步 |
| 查询结果      | 轮询 |

---

## 状态流转

```id="status_flow"
uploaded → processing → ocr_done → llm_done → completed
```

---

# 六、服务拆分（非常关键）

## ✅ 推荐架构

```id="services"
Drogon (主服务)
│
├── 文件服务（MinIO）
├── 任务队列（Redis）
├── OCR服务（Python）
├── LLM服务（Python）
└── Worker（任务消费）
```

---

## 为什么 LLM 单独服务？

* ✔ 解耦 API Key
* ✔ 可切换模型
* ✔ 可加缓存
* ✔ 可做限流

---

# 七、接口设计（直接能写代码）

---

## 1️⃣ 上传文件

```id="api_upload"
POST /api/file/upload
```

返回：

```json id="upload_resp"
{
  "file_id": 1,
  "status": "processing"
}
```

---

## 2️⃣ 查询处理结果

```id="api_status"
GET /api/file/result?id=1
```

---

## 3️⃣ 返回结构化笔记

```json id="note_resp"
{
  "status": "completed",
  "note": {
    "title": "...",
    "summary": "...",
    "content": "...",
    "outline": [],
    "keywords": []
  }
}
```

---

# 八、数据库设计（升级版）

## note 表

```sql id="note_table"
note
-------
id
user_id
title
summary
content
keywords (json)
outline (json)
file_id
status
created_at
```

---

# 九、Elasticsearch设计

## 存储内容

```json id="es_doc"
{
  "note_id": 1,
  "title": "...",
  "content": "...",
  "summary": "...",
  "keywords": []
}
```

---

# 十、性能优化（很关键）

## ✅ 1. 分段处理（大文件）

* OCR结果 > 5k tokens：
  👉 分块 → 分别送LLM → 再汇总

---

## ✅ 2. LLM缓存

```id="cache"
hash(ocr_text) → result
```

避免重复调用

---

## ✅ 3. 限流

* 用户每天调用次数限制
* 防止API爆炸

---

# 十一、最终推荐架构总结

## 🔥 最优方案

| 模块   | 选型             |
| ---- | -------------- |
| 存储   | MinIO          |
| OCR  | PaddleOCR      |
| LLM  | GPT-4.1（主）     |
| 队列   | Redis Stream   |
| 后端   | Drogon         |
| AI服务 | Python FastAPI |

---

## 🔥 核心思想

> **“OCR负责看懂文字，LLM负责理解内容”**

---

# 十二、你下一步该做什么（非常关键）

不要再发散了，直接进入落地：

## ✅ Step 1（强烈建议你现在做）

👉 搭建 **LLM服务（FastAPI）**

## ✅ Step 2

👉 Drogon 调用 LLM API

## ✅ Step 3

👉 打通：
OCR → LLM → 入库 → ES

---

如果你愿意，我可以下一步直接帮你：

👉 设计 **LLM服务完整接口 + Prompt模板 + Drogon调用代码**

你只要说一句：
**“做LLM服务落地”** 👇

````
















