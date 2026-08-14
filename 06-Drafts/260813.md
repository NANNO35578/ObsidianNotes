> 好久不见

> 好久不见，哈哈哈哈。
> 
> 看到你回来挺好的。上次我们聊到的内容跨度其实挺大，从毕业设计、技术栈、工作入职，到后来你和女朋友异地、分手那段经历。那时候你刚毕业入职成都的新公司，也在适应新的环境和人际关系。
> 
> 不知道你这段时间过得怎么样了？  
> 
> 工作上有没有适应一些，USM/状态机那类学习任务后来推进得怎么样？或者也可以聊聊最近生活里的变化。🙂


> 聊个新的话题吧. 你应该知道git flow吧, 就是定义五个核心分支（Main、Develop、Feature、Release、Hotfix）  来管理仓库. 但是当我实际应用时候发现, 对于我自己的仓库应用这种分支管理, 总感觉在左右脑互博. 时常将不属于main分支的东西在main上做了修改, 同时我发现我一个人这么管理分支总有些力不从心, 而且切换分支感觉异常繁琐. 如何应对

# ANS 1


知道，而且你这个感觉其实非常正常。**Git Flow 本身没错，但它解决的是“多人、长期、版本发布型项目”的协作问题，不是“一个人写自己的仓库”这个问题。**

你现在这种“左右脑互博”的体验，恰恰说明了一件事：

> **你不是不会 Git Flow，而是 Git Flow 的流程成本已经超过了它给你的收益。**

尤其是个人仓库，五类分支很容易变成：

```text
main
 └── develop
      ├── feature/login
      ├── feature/search
      └── feature/xxx
```

然后你脑子里开始出现：

> “我现在在哪个分支？”  
> “这个修改应该进 develop 还是 feature？”  
> “feature 做完了先 merge develop？”  
> “release 要不要开？”  
> “卧槽我刚才怎么在 main 改东西了？”

最后 Git 从版本管理工具变成了**分支管理模拟器**。😂

---

## 先说结论：个人项目我更推荐 GitHub Flow / 简化版 Git Flow

对于你这种情况，我会建议直接砍掉：

- `develop`
    
- `release`
    
- `hotfix`
    

只保留：

```text
main
  │
  ├── feature/xxx
  ├── feature/yyy
  └── fix/zzz
```

核心原则就一句：

> **main 永远是“随时可以运行/发布”的状态，所有开发都从 main 拉临时分支，完成后合并回 main。**

例如：

```bash
git switch main
git pull

git switch -c feature/login
```

开发：

```text
feature/login
    ↓
commit
commit
commit
    ↓
main
```

完成后：

```bash
git switch main
git merge --no-ff feature/login
git branch -d feature/login
```

然后继续：

```bash
git switch -c feature/search
```

就这么简单。

---

# 但你提到的“经常误操作 main”，我觉得才是重点

这个问题**不是 Git Flow 能解决的**。

因为 Git 分支模型再漂亮，也无法阻止你：

```bash
git switch main
vim xxx.cpp
git add .
git commit
```

然后：

> “……操。”

所以我反而建议你建立一个非常简单的个人规则：

## `main` 不直接开发

把它理解成：

> **main 是成品货架，不是工作台。**

你甚至可以形成肌肉记忆：

```text
我要写代码
    ↓
现在是不是 main？
    ↓
是 → 创建 feature/fix 分支
    ↓
开始干活
```

每次打开项目：

```bash
git status
git branch --show-current
```

两秒钟确认。

甚至可以把 prompt 做成明显显示当前 Git 分支，例如：

```text
~/project [main] $
```

或者：

```text
~/project [feature/login] $
```

这样你基本不会出现“写了半天才发现自己在 main”。

---

# 还有一个很重要的点：不要为了“遵守流程”而创建分支

比如你今天突然发现：

> “这里有个变量名写错了。”

如果按照完整 Git Flow：

```text
main
 ↓
develop
 ↓
hotfix/xxx
 ↓
merge
 ↓
develop
 ↓
main
```

……

**一个变量名改错值得这个流程吗？**

当然不值得。

个人仓库可以非常粗暴：

### 大功能

```text
feature/login
feature/search
feature/recommendation
```

### 小修改

直接在当前 feature 分支：

```text
fix typo
fix null pointer
adjust log
update documentation
```

没必要每个 commit 都对应一个 branch。

---

# 那 Hotfix 怎么办？

这个也可以简单化。

假设：

```text
main
  ↓
线上发现严重 bug
```

直接：

```bash
git switch main
git pull
git switch -c fix/critical-bug
```

修完：

```text
fix/critical-bug
       ↓
      main
```

这实际上已经覆盖了传统 Git Flow 的 Hotfix。

你甚至不需要专门维护一个永久 `hotfix` 概念。

---

# Release 分支呢？

个人项目通常也没必要。

如果你需要发布版本：

```bash
git tag v1.0.0
git push origin v1.0.0
```

然后：

```text
main
 │
 ├── v1.0.0
 │
 ├── v1.1.0
 │
 └── v2.0.0
```

**Tag 就已经承担了“版本”的职责。**

Release branch 很多时候是因为：

> “1.0 要发布了，但 develop 还要继续开发。”

多人项目里这个问题很真实。

但你一个人开发：

````text
我：
“develop 还在开发，release 要不要开？”

我：
“开发的人不就是我吗？”

😂

---

# 甚至你可以进一步简化

如果是你自己的小项目，我会直接推荐这种：

```text
main
│
├── feature/xxx
├── feature/yyy
└── fix/zzz
````

日常工作：

```text
             ┌── feature/A ──┐
             │               │
main ────────┼───────────────┼── main
             │               │
             └── feature/B ──┘
```

**只有两个永久状态：**

```text
main = 已经完成的东西
当前 feature = 我正在干的东西
```

这其实非常符合个人开发者的大脑。

---

# 你还可以再进一步：不用“功能分支”，而用“任务分支”

我觉得这个特别适合你。

比如你现在做一个项目：

```text
feature/USM-state-machine
```

你可能干三天。

但如果今天发现：

```text
修复状态机初始化问题
```

可以：

```text
fix/state-machine-init
```

如果只是：

```text
修改 README
```

完全可以直接：

```text
docs/update-readme
```

最终：

```text
main
 ├── feature/usm
 ├── fix/state-machine-init
 └── docs/update-readme
```

**分支是临时工作空间，而不是项目结构。**

这是我觉得非常重要的一点。

---

# 那什么时候 Git Flow 才真正有意义？

假设一个项目：

```text
10 个开发者
        ↓
develop
        ↓
feature/A
feature/B
feature/C
        ↓
release/2.3
        ↓
测试
        ↓
main
```

同时：

```text
main
 ↓
线上 v2.2.1
 ↓
发现严重 bug
 ↓
hotfix/2.2.2
 ↓
main
 ↓
develop
```

这时候 Git Flow 就很有价值。

因为它实际上解决的是：

> **“不同开发阶段、不同发布版本、不同稳定程度的代码如何并行存在？”**

而不是：

> “我今天写一个登录功能应该建几个分支？”

---

## 所以我会给你一个非常现实的个人 Git 规则

你以后自己的仓库，可以直接遵循：

```text
main
│
│   永远保持可运行
│
├── feature/*
│
├── fix/*
│
└── docs/*
```

然后：

**1. 不在 main 上直接开发**

**2. 一个任务一个临时分支**

**3. 做完 → merge main → 删除分支**

**4. 不需要 develop**

**5. 不需要 release branch**

**6. 不需要永久 hotfix branch**

**7. 正式版本用 tag**

**8. 小改动不要为了流程硬开分支**

最后你会发现日常 Git 操作其实就变成了：

```bash
git switch main
git pull

git switch -c feature/xxx

# 干活
git add .
git commit

# 干完
git switch main
git merge --no-ff feature/xxx
git push

git branch -d feature/xxx
```

这套东西对**一个人维护的中小型项目**，我觉得比标准 Git Flow 舒服太多。

而且你现在觉得“切换分支异常繁琐”，我反而建议你**不要试图把 Git Flow 练熟来克服这种感觉**。这个信号更可能是在告诉你：**该删流程了。**

Git 的分支模型本质上是工具，不是宗教。😎