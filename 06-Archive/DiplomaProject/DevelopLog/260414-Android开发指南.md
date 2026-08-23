````markdown title:"GPT 老大哥"
好，这一步非常关键 👍  
你现在需要的不是“更多 prompt”，而是**一套干净、完整、强约束的最终提示词**，让新 Agent 一次性理解你的系统。

我帮你把这几轮对话里的内容**去重 + 重构 + 工程化**，整理成一份：

👉 **可以直接喂给新 Agent 的“终极 Prompt”**

---

# 🚀 Android 客户端开发终极 Prompt（整合版）

```id="android_final_prompt_260414"
你是一个资深 Android 工程师，请在【已有项目基础上】进行全面重构与修复，而不是重新创建项目。

后端 API 文档位于：./docs/api.md  
后端地址：http://192.168.10.1  

--------------------------------
# 一、必须遵守的技术架构

## 1. 架构
- MVVM
- Repository 模式

## 2. 技术栈
- Kotlin
- Jetpack（ViewModel + StateFlow + Navigation）
- Retrofit + OkHttp
- Gson / Kotlinx Serialization
- DataStore（存储 token）
- Markwon（Markdown 渲染）
- Coroutines + Flow

## 3. UI
- XML + ViewBinding（禁止 Compose）

--------------------------------
# 二、核心架构改造（必须完成）

## ❗ Single Activity 架构

当前问题：
多个 Activity 导致返回逻辑混乱

👉 必须修改为：
- 单 Activity（MainActivity）
- 使用 Navigation Component
- 所有页面改为 Fragment：

- LoginFragment
- RegisterFragment
- NoteListFragment
- NoteEditorFragment
- SearchFragment

👉 要求：
- 正确管理 back stack
- 登录后清空登录页

--------------------------------
# 三、必须修复的 BUG

## 1. RecyclerView 不显示
👉 修复：
- 设置 LinearLayoutManager
- 正确绑定 Adapter
- 正确刷新数据

--------------------------------

## 2. BaseUrl 错误
👉 修改为：
http://192.168.10.1

禁止使用 localhost / 127.0.0.1

--------------------------------

## 3. 全面屏适配

👉 必须使用 WindowInsets：

- 顶部避开状态栏
- UI 不可贴边

--------------------------------

## 4. 登录/注册 UI 重做

要求：
- 整体居中
- 屏幕 1/3 显示：

Calcite

- 包含：
  - 用户名
  - 邮箱（注册必须有）
  - 密码
  - 登录 / 注册按钮

--------------------------------

## 5. 多 Activity 问题

👉 必须删除多余 Activity  
👉 全部改为 Fragment

--------------------------------

# 四、主界面重构（核心）

## 1. 整体结构

使用：

DrawerLayout + Navigation + BottomNavigationView

结构：

DrawerLayout
 ├── 主内容（FragmentContainerView）
 ├── 左抽屉（笔记树）
 └── 右抽屉（工具面板）

--------------------------------

## 2. 底部工具栏

按钮：

- 笔记列表（打开左抽屉）
- 新建笔记
- OCR
- 搜索
- 标签管理
- 文件管理

--------------------------------

## 3. 左侧抽屉（笔记树）

结构：

【顶部按钮】
- 新建笔记
- 新建文件夹
- OCR生成

【中部】
- 文件树（必须层级结构）
- 类似 VSCode / el-tree

【底部】
- 用户信息按钮
- 点击：
  - 退出登录

--------------------------------

## 4. 右侧抽屉

包含两个模块：

### 文件管理
- 文件列表
- 状态：
  - 完成
  - 处理中
  - 失败

操作：
- 删除
- 复制链接（ClipboardManager）

### 标签管理
- 标签 CRUD
- 点击绑定当前笔记

--------------------------------

# 五、笔记模块（重点）

## 1. 笔记列表
- 正常展示

## 2. 笔记编辑页面（必须重写）

顶部：
- 标题（可点击编辑）
- 预览按钮

内容：
- EditText（Markdown）
- TextView（Markwon 渲染）

👉 必须实现：
- 编辑 / 预览切换

--------------------------------

## 3. 自动保存（必须实现）

要求：
- 无保存按钮
- 5秒无操作自动保存

👉 必须使用：

Flow + debounce(5000)

禁止频繁请求

--------------------------------

## 4. 标题编辑

- 点击标题 → 变 EditText
- 失焦自动保存

--------------------------------

# 六、标签系统（必须修复）

问题：
当前不可用

👉 必须实现：

进入笔记详情：
- 获取标签列表

UI：
- 显示已绑定标签（选中状态）

点击标签：
- 调用 /api/tag/bind
- 成功后刷新 UI

--------------------------------

# 七、OCR 功能（必须修复）

问题：
不能输入路径

👉 必须实现：

- 使用 ActivityResultContracts.GetContent
- 选择 image/*

上传：
- Multipart
- 调用 /api/ocr/recognize

👉 需要：
- 轮询 OCR 状态
- 完成后生成笔记

--------------------------------

# 八、搜索功能

要求：

- 独立页面
- 实时搜索

👉 高亮：

API 返回包含 <mark>

必须渲染：

- Html.fromHtml 或 Markwon HTML plugin

--------------------------------

# 九、网络层要求

## 1. Retrofit
BaseUrl：
http://192.168.10.1

## 2. Token
必须使用 Interceptor 自动注入：

Authorization: Bearer {token}

--------------------------------

# 十、状态同步（重要）

以下操作必须刷新 UI：

- 标签绑定
- OCR完成
- 笔记更新

--------------------------------

# 十一、代码输出要求

必须：

1. 标明修改文件
2. 给出完整代码
3. 不允许伪代码

--------------------------------

# 十二、文档输出（必须）

在 ./docs 生成：

260413_android_refactor.md

内容：

1. 架构设计
2. BUG 修复说明
3. OCR流程
4. 自动保存实现
5. 标签系统设计

--------------------------------

# 十三、输出顺序

1. 架构改造
2. Navigation实现
3. BUG修复
4. 主界面
5. 编辑器（Markwon）
6. OCR实现
7. 文档
```

---

# ✅ 这一版的核心提升

相比你原来的 prompt，这一版做了这些关键增强：

### 🔒 架构锁死

- Single Activity（避免无限返回问题）
    

### 🔒 UI锁死

- Drawer + BottomNav（不会乱实现）
    

### 🔒 行为锁死

- “按钮 = 打开抽屉” 写死了
    

### 🔒 技术锁死

- OCR 必须用 ActivityResult
    
- 自动保存必须 debounce
    

### 🔒 数据流锁死

- 标签必须“获取 + 显示 + 刷新”
    

---

# 🎯 给你一个很关键的建议

你现在已经具备一个很强的能力了：

👉 **你可以“设计系统 + 驱动 AI 实现”**

下一步如果你想再提升成功率：

👉 用这一套方式：

- Prompt 1：只做架构
    
- Prompt 2：只做 UI
    
- Prompt 3：只接 API
    

成功率会接近 **100%**

---

如果你愿意，我可以帮你把这整套拆成：

👉 **“毕业设计级三阶段 Prompt（极稳定版）”**
````


```
你是一个资深 Android 工程师，请在【已有项目基础上】进行全面重构与修复，而不是重新创建项目。

后端 API 文档位于：./docs/api.md  
后端地址：http://127.0.0.1:8888

--------------------------------
# 一、必须遵守的技术架构

## 1. 架构
- MVVM
- Repository 模式

## 2. 技术栈
- Kotlin
- Jetpack（ViewModel + StateFlow + Navigation）
- Retrofit + OkHttp
- Gson / Kotlinx Serialization
- DataStore（存储 token）
- Markwon（Markdown 渲染）
- Coroutines + Flow

## 3. UI
- XML + ViewBinding（禁止 Compose）

--------------------------------
# 二、核心架构改造（必须完成）

## ❗ Single Activity 架构

当前问题：
多个 Activity 导致返回逻辑混乱

👉 必须修改为：
- 单 Activity（MainActivity）
- 使用 Navigation Component
- 所有页面改为 Fragment：

- LoginFragment
- RegisterFragment
- NoteListFragment
- NoteEditorFragment
- SearchFragment

👉 要求：
- 正确管理 back stack
- 登录后清空登录页

--------------------------------


## 3. 全面屏适配

👉 必须使用 WindowInsets：

- 顶部避开状态栏
- UI 不可贴边

--------------------------------

## 4. 登录/注册 UI

要求：
- 整体居中
- 屏幕 1/3 显示：

Calcite

- 包含：
  - 用户名
  - 邮箱（注册必须有）
  - 密码
  - 登录 / 注册界面切换按钮提示

--------------------------------

## 5. 多 Activity 问题

👉 必须删除多余 Activity  
👉 全部改为 Fragment

--------------------------------

# 四、主界面（核心）

## 1. 整体结构
使用：
DrawerLayout + Navigation + BottomNavigationView
结构：
DrawerLayout
 ├── 主内容（FragmentContainerView）
 ├── 左抽屉（笔记树）
 └── 右抽屉（工具面板）

--------------------------------

## 2. 底部工具栏
按钮：
- 笔记列表（打开左抽屉）
- 新建笔记
- OCR
- 搜索
- 工具按钮(打开右抽屉) 

--------------------------------

## 3. 左侧抽屉（笔记树）
结构：
【顶部按钮】横向排布.
- 新建笔记
- 新建文件夹
- OCR生成

【中部】
- 文件树（必须层级结构）
- 类似 VSCode / el-tree

【底部】
- 用户信息按钮
- 点击：
  - 退出登录

--------------------------------

## 4. 右侧抽屉

包含两个模块：

### 已上传文件管理
- 已上传文件列表
- 状态：
  - 完成
  - 处理中
  - 失败

操作：
- 删除
- 复制链接（ClipboardManager）
- 按状态查询
  
### 标签管理
- 若当前为笔记编辑界面, 则上部分显示当前笔记已绑定标签. 否则显示全部标签.
- 标签 CRUD
- 点击绑定到当前笔记
  
进入笔记详情：
- 获取已绑定标签列表
- 抽屉中显示已绑定标签（选中状态）

--------------------------------

# 五、笔记模块（重点）
## 1. 笔记列表
- 正常展示

## 2. 笔记编辑页面
顶部：
- 标题（可点击编辑）
- 预览按钮, 点击切换为markdown预览.

内容：
- EditText（Markdown）
- TextView（Markwon 渲染）

👉 必须实现：
- 编辑 / 预览切换

--------------------------------

## 3. 自动保存（必须实现）

要求：
- 无保存按钮
- 5秒无操作自动保存

👉 必须使用：
Flow + debounce(5000)
禁止频繁请求

--------------------------------

## 4. 标题编辑
- 点击标题 → 变 EditText
- 失焦自动保存

--------------------------------

# 六、OCR 功能

- 使用 ActivityResultContracts.GetContent
- 选择 image/*

上传：
- Multipart
- 调用 /api/ocr/recognize

👉 需要：
- 轮询 OCR 状态
- 完成后弹窗提示已生成笔记

--------------------------------

# 八、搜索功能
要求：
- 独立页面
- 实时搜索

👉 高亮：
API 返回包含 <mark>
必须渲染：
- Html.fromHtml 或 Markwon HTML plugin

--------------------------------

# 九、网络层要求
## 1. Retrofit
BaseUrl：
http://127.0.0.1:8888

## 2. Token
必须使用 Interceptor 自动注入：

Authorization: Bearer {token}

--------------------------------

# 十、状态同步（重要）

以下操作必须刷新 UI：

- 标签绑定
- OCR完成
- 笔记更新

--------------------------------

# 十一、代码输出要求

必须：

1. 标明修改文件
2. 给出完整代码
3. 不允许伪代码

--------------------------------

# 十二、文档输出（必须）

在 ./docs 生成：

260413_为前缀的标签

内容：

1. 架构设计
2. BUG 修复说明
3. OCR流程
4. 自动保存实现
5. 标签系统设计

--------------------------------

# 十三、输出顺序

1. 架构改造
2. Navigation实现
3. BUG修复
4. 主界面
5. 编辑器（Markwon）
6. OCR实现
7. 文档
```


下面是我修改后的prompt. 


我觉得一次性处理这么多内容agent可能处理不来. 我决定按照如下步骤逐次实现, 请分条给出每次的prompt.
1. 注册登录界面
2. 登录后框架实现(下方工具栏, 左右抽屉), DrawerLayout + Navigation + BottomNavigationView
3. 左侧抽屉部分: 笔记列表, 笔记CRUD与新建笔记, 新建文件夹按钮实现, 文件夹CRUD. 工具栏新建笔记按钮实现
4. 右侧抽屉实现: 标签管理, 已上传文件管理列表, 搜索实现, 搜索高亮实现
5. 文件上传实现:在已上传文件列表中顶部添加一个按钮上传文件. OCR实现.

这些功能./docs/api.md中记录了API的详细使用方法, 每次都强调一次.



# Here we begin

你这个思路是对的 👍 ——**分阶段是唯一能让 Agent 稳定产出的方式**。
我帮你把每一步都“工程化约束”，保证：

* 不会跑偏
* 每一步都能落地
* 下一步可以无缝接上

下面是**5个阶段的可直接使用 Prompt**（已经帮你做了依赖衔接和约束补全）。

---

# 🚀 第1步：注册 / 登录模块

```text title:"ASK"
你是一个资深 Android 工程师，请在【当前空Android项目框架上】实现用户注册和登录模块。

后端 API 文档位于：./docs/api.md  
后端地址：http://127.0.0.1:8888  

--------------------------------
# 一、技术要求（必须遵守）

- MVVM 架构
- Kotlin + ViewBinding
- Retrofit + OkHttp
- DataStore 存储 token
- 使用 Navigation（为后续扩展做准备）

--------------------------------
# 二、功能实现

## 1. 页面
实现：

- LoginFragment
- RegisterFragment

要求：
- 两个页面可相互跳转

--------------------------------

## 2. UI 要求

整体：
- ConstraintLayout
- 垂直居中

顶部（屏幕1/3处）：
- 显示：Calcite

输入项：

登录：
- 用户名
- 密码

注册：
- 用户名
- 邮箱（必须）
- 密码

--------------------------------

## 3. API 接入（严格按照 ./docs/api.md Line:60~168）
实现：
- /api/auth/login 
- /api/auth/register

--------------------------------

## 4. Token 管理

- 登录成功后保存 token（DataStore）
- 使用 OkHttp Interceptor 自动注入：

Authorization: Bearer {token}

--------------------------------

## 5. 登录成功行为

- 跳转到主界面（先用空白 Fragment 占位）
- 清空返回栈（不能返回登录页）

--------------------------------

# 三、代码要求

必须输出：
1. Fragment 代码
2. ViewModel
3. Repository
4. Retrofit 接口
5. Token 存储实现

不得省略代码

--------------------------------

# 四、输出顺序

1. 网络层
2. Repository
3. ViewModel
4. UI（Fragment）

最后将你的操作在./docs中形成以260414_为前缀的文档
```


## ANS1
````markdown title:""
所有代码已实际写入项目文件系统，并已通过 `./gradlew.bat build` **编译成功**。完整文档已生成在 `./docs/260414_auth_implementation.md`。

以下按你要求的输出顺序，列出全部核心代码（无省略）：

---

## 一、网络层

### 1. 数据模型
**路径：** `app/src/main/java/com/calcite/notes/model/AuthModels.kt`
```kotlin
package com.calcite.notes.model

data class ApiResponse<T>(
    val code: Int,
    val message: String,
    val data: T?
)

data class LoginRequest(
    val username: String,
    val password: String
)

data class RegisterRequest(
    val username: String,
    val email: String,
    val password: String
)

data class LoginData(
    val user_id: Long,
    val username: String,
    val token: String
)

data class RegisterData(
    val user_id: Long,
    val token: String
)
```

### 2. Retrofit 接口
**路径：** `app/src/main/java/com/calcite/notes/data/remote/ApiService.kt`
```kotlin
package com.calcite.notes.data.remote

import com.calcite.notes.model.ApiResponse
import com.calcite.notes.model.LoginData
import com.calcite.notes.model.LoginRequest
import com.calcite.notes.model.RegisterData
import com.calcite.notes.model.RegisterRequest
import retrofit2.http.Body
import retrofit2.http.POST

interface ApiService {

    @POST("/api/auth/login")
    suspend fun login(@Body request: LoginRequest): ApiResponse<LoginData>

    @POST("/api/auth/register")
    suspend fun register(@Body request: RegisterRequest): ApiResponse<RegisterData>
}
```

### 3. Token 存储（DataStore）
**路径：** `app/src/main/java/com/calcite/notes/data/local/TokenDataStore.kt`
```kotlin
package com.calcite.notes.data.local

import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map

class TokenDataStore(private val context: Context) {

    companion object {
        private val Context.dataStore: DataStore<Preferences> by preferencesDataStore(name = "auth_prefs")
        private val TOKEN_KEY = stringPreferencesKey("token")
    }

    val token: Flow<String?> = context.dataStore.data.map { preferences ->
        preferences[TOKEN_KEY]
    }

    suspend fun saveToken(token: String) {
        context.dataStore.edit { preferences ->
            preferences[TOKEN_KEY] = token
        }
    }

    suspend fun clearToken() {
        context.dataStore.edit { preferences ->
            preferences.remove(TOKEN_KEY)
        }
    }
}
```

### 4. OkHttp 拦截器（自动注入 Bearer Token）
**路径：** `app/src/main/java/com/calcite/notes/data/remote/AuthInterceptor.kt`
```kotlin
package com.calcite.notes.data.remote

import com.calcite.notes.data.local.TokenDataStore
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.runBlocking
import okhttp3.Interceptor

class AuthInterceptor(private val tokenDataStore: TokenDataStore) : Interceptor {

    override fun intercept(chain: Interceptor.Chain): okhttp3.Response {
        val request = chain.request()
        val token = runBlocking { tokenDataStore.token.first() }

        val newRequest = if (!token.isNullOrEmpty()) {
            request.newBuilder()
                .addHeader("Authorization", "Bearer $token")
                .build()
        } else {
            request
        }

        return chain.proceed(newRequest)
    }
}
```

### 5. Retrofit 单例
**路径：** `app/src/main/java/com/calcite/notes/data/remote/RetrofitClient.kt`
```kotlin
package com.calcite.notes.data.remote

import android.content.Context
import com.calcite.notes.data.local.TokenDataStore
import okhttp3.OkHttpClient
import okhttp3.logging.HttpLoggingInterceptor
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

object RetrofitClient {

    private const val BASE_URL = "http://127.0.0.1:8888"

    fun getApiService(context: Context): ApiService {
        val tokenDataStore = TokenDataStore(context)

        val loggingInterceptor = HttpLoggingInterceptor().apply {
            level = HttpLoggingInterceptor.Level.BODY
        }

        val client = OkHttpClient.Builder()
            .addInterceptor(AuthInterceptor(tokenDataStore))
            .addInterceptor(loggingInterceptor)
            .build()

        return Retrofit.Builder()
            .baseUrl(BASE_URL)
            .client(client)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
            .create(ApiService::class.java)
    }
}
```

---

## 二、Repository

### 结果封装
**路径：** `app/src/main/java/com/calcite/notes/utils/Result.kt`
```kotlin
package com.calcite.notes.utils

sealed class Result<out T> {
    data class Success<out T>(val data: T) : Result<T>()
    data class Error(val message: String) : Result<Nothing>()
    data object Loading : Result<Nothing>()
}
```

### AuthRepository
**路径：** `app/src/main/java/com/calcite/notes/data/repository/AuthRepository.kt`
```kotlin
package com.calcite.notes.data.repository

import com.calcite.notes.data.local.TokenDataStore
import com.calcite.notes.data.remote.ApiService
import com.calcite.notes.model.LoginData
import com.calcite.notes.model.LoginRequest
import com.calcite.notes.model.RegisterData
import com.calcite.notes.model.RegisterRequest
import com.calcite.notes.utils.Result

class AuthRepository(
    private val apiService: ApiService,
    private val tokenDataStore: TokenDataStore
) {

    suspend fun login(username: String, password: String): Result<LoginData> {
        return try {
            val response = apiService.login(LoginRequest(username, password))
            if (response.code == 0 && response.data != null) {
                tokenDataStore.saveToken(response.data.token)
                Result.Success(response.data)
            } else {
                Result.Error(response.message)
            }
        } catch (e: Exception) {
            Result.Error(e.message ?: "网络请求失败")
        }
    }

    suspend fun register(username: String, email: String, password: String): Result<RegisterData> {
        return try {
            val response = apiService.register(RegisterRequest(username, email, password))
            if (response.code == 0 && response.data != null) {
                tokenDataStore.saveToken(response.data.token)
                Result.Success(response.data)
            } else {
                Result.Error(response.message)
            }
        } catch (e: Exception) {
            Result.Error(e.message ?: "网络请求失败")
        }
    }
}
```

---

## 三、ViewModel

### LoginViewModel
**路径：** `app/src/main/java/com/calcite/notes/ui/login/LoginViewModel.kt`
```kotlin
package com.calcite.notes.ui.login

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import com.calcite.notes.data.repository.AuthRepository
import com.calcite.notes.model.LoginData
import com.calcite.notes.utils.Result
import kotlinx.coroutines.launch

class LoginViewModel(private val repository: AuthRepository) : ViewModel() {

    private val _loginResult = MutableLiveData<Result<LoginData>>()
    val loginResult: LiveData<Result<LoginData>> = _loginResult

    fun login(username: String, password: String) {
        if (username.isBlank() || password.isBlank()) {
            _loginResult.value = Result.Error("用户名和密码不能为空")
            return
        }

        _loginResult.value = Result.Loading
        viewModelScope.launch {
            _loginResult.value = repository.login(username, password)
        }
    }

    class Factory(private val repository: AuthRepository) : ViewModelProvider.Factory {
        @Suppress("UNCHECKED_CAST")
        override fun <T : ViewModel> create(modelClass: Class<T>): T {
            return LoginViewModel(repository) as T
        }
    }
}
```

### RegisterViewModel
**路径：** `app/src/main/java/com/calcite/notes/ui/register/RegisterViewModel.kt`
```kotlin
package com.calcite.notes.ui.register

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import com.calcite.notes.data.repository.AuthRepository
import com.calcite.notes.model.RegisterData
import com.calcite.notes.utils.Result
import kotlinx.coroutines.launch

class RegisterViewModel(private val repository: AuthRepository) : ViewModel() {

    private val _registerResult = MutableLiveData<Result<RegisterData>>()
    val registerResult: LiveData<Result<RegisterData>> = _registerResult

    fun register(username: String, email: String, password: String) {
        if (username.isBlank() || password.isBlank()) {
            _registerResult.value = Result.Error("用户名和密码不能为空")
            return
        }
        if (email.isBlank()) {
            _registerResult.value = Result.Error("邮箱不能为空")
            return
        }

        _registerResult.value = Result.Loading
        viewModelScope.launch {
            _registerResult.value = repository.register(username, email, password)
        }
    }

    class Factory(private val repository: AuthRepository) : ViewModelProvider.Factory {
        @Suppress("UNCHECKED_CAST")
        override fun <T : ViewModel> create(modelClass: Class<T>): T {
            return RegisterViewModel(repository) as T
        }
    }
}
```

---

## 四、UI（Fragment + Activity + Navigation）

### MainActivity
**路径：** `app/src/main/java/com/calcite/notes/MainActivity.kt`
```kotlin
package com.calcite.notes

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.calcite.notes.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
    }
}
```

### LoginFragment
**路径：** `app/src/main/java/com/calcite/notes/ui/login/LoginFragment.kt`
```kotlin
package com.calcite.notes.ui.login

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.fragment.app.viewModels
import androidx.navigation.fragment.findNavController
import com.calcite.notes.R
import com.calcite.notes.data.local.TokenDataStore
import com.calcite.notes.data.remote.RetrofitClient
import com.calcite.notes.data.repository.AuthRepository
import com.calcite.notes.databinding.FragmentLoginBinding
import com.calcite.notes.utils.Result

class LoginFragment : Fragment() {

    private var _binding: FragmentLoginBinding? = null
    private val binding get() = _binding!!

    private val viewModel: LoginViewModel by viewModels {
        val apiService = RetrofitClient.getApiService(requireContext())
        val tokenDataStore = TokenDataStore(requireContext())
        val repository = AuthRepository(apiService, tokenDataStore)
        LoginViewModel.Factory(repository)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
n        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentLoginBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnLogin.setOnClickListener {
            val username = binding.etUsername.text.toString().trim()
            val password = binding.etPassword.text.toString().trim()
            viewModel.login(username, password)
        }

        binding.tvGoRegister.setOnClickListener {
            findNavController().navigate(R.id.action_loginFragment_to_registerFragment)
        }

        viewModel.loginResult.observe(viewLifecycleOwner) { result ->
            when (result) {
                is Result.Loading -> {
                    binding.btnLogin.isEnabled = false
                    binding.progressBar.visibility = View.VISIBLE
                }

                is Result.Success -> {
                    binding.btnLogin.isEnabled = true
                    binding.progressBar.visibility = View.GONE
                    Toast.makeText(requireContext(), "登录成功", Toast.LENGTH_SHORT).show()
                    findNavController().navigate(R.id.action_loginFragment_to_homeFragment)
                }

                is Result.Error -> {
                    binding.btnLogin.isEnabled = true
                    binding.progressBar.visibility = View.GONE
                    Toast.makeText(requireContext(), result.message, Toast.LENGTH_LONG).show()
                }
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
```

### RegisterFragment
**路径：** `app/src/main/java/com/calcite/notes/ui/register/RegisterFragment.kt`
```kotlin
package com.calcite.notes.ui.register

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.fragment.app.viewModels
import androidx.navigation.fragment.findNavController
import com.calcite.notes.R
import com.calcite.notes.data.local.TokenDataStore
import com.calcite.notes.data.remote.RetrofitClient
import com.calcite.notes.data.repository.AuthRepository
import com.calcite.notes.databinding.FragmentRegisterBinding
import com.calcite.notes.utils.Result

class RegisterFragment : Fragment() {

    private var _binding: FragmentRegisterBinding? = null
    private val binding get() = _binding!!

    private val viewModel: RegisterViewModel by viewModels {
        val apiService = RetrofitClient.getApiService(requireContext())
        val tokenDataStore = TokenDataStore(requireContext())
        val repository = AuthRepository(apiService, tokenDataStore)
        RegisterViewModel.Factory(repository)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentRegisterBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnRegister.setOnClickListener {
            val username = binding.etUsername.text.toString().trim()
            val email = binding.etEmail.text.toString().trim()
            val password = binding.etPassword.text.toString().trim()
            viewModel.register(username, email, password)
        }

        binding.tvGoLogin.setOnClickListener {
            findNavController().navigateUp()
        }

        viewModel.registerResult.observe(viewLifecycleOwner) { result ->
            when (result) {
                is Result.Loading -> {
                    binding.btnRegister.isEnabled = false
                    binding.progressBar.visibility = View.VISIBLE
                }

                is Result.Success -> {
                    binding.btnRegister.isEnabled = true
                    binding.progressBar.visibility = View.GONE
                    Toast.makeText(requireContext(), "注册成功", Toast.LENGTH_SHORT).show()
                    findNavController().navigate(R.id.action_registerFragment_to_homeFragment)
                }

                is Result.Error -> {
                    binding.btnRegister.isEnabled = true
                    binding.progressBar.visibility = View.GONE
                    Toast.makeText(requireContext(), result.message, Toast.LENGTH_LONG).show()
                }
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
```

### HomeFragment（登录成功后跳转的空白主界面）
**路径：** `app/src/main/java/com/calcite/notes/ui/home/HomeFragment.kt`
```kotlin
package com.calcite.notes.ui.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.calcite.notes.databinding.FragmentHomeBinding

class HomeFragment : Fragment() {

    private var _binding: FragmentHomeBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentHomeBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        binding.tvTitle.text = "主界面（占位）"
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
```

### 导航图（关键：清空返回栈）
**路径：** `app/src/main/res/navigation/nav_graph.xml`
```xml
<?xml version="1.0" encoding="utf-8"?>
<navigation xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    android:id="@+id/nav_graph"
    app:startDestination="@id/loginFragment">

    <fragment
        android:id="@+id/loginFragment"
        android:name="com.calcite.notes.ui.login.LoginFragment"
        android:label="LoginFragment">
        <action
            android:id="@+id/action_loginFragment_to_registerFragment"
            app:destination="@id/registerFragment" />
        <action
            android:id="@+id/action_loginFragment_to_homeFragment"
            app:destination="@id/homeFragment"
            app:popUpTo="@id/nav_graph"
            app:popUpToInclusive="true" />
    </fragment>

    <fragment
        android:id="@+id/registerFragment"
        android:name="com.calcite.notes.ui.register.RegisterFragment"
        android:label="RegisterFragment">
        <action
            android:id="@+id/action_registerFragment_to_homeFragment"
            app:destination="@id/homeFragment"
            app:popUpTo="@id/nav_graph"
            app:popUpToInclusive="true" />
    </fragment>

    <fragment
        android:id="@+id/homeFragment"
        android:name="com.calcite.notes.ui.home.HomeFragment"
        android:label="HomeFragment" />

</navigation>
```

### 布局文件说明
- `app/src/main/res/layout/activity_main.xml`：`FragmentContainerView` + `NavHostFragment`
- `app/src/main/res/layout/fragment_login.xml`：`ConstraintLayout`，标题在 1/3 处，用户名/密码输入框，登录按钮，去注册链接
- `app/src/main/res/layout/fragment_register.xml`：`ConstraintLayout`，标题在 1/3 处，用户名/邮箱/密码输入框，注册按钮，去登录链接
- `app/src/main/res/layout/fragment_home.xml`：空白主界面占位

---

## 五、其他关键配置

### AndroidManifest.xml
已添加 `INTERNET` 权限、`usesCleartextTraffic="true"`（支持 HTTP），并声明了 `MainActivity` 为入口。

### Gradle 依赖
已在 `libs.versions.toml` 和 `app/build.gradle.kts` 中完整引入 Navigation、Retrofit、OkHttp、DataStore、Lifecycle、Coroutines、ConstraintLayout 等依赖，并开启 `viewBinding = true`。

---

## 六、文档

**路径：** `./docs/260414_auth_implementation.md`

该文档汇总了本次所有代码实现、模块结构、API 对接说明、编译结果及模拟器运行注意事项（如使用模拟器需将 `BASE_URL` 改为 `http://10.0.2.2:8888`）。
````


---

# 🚀 第2步：主框架（Drawer + BottomNav）

```text title:"ASK"
请在【已有登录功能基础上】实现主界面框架。

后端 API 文档位于：./docs/api.md（本阶段无需重点使用）

--------------------------------
# 一、架构要求（必须）

- Single Activity（MainActivity）
- Navigation Component
- Fragment 架构

--------------------------------
# 二、主界面结构（必须实现）

使用：

DrawerLayout + BottomNavigationView + Navigation

结构：

DrawerLayout
 ├── 主内容（FragmentContainerView）
 ├── 左抽屉（笔记树，占位）
 └── 右抽屉（工具，占位）

--------------------------------

# 三、底部工具栏

按钮：

- 笔记列表（打开左抽屉）
- 新建笔记（暂时Toast）
- OCR（暂时Toast）
- 搜索（跳转空Fragment）
- 工具（打开右抽屉）

--------------------------------

# 四、Navigation

必须实现：

- NoteListFragment（占位）
- NoteEditorFragment（占位）
- SearchFragment（占位）

--------------------------------

# 五、交互要求

- 点击“笔记列表” → 打开左抽屉
- 点击“工具” → 打开右抽屉
- Fragment 切换通过 Navigation

--------------------------------

# 六、全面屏适配（必须）

使用 WindowInsets 处理状态栏

--------------------------------

# 七、代码要求

必须输出：

1. MainActivity
2. navigation.xml
3. DrawerLayout XML
4. BottomNavigationView 配置

最后将你的操作在./docs中形成以260414_为前缀的文档
```

## ANS2
````markdown title:"ANS"
主界面框架已全部实现并编译通过（`BUILD SUCCESSFUL`）。完整文档已生成在 `./docs/260414_main_framework.md`。

以下按要求的输出顺序列出全部代码（无省略）：

---

## 一、MainActivity

**路径：** `app/src/main/java/com/calcite/notes/MainActivity.kt`

```kotlin
package com.calcite.notes

import android.os.Bundle
import android.widget.Toast
import androidx.activity.addCallback
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.GravityCompat
import androidx.navigation.NavController
import androidx.navigation.fragment.NavHostFragment
import com.calcite.notes.databinding.ActivityMainBinding
import com.calcite.notes.ui.main.NoteListFragment

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var navController: NavController

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // 设置 NavController
        val navHostFragment = supportFragmentManager
            .findFragmentById(R.id.nav_host_fragment) as NavHostFragment
        navController = navHostFragment.navController

        // 将 NoteListFragment 嵌入左抽屉
        if (savedInstanceState == null) {
            supportFragmentManager.beginTransaction()
                .replace(R.id.left_drawer, NoteListFragment())
                .commit()
        }

        // 底部导航栏点击事件
        binding.bottomNav.setOnItemSelectedListener { item ->
            when (item.itemId) {
                R.id.menu_notes -> {
                    binding.drawerLayout.openDrawer(GravityCompat.START)
                    true
                }
                R.id.menu_new_note -> {
                    Toast.makeText(this, "新建笔记", Toast.LENGTH_SHORT).show()
                    true
                }
                R.id.menu_ocr -> {
                    Toast.makeText(this, "OCR", Toast.LENGTH_SHORT).show()
                    true
                }
                R.id.menu_search -> {
                    if (navController.currentDestination?.id != R.id.searchFragment) {
                        navController.navigate(R.id.action_global_searchFragment)
                    }
                    true
                }
                R.id.menu_tools -> {
                    binding.drawerLayout.openDrawer(GravityCompat.END)
                    true
                }
                else -> false
            }
        }

        // 根据当前页面控制 BottomNavigationView 显隐
        navController.addOnDestinationChangedListener { _, destination, _ ->
            val isAuthPage = destination.id == R.id.loginFragment || destination.id == R.id.registerFragment
            binding.bottomNav.visibility = if (isAuthPage) android.view.View.GONE else android.view.View.VISIBLE
        }

        // 全面屏适配：状态栏与导航栏 insets
        ViewCompat.setOnApplyWindowInsetsListener(binding.mainContent) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        // 返回键分发：优先关闭抽屉
        onBackPressedDispatcher.addCallback(this) {
            when {
                binding.drawerLayout.isDrawerOpen(GravityCompat.START) -> {
                    binding.drawerLayout.closeDrawer(GravityCompat.START)
                }
                binding.drawerLayout.isDrawerOpen(GravityCompat.END) -> {
                    binding.drawerLayout.closeDrawer(GravityCompat.END)
                }
                else -> {
                    isEnabled = false
                    onBackPressedDispatcher.onBackPressed()
                    isEnabled = true
                }
            }
        }
    }
}
```

---

## 二、navigation.xml

**路径：** `app/src/main/res/navigation/nav_graph.xml`

```xml
<?xml version="1.0" encoding="utf-8"?>
<navigation xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    android:id="@+id/nav_graph"
    app:startDestination="@id/loginFragment">

    <!-- 认证页 -->
    <fragment
        android:id="@+id/loginFragment"
        android:name="com.calcite.notes.ui.login.LoginFragment"
        android:label="LoginFragment">
        <action
            android:id="@+id/action_loginFragment_to_registerFragment"
            app:destination="@id/registerFragment" />
        <action
            android:id="@+id/action_loginFragment_to_noteEditorFragment"
            app:destination="@id/noteEditorFragment"
            app:popUpTo="@id/nav_graph"
            app:popUpToInclusive="true" />
    </fragment>

    <fragment
        android:id="@+id/registerFragment"
        android:name="com.calcite.notes.ui.register.RegisterFragment"
        android:label="RegisterFragment">
        <action
            android:id="@+id/action_registerFragment_to_noteEditorFragment"
            app:destination="@id/noteEditorFragment"
            app:popUpTo="@id/nav_graph"
            app:popUpToInclusive="true" />
    </fragment>

    <!-- 主界面 Fragment -->
    <fragment
        android:id="@+id/noteEditorFragment"
        android:name="com.calcite.notes.ui.main.NoteEditorFragment"
        android:label="NoteEditorFragment" />

    <fragment
        android:id="@+id/noteListFragment"
        android:name="com.calcite.notes.ui.main.NoteListFragment"
        android:label="NoteListFragment" />

    <fragment
        android:id="@+id/searchFragment"
        android:name="com.calcite.notes.ui.main.SearchFragment"
        android:label="SearchFragment" />

    <!-- 全局搜索跳转 -->
    <action
        android:id="@+id/action_global_searchFragment"
        app:destination="@id/searchFragment" />

</navigation>
```

---

## 三、DrawerLayout XML

**路径：** `app/src/main/res/layout/activity_main.xml`

```xml
<?xml version="1.0" encoding="utf-8"?>
<androidx.drawerlayout.widget.DrawerLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    android:id="@+id/drawer_layout"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:fitsSystemWindows="true">

    <!-- 主内容区 -->
    <androidx.constraintlayout.widget.ConstraintLayout
        android:id="@+id/main_content"
        android:layout_width="match_parent"
        android:layout_height="match_parent">

        <androidx.fragment.app.FragmentContainerView
            android:id="@+id/nav_host_fragment"
            android:name="androidx.navigation.fragment.NavHostFragment"
            android:layout_width="0dp"
            android:layout_height="0dp"
            app:defaultNavHost="true"
            app:layout_constraintBottom_toTopOf="@id/bottom_nav"
            app:layout_constraintEnd_toEndOf="parent"
            app:layout_constraintStart_toStartOf="parent"
            app:layout_constraintTop_toTopOf="parent"
            app:navGraph="@navigation/nav_graph" />

        <com.google.android.material.bottomnavigation.BottomNavigationView
            android:id="@+id/bottom_nav"
            android:layout_width="0dp"
            android:layout_height="wrap_content"
            android:background="?android:attr/windowBackground"
            app:layout_constraintBottom_toBottomOf="parent"
            app:layout_constraintEnd_toEndOf="parent"
            app:layout_constraintStart_toStartOf="parent"
            app:menu="@menu/bottom_nav_menu" />

    </androidx.constraintlayout.widget.ConstraintLayout>

    <!-- 左抽屉：笔记树 -->
    <FrameLayout
        android:id="@+id/left_drawer"
        android:layout_width="300dp"
        android:layout_height="match_parent"
        android:layout_gravity="start"
        android:background="?android:attr/colorBackground"
        android:fitsSystemWindows="true" />

    <!-- 右抽屉：工具 -->
    <FrameLayout
        android:id="@+id/right_drawer"
        android:layout_width="260dp"
        android:layout_height="match_parent"
        android:layout_gravity="end"
        android:background="?android:attr/colorBackground"
        android:fitsSystemWindows="true">

        <TextView
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_gravity="center"
            android:text="工具面板（占位）"
            android:textSize="18sp" />
    </FrameLayout>

</androidx.drawerlayout.widget.DrawerLayout>
```

---

## 四、BottomNavigationView 配置

**路径：** `app/src/main/res/menu/bottom_nav_menu.xml`

```xml
<?xml version="1.0" encoding="utf-8"?>
<menu xmlns:android="http://schemas.android.com/apk/res/android">

    <item
        android:id="@+id/menu_notes"
        android:icon="@drawable/ic_launcher_foreground"
        android:title="笔记列表" />

    <item
        android:id="@+id/menu_new_note"
        android:icon="@drawable/ic_launcher_foreground"
        android:title="新建笔记" />

    <item
        android:id="@+id/menu_ocr"
        android:icon="@drawable/ic_launcher_foreground"
        android:title="OCR" />

    <item
        android:id="@+id/menu_search"
        android:icon="@drawable/ic_launcher_foreground"
        android:title="搜索" />

    <item
        android:id="@+id/menu_tools"
        android:icon="@drawable/ic_launcher_foreground"
        android:title="工具" />

</menu>
```

---

## 五、补充说明

- **左抽屉内容：** 在 `MainActivity` 中通过 `supportFragmentManager.beginTransaction().replace(R.id.left_drawer, NoteListFragment()).commit()` 将 `NoteListFragment` 嵌入左抽屉。
- **登录/注册跳转：** 登录与注册成功后均跳转到 `noteEditorFragment`，并通过 `app:popUpTo="@id/nav_graph" app:popUpToInclusive="true"` 清空返回栈，禁止返回到登录页。
- **底部栏显隐控制：** `MainActivity` 通过 `addOnDestinationChangedListener` 判断当前是否为登录/注册页，自动隐藏/显示 `BottomNavigationView`。
- **返回键处理：** 使用 `OnBackPressedDispatcher` 替代已废弃的 `onBackPressed()`，优先关闭抽屉，再执行默认返回。
- **全面屏适配：** 通过 `WindowInsetsCompat` 获取系统栏高度，并设置给 `main_content` 的 padding，避免内容被状态栏或手势导航条遮挡。

---

**文档位置：** `./docs/260414_main_framework.md`
````
---

# 🚀 第3步：左抽屉 + 笔记系统

```text title:"ASK"
请在【已有主框架基础上】实现左侧抽屉与笔记系统。

后端 API 文档位于：./docs/api.md（必须严格遵守）

--------------------------------
# 一、功能目标

实现：
- 笔记列表
- 笔记 CRUD
- 文件夹 CRUD
- 文件树结构

--------------------------------

# 二、左侧抽屉 UI

结构：

【顶部按钮（横向）】
- 新建笔记
- 新建文件夹
- OCR生成（暂不实现）

【中部】
- 文件树（层级结构）
- RecyclerView + 多类型 ViewHolder
- 一定是类似el-tree的样子

【底部】
- 用户信息按钮, 显示用户信息. 点击退出登录

--------------------------------

# 三、API（必须实现）
笔记： ./docs/api.md Line:170~337
- /api/note/list
- /api/note/create
- /api/note/update
- /api/note/delete

文件夹：./docs/api.md Line:588~712
- /api/folder/create
- /api/folder/list
- /api/folder/update
- /api/folder/delete

--------------------------------

# 四、功能要求

## 1. 文件树
- 支持展开/折叠
- 文件夹 + 笔记混合

## 2. 新建笔记
- 可选择父文件夹

## 3. 点击笔记
- 跳转 NoteEditorFragment

## 4. 工具栏“新建笔记”
- 直接创建并进入编辑页
  
## 5. 笔记编辑界面NoteEditorFragment
- 使用markdwon实现markdown预览
- 顶部为标题, 点击标题可修改. 标题右侧是预览按钮, 点击切换预览
- 每5s自动保存, 有未保存内容提示未保存

## 6. 笔记列表
- 登录后显示根目录下所有笔记.
- 点击笔记进入笔记编辑界面

--------------------------------

# 五、代码要求

概括输出下面内容的实现思路

1. RecyclerView Adapter（树结构）
2. ViewModel
3. API 接口
4. Fragment 实现
5. 数据结构设计
6. API
7. ViewModel
8. UI实现
   
最后将你的操作在./docs中形成以260414_为前缀的文档
```

## ANS3
````markdown title:"ANS"
左侧抽屉与笔记系统已全部实现，编译通过（`BUILD SUCCESSFUL`）。完整文档已生成在 `./docs/260414_note_system.md`。

以下是按你要求的 8 点实现思路概括：

---

### 1. 数据结构设计

采用**扁平化树节点模型**适配 `RecyclerView`：

```kotlin
sealed class TreeNode {
    abstract val level: Int
    data class FolderNode(val folder: Folder, override val level: Int, var isExpanded: Boolean) : TreeNode()
    data class NoteNode(val note: Note, override val level: Int) : TreeNode()
}
```

- `level` 控制左侧缩进（`16 + level * 32`），在视觉上呈现层级。
- `FolderNode` 的 `isExpanded` 标记展开状态。
- `RecyclerView` 只维护“当前可见节点”的线性列表，折叠时移除子节点，展开时插入子节点。

---

### 2. RecyclerView Adapter（树结构）

- `TreeAdapter` 继承 `ListAdapter<TreeNode, VH>(DiffUtil)`。
- 多类型 `ViewHolder`：
  - `FolderViewHolder`：箭头（旋转 90°/0°）+ 文件夹图标 + 名称，点击展开/折叠，长按弹出菜单（重命名/删除）。
  - `NoteViewHolder`：文档图标 + 标题，点击跳转到编辑器。
- `DiffUtil` 以 `id + level` 作为唯一标识，确保同一文件夹在不同层级可重复出现。

---

### 3. API 接口

严格遵循 `./docs/api.md`，在 `ApiService` 中新增：

- **笔记**：`createNote`、`updateNote`、`deleteNote`、`getNoteList`、`getNoteDetail`
- **文件夹**：`createFolder`、`updateFolder`、`deleteFolder`、`getFolderList`
- **用户**：`getUserProfile`（左下角显示用户名）

所有接口均通过已有的 `AuthInterceptor` 自动注入 `Authorization: Bearer {token}`。

---

### 4. Repository 实现思路

按模块拆分为三个 Repository，统一返回 `Result<T>`：

- **`NoteRepository`**：封装笔记 CRUD + list/detail。
- **`FolderRepository`**：封装文件夹 CRUD + list。
- **`UserRepository`**：获取用户信息 + `TokenDataStore.clearToken()` 退出登录。

全部包裹 `try-catch`，网络异常统一降级为 `Result.Error`。

---

### 5. ViewModel（左侧抽屉）

`NoteListViewModel` 维护：

- `_treeNodes: LiveData<List<TreeNode>>`
- `childrenCache: Map<Long, List<TreeNode>>`（已加载子节点缓存，避免重复请求）
- `expandedFolders: Set<Long>`

**核心逻辑 `toggleFolder`：**
- **折叠**：删除该节点后所有 `level > 当前 level` 的连续节点。
- **展开**：若 `cache` 命中直接插入；否则异步调用 `getFolderList` + `getNoteList`，将子文件夹和子笔记转为 `TreeNode(level+1)`，排序后插入列表并写入缓存。

**新建/删除/重命名后**：调用 `refreshAffectedFolder(folderId)`，清除对应缓存并触发重新加载。

---

### 6. ViewModel（笔记编辑器）

`NoteEditorViewModel` 维护：

- `noteDetail`：当前标题与正文
- `isPreview`：编辑/预览模式切换
- `hasUnsavedChanges`：通过比对 `lastSavedTitle/Content` 与当前内容得出

**自动保存：**
```kotlin
viewModelScope.launch {
    while (true) {
        delay(5000)
        if (hasUnsavedChanges && noteId != 0L) saveNote()
    }
}
```
- `onPause` 时强制保存一次。
- 保存结果通过 `saveResult` LiveData 回传 UI，成功时 Toast "已自动保存"。

---

### 7. Fragment 实现

#### NoteListFragment（左侧抽屉）
- **顶部**：横向按钮区（新建笔记 / 新建文件夹 / OCR占位）。
- **中部**：`RecyclerView` 挂载 `TreeAdapter`。
- **底部**：用户头像占位 + 用户名 + 退出登录按钮。
- **Dialog**：新建笔记/文件夹时弹出 AlertDialog，内含 EditText（名称）+ Spinner（父文件夹选择，数据源来自 `getAllFolders()`）。
- **文件夹长按菜单**：重命名 / 删除。
- **退出登录**：清除 Token → `navigate(R.id.loginFragment)`。

#### NoteEditorFragment（Markdown 编辑器）
- **顶部标题栏**：可点击标题（弹出 Dialog 修改）+ 预览/编辑切换按钮。
- **未保存提示**：右上角红色 "未保存" `TextView`，由 `hasUnsavedChanges` 控制显隐。
- **中部**：`FrameLayout` 内嵌 `EditText`（编辑）和 `NestedScrollView+TextView`（预览），通过 `visibility` 切换。
- **Markdown 渲染**：使用 `io.noties.markwon:core`，调用 `markwon.setMarkdown(tvPreview, content)`。

---

### 8. UI 实现与整合

- **DrawerLayout**：左抽屉 `300dp` 宽，内嵌 `NoteListFragment`；右抽屉 `260dp` 宽，目前为工具占位。
- **MainActivity 底部“新建笔记”**：点击后直接调用 `NoteRepository.createNote("未命名笔记", "")`，成功后携带 `noteId` 跳转到 `NoteEditorFragment`。
- **全面屏适配**：继续使用 `WindowInsetsCompat` 为 `main_content` 设置系统栏 padding。
- **返回键**：`OnBackPressedDispatcher` 优先关闭抽屉，再执行默认返回。

---

**编译结果：** `BUILD SUCCESSFUL`

**文档位置：** `./docs/260414_note_system.md`（含全部代码、结构说明与 API 对接细节）
````

---

# 🚀 第4步：右抽屉 + 标签 + 搜索

```text  title:"ASK"
请在【已有笔记系统基础上】实现右侧抽屉功能。

后端 API 文档位于：./docs/api.md（必须使用）

--------------------------------
# 一、功能模块

实现：

1. 标签管理
2. 已上传文件列表（请求&展示）
3. 搜索功能（重点）

--------------------------------

# 二、右抽屉结构

包含两个区域：

## 标签管理

- 显示全部标签
- 若当前在编辑笔记：
  - 显示已绑定标签（选中状态）

操作：
- 创建 / 删除 / 修改
- 点击绑定笔记

API： ./docs/api.md Line:444~588
- /api/tag/list
- /api/tag/create
- /api/tag/update
- /api/tag/delete
- /api/tag/bind

--------------------------------

## 已上传文件列表

API：./docs/api.md Line:793~847
- /api/file/list 

显示：
- 文件名
- 状态（完成/处理中/失败）

--------------------------------

# 三、搜索功能（重点）

API：./docs/api.md Line:337~380
- /api/note/search

要求：

- 独立页面
- 输入实时搜索

## 高亮（必须）

API 返回包含：
`<mark></mark>`

👉 必须渲染：

Html.fromHtml()或 Markwon HTML plugin

--------------------------------

# 四、交互要求

- 标签绑定后立即刷新 UI
- 搜索结果点击进入编辑页

--------------------------------

# 五、代码要求
概括以下实现思路:
- 标签模块完整实现
- 搜索页面
- 高亮实现

最后将你的操作在./docs中形成以260414_为前缀的文档
```


## ANS4
````markdown title:"ANS"
右侧抽屉与搜索功能已全部实现，编译通过（`BUILD SUCCESSFUL`）。完整文档已生成在 `./docs/260414_right_drawer.md`。

以下按你要求的 3 点概括实现思路：

---

## 1. 标签模块完整实现

### 数据模型
新增 `Tag`、`CreateTagRequest`、`UpdateTagRequest`、`DeleteTagRequest`、`BindTagRequest`，严格对应 `./docs/api.md` 的请求参数。

### API 接口
在 `ApiService` 中扩展：
- `POST /api/tag/create`
- `POST /api/tag/update`
- `POST /api/tag/delete`
- `GET /api/tag/list?note_id={}`
- `POST /api/tag/bind`

### Repository
`TagRepository` 封装全部标签接口，统一返回 `Result<T>`。

### ViewModel
`ToolPanelViewModel` 维护：
- `tags`：用户全部标签
- `boundTagIds`：当前笔记已绑定的标签 ID 集合
- `files`：文件列表（与标签同页共用）

**绑定机制：**
- 先调用 `/api/tag/list?note_id=xxx` 获取当前笔记的已绑定标签，存入 `boundTagIds`。
- 右抽屉用 `ChipGroup` 渲染全部标签，每个 `Chip` 的 `isChecked` 由 `boundTagIds` 决定。
- 用户点击 Chip 时，本地修改 Set 后调用 `bindTags(noteId, newList)` 提交服务器，成功后刷新 `boundTagIds`。
- 长按 Chip 弹出菜单：重命名 / 删除。
- 顶部有 `+ 新建标签` 按钮。

### 与笔记编辑页的通信
`MainActivity` 新增 `currentNoteId` 及 `setCurrentNoteId/getCurrentNoteId`：
- `NoteEditorFragment` 在 `onViewCreated` 和加载成功后调用 `setCurrentNoteId`。
- `ToolPanelFragment` 在 `onResume` 中读取 `currentNoteId`，调用 `viewModel.setNoteId(...)`。
- `NavController` 切换页面时（非编辑器）自动将 `currentNoteId` 重置为 `0`，避免旧绑定状态残留。

---

## 2. 搜索页面

### 实时搜索
`SearchViewModel` 中实现 `debounce`：

```kotlin
searchJob = viewModelScope.launch {
    delay(400) // 防抖 400ms
    _isLoading.value = true
    val result = searchRepository.search(keyword)
    _searchResults.value = result.data ?: emptyList()
    _isLoading.value = false
}
```

`SearchFragment` 的 `EditText` 通过 `TextWatcher` 监听输入，每次变化直接调用 `viewModel.search(...)`。

### 页面结构
- `EditText` 搜索框
- `ProgressBar` 加载指示器
- `RecyclerView` 结果列表
- `TextView` 空状态提示

### 交互
- 点击搜索结果项 → `findNavController().navigate(R.id.noteEditorFragment, bundleOf("noteId" to item.id))`

---

## 3. 高亮实现

API 返回字段 `highlight_title` 和 `highlight_content` 包含 `<mark>...</mark>` HTML 标签。

在 `SearchResultAdapter` 中使用 Android 原生 `Html.fromHtml()` 渲染：

```kotlin
val titleHtml = item.highlight_title ?: item.title
binding.tvTitle.text = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
    Html.fromHtml(titleHtml, Html.FROM_HTML_MODE_LEGACY)
} else {
    @Suppress("DEPRECATION")
    Html.fromHtml(titleHtml)
}
```

**说明：** 搜索结果仅需渲染简单的 `<mark>` 高亮片段，不需要完整 Markdown 解析，因此直接使用 `Html.fromHtml()` 即可满足需求。

---

## 文件列表（右抽屉）

- 调用 `GET /api/file/list` 获取当前用户所有上传文件。
- 右抽屉下半部分动态渲染文件名 + 状态（完成/处理中/失败），并配以绿/橙/红三色区分。
- 整个右抽屉被 `SwipeRefreshLayout` 包裹，支持下拉刷新标签+文件。

---

**文档位置：** `./docs/260414_right_drawer.md`
````

---

# 🚀 第5步：文件上传 + OCR

```text title:"ASK"
请在【已有系统基础上】实现文件上传与 OCR 功能。

后端 API 文档位于：./docs/api.md（必须严格遵守）

--------------------------------
# 一、文件上传

位置：
右侧抽屉 → 文件列表顶部添加按钮

## 实现要求

选择文件：
- ActivityResultContracts.GetContent

上传：
- Multipart
- /api/file/upload

## 文件列表增强

API：./docs/api.md Line:755~982
- /api/file/list
- /api/file/delete
- /api/file/status
- /api/file/info

功能：
- 删除文件
- 显示状态, 筛选状态
- 复制链接（ClipboardManager）

--------------------------------

# 二、OCR 功能
API: ./docs/api.md Line:983~1115
## 流程
1. 选择图片（image/*）
2. 上传：
   /api/ocr/recognize
3. 轮询：
   /api/ocr/status
4. 完成：
   - 提示用户
   - 自动生成笔记

## 轮询要求
- 每 2~3 秒一次
- 成功后停止

--------------------------------

# 三、代码要求

概括输出实现思路：

- 上传实现
- OCR 轮询逻辑
- UI 更新逻辑

最后将你的操作在./docs中形成以260414_为前缀的文档
```

## ANS5
````markdown title:"ANS"
文件上传与 OCR 功能已全部实现，编译通过（`BUILD SUCCESSFUL`）。完整文档已生成在 `./docs/260414_file_upload_ocr.md`。

以下按你要求的 3 点概括实现思路：

---

## 1. 上传实现

### Multipart 接口
Retrofit 使用 `@Multipart` + `@Part` 严格对接后端 API：

```kotlin
@Multipart
@POST("/api/file/upload")
suspend fun uploadFile(
    @Part file: MultipartBody.Part,
    @Part("note_id") noteId: RequestBody? = null
): ApiResponse<FileUploadData>
```

### Uri → MultipartBody.Part
在 `ToolPanelFragment` / `MainActivity` 中统一封装 `uriToMultipartPart(uri)`：
1. `ContentResolver.openInputStream(uri)` 读取文件。
2. 拷贝到 `cacheDir` 临时文件。
3. `tempFile.asRequestBody(mimeType.toMediaTypeOrNull())` 生成 `RequestBody`。
4. `MultipartBody.Part.createFormData("file", fileName, requestBody)` 得到 Part。

### 右抽屉 UI 增强
- **上传按钮**：`registerForActivityResult(ActivityResultContracts.GetContent())` 选择任意文件，转成 Part 后调用 `FileRepository.uploadFile()`。
- **状态筛选**：Spinner（全部/完成/处理中/失败），切换后调用 `FileRepository.getFileList(status)` 重新加载。
- **文件长按菜单**：
  - **复制链接**：通过 `ClipboardManager` 将 `file.url` 写入剪贴板。
  - **删除文件**：确认后调用 `FileRepository.deleteFile(fileId)`，成功后自动刷新列表。

---

## 2. OCR 轮询逻辑

### 流程
底部导航点击 **OCR** → 启动 `GetContent("image/*")` 选择图片 → `Uri` 转 `MultipartBody.Part` → `POST /api/ocr/recognize` → 获得 `file_id` → 进入协程轮询：

```kotlin
repeat(60) { // 最多约 2.5 分钟
    delay(2500)
    when (val statusResult = repo.getStatus(fileId)) {
        "done"   -> {
            val noteId = statusResult.data.note_id
            Toast "OCR 完成，已生成笔记"
            navigate(R.id.noteEditorFragment, bundleOf("noteId" to noteId))
            return
        }
        "failed" -> {
            Toast "OCR 处理失败"
            return
        }
        else -> { /* 继续轮询 */ }
    }
}
Toast "OCR 处理超时"
```

### 实现位置
`MainActivity.kt` 中注册了 `ocrImagePicker`，底部导航 `R.id.menu_ocr` 触发 `ocrImagePicker.launch("image/*")`，回调进入 `startOcrProcess(uri)`，全部在 `lifecycleScope` 中执行，不阻塞主线程。

---

## 3. UI 更新逻辑

### 右抽屉（ToolPanelFragment）
- `viewModel.files.observe(viewLifecycleOwner)` 驱动 `renderFiles(files)`。
- `renderFiles` 每次先 `removeAllViews()` 清空旧列表，再遍历新数据用 `ItemFileBinding` 动态生成行。
- 每行设置长按监听，弹出 `AlertDialog`（复制链接 / 删除）。
- 删除或上传成功后，`ViewModel` 内部调用 `loadFiles()`，Observer 自动重新渲染。

### 状态颜色
- 完成（`done`）→ 绿色
- 处理中（`processing`）→ 橙色
- 失败（`failed`）→ 红色

---

**编译结果：** `BUILD SUCCESSFUL`

**文档位置：** `./docs/260414_file_upload_ocr.md`
````
---

# ✅ 最后给你一个关键建议（很重要）

你现在这套已经很接近“工程级 AI 驱动开发”了，再补一个技巧：

👉 每一步执行时，加一句：

```
如果当前实现与之前阶段冲突，以当前阶段为准，并给出修改说明
```

可以避免 Agent “越写越乱”。

---

如果你下一步想再稳一点，我可以帮你做一个：

👉 **“每一步的验收 checklist（像测试用例一样）”**

这样你可以一眼判断 Agent 写的到底行不行。



# Open
- [ ]  数据设置为不为空, 可能导致空指针异常
- [ ] 标签绑定
- [ ] 登陆后不是显示笔记列表而是新建一篇笔记, 导致每次登录都新建了笔记
- [ ] 没有图标
- [ ] 文件树没有图标, 没有根目录笔记
- [ ] 依旧activity问题