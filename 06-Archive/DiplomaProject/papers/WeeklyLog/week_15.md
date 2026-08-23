# 开发周志 —— 第15周：Web 前端工程搭建与主题系统

## 本周 TODO
- [ ] 初始化 Vue 3 + Vite 工程，配置路径别名、代理、代码分割
- [ ] 集成 Element Plus、md-editor-v3、Axios、Vue Router、Pinia
- [ ] 实现 Everforest 深色/浅色主题系统（CSS 变量 + useTheme）
- [ ] 实现登录页与注册页 UI 及表单校验
- [ ] 封装 request.js（Axios 拦截器：Token 注入、401 跳转、错误提示）

## 工作内容概括

本周从后端转向 Web 前端开发。完成 `calcite-web` 工程初始化，配置 Vite 开发服务器代理 `/api` 到 `localhost:8888`，解决开发阶段跨域问题。主题系统采用 CSS 变量驱动，定义 `--bg-primary`、`--text-primary`、`--accent-primary` 等变量，通过 `useTheme` 组合式函数实现深色/浅色切换，主题偏好持久化至 `localStorage`。

页面方面完成登录页与注册页：
- 登录页支持用户名/密码输入，调用 `/api/auth/login`，成功后存储 Token 与用户信息至 `localStorage`，跳转 `/home`；
- 注册页支持用户名/邮箱/密码，注册成功后自动登录；
- 两页均采用 Element Plus 表单组件与自定义主题配色。

`request.js` 封装 Axios 实例：请求拦截器注入 `Authorization: Bearer {token}`；响应拦截器统一处理 401（清除 Token 跳转登录）、业务错误（ElMessage 提示）。

## 关键产出
- Web 前端工程骨架与开发服务器
- Everforest 主题系统（深/浅）
- 登录 / 注册页面
- Axios 统一请求封装

---

## 工作记录（精简版）

### 一、工程初始化

Vite 关键配置：

| 配置项 | 取值 | 说明 |
|--------|------|------|
| proxy `/api` | `localhost:8888` | 开发阶段解决 CORS |
| `manualChunks` | element-plus / md-editor-v3 | 代码分割，降低首屏体积 |
| `resolve.alias` | `@ → src` | 路径别名 |

依赖版本锁定：`vue@3.5.24`，`element-plus@2.13.7`，`md-editor-v3@6.4.2`，`axios@1.13.2`，`pinia@3.0.4`，`vue-router@4.6.4`。

### 二、主题系统

CSS 变量定义（深色模式节选）：

```css
:root[data-theme="dark"] {
  --bg-primary: #272e33;
  --bg-secondary: #2d353b;
  --text-primary: #d3c6aa;
  --accent-primary: #7fbbb3;
}
```

`useTheme()` 组合式函数：读取 `localStorage` 中 `calcite-theme`，切换时更新 `document.documentElement.dataset.theme`，并持久化偏好。

### 三、登录/注册页

| 页面 | 校验规则 | 成功行为 |
|------|----------|----------|
| Login | 用户名 3-20 位，密码 ≥6 位 | 存 Token + userInfo 至 localStorage，跳转 `/home` |
| Register | 邮箱格式可选填，密码 ≥6 位 | 注册成功后自动调用登录接口，无感进入首页 |

表单使用 Element Plus `el-form` + `rules`，错误提示中文本地化。

### 四、request.js 封装

```javascript
// 请求拦截器：注入 Bearer Token
request.interceptors.request.use((config) => {
  const token = localStorage.getItem('token');
  if (token) config.headers.Authorization = `Bearer ${token}`;
  return config;
});

// 响应拦截器：401 强制跳转
request.interceptors.response.use(
  (res) => res.data,
  (err) => {
    if (err.response?.status === 401) {
      localStorage.removeItem('token');
      window.location.href = '/login';
    }
    return Promise.reject(err);
  }
);
```

### 五、本周问题

- **Element Plus 样式与主题变量冲突**：组件内部硬编码颜色覆盖 CSS 变量。解决：使用 `:deep()` 选择器强制覆盖，或利用 Element Plus 的 CSS 变量映射（`--el-color-primary` → `--accent-primary`）。
- **md-editor-v3 主题适配**：编辑器内部使用固定背景色，与 Everforest 不搭。解决：通过 `:deep(.md-editor)` 覆盖编辑器 CSS 变量，动态切换时同步更新。

### 六、会议备忘

自查：Web 工程可正常运行，登录/注册联调通过，Token 注入与 401 跳转正常。下一步构建三栏主界面框架。
