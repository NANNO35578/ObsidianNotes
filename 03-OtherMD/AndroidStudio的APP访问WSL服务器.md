Windows主机运行
```sh
adb reverse tcp:8888 tcp:8888
```
- ADB 反向代理（仅模拟器 / 已连接真机）




这个报错**和端口无关**，是 Android 系统的安全限制：

**Android 9.0 以上默认禁止 HTTP 明文请求（只允许 HTTPS）**
你现在访问 `http://127.0.0.1:8888` 是 HTTP，系统直接拦截了！

# 一、10秒解决方法（最简单，直接可用）
## 步骤 1：打开 `AndroidManifest.xml`
位置：`app/src/main/AndroidManifest.xml`

## 步骤 2：在 `<application>` 标签里加一行
```xml
android:usesCleartextTraffic="true"
```

### 最终效果：
```xml
<application
    android:allowBackup="true"
    android:icon="@mipmap/ic_launcher"
    android:label="@string/app_name"
    android:roundIcon="@mipmap/ic_launcher_round"
    android:supportsRtl="true"
    <!-- 加上下面这一行 -->
    android:usesCleartextTraffic="true"

    android:theme="@style/Theme.YourAppName">
```

## 步骤 3：重启 APP（必须重启）
然后重新请求，**立刻就能连上 127.0.0.1:8888**

---

# 二、如果上面不行（更严谨的方案）
在 `res/xml` 新建文件：`network_security_config.xml`
```xml
<?xml version="1.0" encoding="utf-8"?>
<network-security-config>
    <domain-config cleartextTrafficPermitted="true">
        <domain includeSubdomains="true">127.0.0.1</domain>
    </domain-config>
</network-security-config>
```

然后在 `AndroidManifest.xml` 里引用：
```xml
android:networkSecurityConfig="@xml/network_security_config"
```

---

# 三、你现在的完整连接流程
1. WSL 服务监听 `0.0.0.0:8888`
2. 执行：`./adb reverse tcp:8888 tcp:8888`
3. APP 里访问：`http://127.0.0.1:8888`
4. 加上 `usesCleartextTraffic="true"` ✅

**现在就能成功连接！**

---

### 总结
1. 报错原因：**Android 9+ 禁止 HTTP 明文请求**
2. 解决：`AndroidManifest.xml` 加一行 `android:usesCleartextTraffic="true"`
3. 重启 APP 即可正常访问 WSL 服务

需要我帮你检查你的清单文件是否写对吗？