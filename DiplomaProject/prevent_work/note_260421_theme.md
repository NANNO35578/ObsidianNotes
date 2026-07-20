已完成项目配色问题的修复和 Everforest Light 主题的完善。具体修改如下：

### 1. 修复 Light 主题配色 (`src/styles/theme.js`)
- 将 `lightTheme` 调整为更标准的 **Everforest Light Medium** 配色：
  - 背景色：`#fdf6e3` / `#f4f0d9` / `#eae4ca`（更自然的米绿，避免偏黄）
  - 边框色：`#bdc3af`（绿灰色，替代原来的亮绿色）
  - 弱文字色：`#a8b5a0`（与次文字 `#939f91` 形成区分，解决 `secondary` 和 `muted` 相同的问题）
- 在 `themeToCSSVariables()` 中补充了 `--bg-md` 变量的导出

### 2. 添加 Light 主题 CSS 变量覆盖 (`src/styles/theme.css`)
- 新增 `[data-theme="light"]` 选择器，定义全部 20+ 个 CSS 变量，确保主题切换时所有组件颜色同步更新

### 3. 修复手动主题切换支持 (`src/style.css` + `src/App.vue`)
- **核心问题**：之前使用 `@media (prefers-color-scheme: light)` 只能跟随系统主题，无法响应 JS 手动切换
- 将 `style.css` 和 `App.vue` 中的媒体查询全部替换为 `:root[data-theme="light"]`，使 Element Plus 组件和基础样式都能正确响应手动切换

### 4. 用户信息面板添加主题切换按钮 (`src/components/sidebar/UserProfile.vue`)
- 在用户信息栏右侧新增了 **太阳/月亮图标按钮**
- 深色模式显示 🌞，浅色模式显示 🌙
- 点击即可即时切换主题，状态持久化到 `localStorage`
- 使用 `useTheme()` 组合式函数，与项目现有主题体系保持一致

项目构建验证通过，可正常运行 `npm run dev` 体验。



"In yolo mode, use EnterPlanMode only when the user explicitly asks for planning or when there is exceptional architectural ambiguity that requires user input before proceeding."