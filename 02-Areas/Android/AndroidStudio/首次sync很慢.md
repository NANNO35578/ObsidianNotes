# 首次sync很慢
换源:
在项目的project视图下, 将`settings.gradle.kts`文件中更新为以下部分:
以阿里云为例

```json
pluginManagement {
    repositories {
        maven { url = uri("https://maven.aliyun.com/repository/gradle-plugin") }
        maven { url = uri("https://maven.aliyun.com/repository/public") }
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        mavenCentral()
        gradlePluginPortal()
    }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        maven { url = uri("https://maven.aliyun.com/repository/public") }
        google()
        mavenCentral()
    }
}

rootProject.name = "0925Android"
include(":app")

```


然后在Build的Sync视图下点击左侧stop然后再继续, 就是使用阿里云下载的了


Android Studio 每次加依赖后都要“sync”很久，本质上是 Gradle 在帮你做三件事：  
1. 解析新的依赖坐标 → 2. 去远程仓库下载对应的 POM/JAR/AAR → 3. 重新计算依赖图并缓存。  
只要其中任何一步慢了，IDE 就会卡在那条 “Gradle: Build running…” 进度条上。

把网上最新（2025 年）的踩坑经验汇总下来，真正能“秒开”的做法只有一句话：  
**“让依赖解析和下载环节全部命中本地缓存，并别让 Gradle 做多余的工作。”**  
具体可以拆成 6 步，按见效快慢排序：

1. 换国内仓库，第一次就把依赖拉回来  
   把 `repositories` 全部换成阿里镜像，Google、MavenCentral 留作 fallback 即可，实测第一次同步能快 3～5 倍。  
   ```gradle
   // settings.gradle(.kts)
   pluginManagement {
       repositories {
           maven("https://maven.aliyun.com/repository/public")
           maven("https://maven.aliyun.com/repository/google")
           google()
           mavenCentral()
       }
   }
   dependencyResolutionManagement {
       repositories {
           maven("https://maven.aliyun.com/repository/public")
           maven("https://maven.aliyun.com/repository/google")
           google()
           mavenCentral()
       }
   }
   ```
   

2. 让 Gradle 常驻 + 并行 + 缓存全开  
   在 `gradle.properties`（项目级或全局 `~/.gradle/gradle.properties`）里一次性加上：
```properties
   org.gradle.daemon=true
   org.gradle.parallel=true
   org.gradle.configureondemand=true
   org.gradle.caching=true
   org.gradle.jvmargs=-Xmx4g -XX:+UseParallelGC
```
   第一条保证后台常驻进程，第二条多线程并发，第三条只配置用到的模块，第四条把构建结果扔进缓存，第五条直接给 4 GB 堆，别抠门。  
   

3. 本地“假装离线”——最暴力的提速  
   只要依赖曾经成功下载过一次，就打开 Android Studio 的 “Offline work” 开关（File → Settings → Build → Gradle → Offline work）。  
   之后无论怎么改 `implementation` 都不会再去远程仓库，sync 基本 3 s 内完成；如果提示找不到，再临时关闭即可。  
   

4. 升级 Gradle + AGP 到 8.x 最新  
   8.x 的依赖解析引擎用上了 Kotlin DSL 缓存和并行图解析，Google 官方数据是同步时间减少 20 %～30 %。  
   升级方法：Update Gradle wrapper 到 8.5（或更高），再把 `com.android.tools.build:gradle` 升到 8.2+ 即可。  
   

5. 精简依赖，别让 Gradle 做“无用图”  
   - 能用 `implementation` 就别用 `api`，减少传递依赖节点。  
   - 定期 `./gradlew app:dependencies > dep.txt` 看一眼，把测试库、调试库用 `debugImplementation` 隔离。  
   - 遇到同一个库多个版本冲突时，在 `build.gradle` 里统一 `force` 或 `exclude`，别让 Gradle 来回试版本。  
   

6. 提前把“大魔王”Gradle 发行版也镜像  
   第一次打开项目时 AS 会去 `https://services.gradle.org` 下几十 MB 的 zip，如果这一步超时就会卡在 “Download gradle-8.x-bin.zip”。  
   手动把 zip 丢到 `~/.gradle/wrapper/dists/gradle-8.x-bin/随机字符串/` 里，或者把 `distributionUrl` 改成国内镜像站（阿里云、清华都有）即可秒过。  
   

把以上 6 步全部做完后，**后续新增依赖的 sync 时间基本能压到 5 s 以内**（离线模式甚至 1 s 内）。  
如果还有极端场景（公司内网仓库、超大单体模块），再考虑：  
- 拆 Feature Module 减小依赖图；  
- 用 Gradle Build Cache Node 做远程缓存；  
- 给 AS 分配 8 GB+ 内存并打开 “Compile independent modules in parallel”。  

照着做就行，别再一直盯着那条蓝色进度条发呆了。