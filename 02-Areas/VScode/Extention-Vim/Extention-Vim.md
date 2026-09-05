

# LAST

可以，这样反而最简单，而且符合你现在这个需求。

核心思路就是：**不再折腾 `vim.statusBarBackground` 这个自定义 ThemeColor，直接把 VSCode 内置的 `statusBarItem.errorBackground` 当作背景色载体。**

你的 `updateColor()` 可以直接改成：

```ts
private updateColor(mode: Mode) {
  let foreground: string | undefined;
  let background: string | undefined;

  const colorToSet = (
    configuration.statusBarColors as unknown as Record<
      string,
      string | string[] | undefined
    >
  )[Mode[mode].toLowerCase()];

  if (colorToSet !== undefined) {
    if (typeof colorToSet === 'string') {
      background = colorToSet;
    } else {
      [background, foreground] = colorToSet;
    }
  }

  // VSCode StatusBarItem 的 backgroundColor 只接受特定的 ThemeColor。
  // 使用 errorBackground 作为背景色载体。
  const workbenchConfiguration =
    vscode.workspace.getConfiguration('workbench');

  const colorCustomizations =
    workbenchConfiguration.get<Record<string, string>>(
      'colorCustomizations',
      {},
    );

  const newColorCustomizations = {
    ...colorCustomizations,
    'statusBarItem.errorBackground': background,
    'statusBarItem.errorForeground': foreground,
  };

  void workbenchConfiguration.update(
    'colorCustomizations',
    newColorCustomizations,
    vscode.ConfigurationTarget.Global,
  );

  this.statusBarItem.backgroundColor = new vscode.ThemeColor(
    'statusBarItem.errorBackground',
  );
  this.statusBarItem.color = new vscode.ThemeColor(
    'statusBarItem.errorForeground',
  );
}
```

不过这里有一个**很重要的小问题**：

你原来的 `setText()` 里面有：

```ts
if (!configuration.statusBarColorControl) {
  this.statusBarItem.color = isError
    ? new vscode.ThemeColor('statusBarItem.errorForeground')
    : undefined;
  this.statusBarItem.backgroundColor = isError
    ? new vscode.ThemeColor('statusBarItem.errorBackground')
    : undefined;
}
```

这样的话，**VSCodeVim 自己发生 error 时，也会复用我们拿来做模式背景的 `errorBackground`**。

如果你说的：

> 调用这个应该不多，真调用了也就一忽儿

那完全可以接受。错误提示结束以后，下一次模式切换又会重新设置。

### 我更建议再简单一点

你的模式颜色实际上已经是：

```json
"vim.statusBarColors.normal": [
    "#a7c080",
    "#2d353b"
]
```

也就是：

```text
[背景色, 前景色]
```

那么 `updateColor()` 只负责两件事情：

```ts
private updateColor(mode: Mode) {
  let foreground: string | undefined;
  let background: string | undefined;

  const colorToSet = (
    configuration.statusBarColors as unknown as Record<
      string,
      string | string[] | undefined
    >
  )[Mode[mode].toLowerCase()];

  if (colorToSet !== undefined) {
    if (typeof colorToSet === 'string') {
      background = colorToSet;
    } else {
      [background, foreground] = colorToSet;
    }
  }

  // 将自定义颜色注入 VSCode 的内置 StatusBarItem 背景色
  const workbench = vscode.workspace.getConfiguration('workbench');
  const colorCustomizations =
    workbench.get<Record<string, string>>('colorCustomizations', {});

  void workbench.update(
    'colorCustomizations',
    {
      ...colorCustomizations,
      'statusBarItem.errorBackground': background,
      'statusBarItem.errorForeground': foreground,
    },
    vscode.ConfigurationTarget.Global,
  );

  this.statusBarItem.backgroundColor =
    new vscode.ThemeColor('statusBarItem.errorBackground');

  this.statusBarItem.color =
    new vscode.ThemeColor('statusBarItem.errorForeground');

  this.previousMode = mode;
}
```

然后你之前加的：

```json
"vim.statusBarBackground"
"vim.statusBarForeground"
```

**全部可以删掉了。**

---

不过还有一个细节值得注意：如果你现在 `setText()` 里已经有：

```ts
const shouldUpdateColor =
  configuration.statusBarColorControl &&
  vimState.currentMode !== this.previousMode;

if (shouldUpdateColor) {
  this.updateColor(vimState.currentMode);
}
```

那么 `updateColor()` **只在模式发生变化时调用**，不会每次 `setText()` 都修改 `settings.json`。

所以这个方案的实际行为就是：

```text
NORMAL
   ↓
updateColor()
   ↓
errorBackground = #a7c080
errorForeground = #2d353b
   ↓
StatusBarItem.backgroundColor
   ↓
绿色背景

        i

INSERT
   ↓
updateColor()
   ↓
errorBackground = #83c092
errorForeground = #2d353b
   ↓
浅绿色背景
```

这就比较干净了。

**唯一的副作用就是会修改用户的 `workbench.colorCustomizations` 中 `statusBarItem.errorBackground/errorForeground`。** 如果你接受这个副作用，这个方案比前面那个自定义 ThemeColor 桥接方案简单很多。