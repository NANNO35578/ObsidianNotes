# 开发周志 —— 第8周：RESTful API 规范与认证方案设计

## 本周 TODO
- [ ] 制定 RESTful API 设计原则（URL 命名、HTTP 方法、状态码）
- [ ] 设计统一响应格式与错误码体系
- [ ] 设计 JWT 认证流程（签发、校验、刷新、多端登录）
- [ ] 定义接口鉴权中间件与过滤器规范

## 工作内容概括

本周完成所有 API 的顶层设计。统一响应格式确定为 JSON 封装：`{"code": 0, "message": "success", "data": {}}`，其中 `code = 0` 表示成功，非 0 表示业务异常。HTTP 状态码仅用于传输层语义（200/400/401/500），具体业务错误通过 `code` + `message` 表达。

认证方案采用 JWT（HS256）：
- 登录成功后服务端生成 Token 并同时写入 `user_token` 表，支持多端登录；
- 后续请求通过 `Authorization: Bearer {token}` 头部携带；
- Token 过期后需重新登录，暂不设 Refresh Token 机制；
- 退出登录时删除 `user_token` 对应记录，实现服务端可控失效。

同步规划了 Drogon Filter（`JwtFilter`）作为全局鉴权入口，对 `/api/*` 路径进行统一拦截，排除 `/api/auth/*` 公开接口。

## 关键产出
- api.md（REST API 设计总文档）框架
- JWT 认证时序图
- Drogon Filter 鉴权规范

---

## 工作记录（精简版）

### 一、RESTful 设计原则

| 原则 | 约定 |
|------|------|
| URL | 全小写 + 下划线，名词复数（`/api/notes` 实际采用 `/api/note` 与 Drogon 路由习惯对齐）|
| HTTP 方法 | POST 用于创建/更新/删除（统一用 POST 避免部分客户端不支持 DELETE/PUT），GET 仅用于查询 |
| 状态码 | 200 正常，400 参数错误，401 鉴权失败，500 服务端异常；业务语义由 `code` 表达 |
| 版本控制 | URL 中不显式带版本号（v1），因毕业设计周期内无多版本并存需求 |

### 二、响应格式与错误码

```json
{ "code": 0, "message": "success", "data": {} }
```

**错误码体系**：

| 区间 | 归属 | 示例 |
|------|------|------|
| 0 | 成功 | — |
| 1 | 通用错误 | 参数缺失、格式非法 |
| 1001-1099 | 用户认证 | Token 无效、用户已存在、密码错误 |
| 2001-2099 | 笔记/文件夹 | 笔记不存在、文件夹循环引用 |
| 3001-3099 | 文件 | 上传失败、文件不存在 |
| 4001-4099 | AI/OCR/外部服务 | DeepSeek 调用失败、OCR 识别超时 |

### 三、JWT 认证方案

- **算法**：HS256，密钥存于服务端环境变量
- **Payload**：`{ "user_id": 123, "exp": 1710000000 }`，不含敏感信息
- **存储**：登录后服务端同时写 `user_token` 表，支持多端登录记录管理
- **携带方式**：Header `Authorization: Bearer {token}`，兼容 URL 参数 `?token=`（供测试与特殊场景）
- **过期策略**：7 天有效期，无 Refresh Token，到期引导重新登录
- **失效机制**：退出时删表记录 + 客户端清本地存储，服务端以表记录为准

### 四、Drogon Filter 鉴权

```cpp
class JwtFilter : public HttpFilter<JwtFilter> {
public:
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) {
        // 1. 提取 Token（Header 或 URL 参数）
        // 2. JwtUtil::verify() 校验签名与过期时间
        // 3. 查 user_token 表确认未主动失效
        // 4. 通过则 req->attributes()->insert("user_id", uid) 透传
        // 5. 失败则返回 401 + code=1001
    }
};
```

**路由配置**：`config.json` 中注册 JwtFilter，匹配 `/api/*`，排除 `/api/auth/*`。

### 五、本周问题

- **HTTP 方法统一问题**：RESTful 规范推荐 POST/GET/PUT/DELETE 区分操作，但 Drogon 路由宏与部分移动端 HTTP 库对 DELETE Body 支持不佳；折中采用 POST 承载全部写操作，通过 URL 区分语义（`/api/note/create`、`/api/note/delete`）。
- **Token Payload 是否包含用户名**：否。仅存 `user_id`，减少 Token 体积与信息泄露面；用户名通过 `user_id` 查库获取。

### 六、会议备忘

导师意见：错误码需预留足够空间，建议各模块按千位分段（非百位），便于后续扩展；鉴权过滤器应记录日志，便于排查异常请求。
