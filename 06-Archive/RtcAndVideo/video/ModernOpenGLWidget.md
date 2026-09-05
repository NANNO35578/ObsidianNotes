##### OpenGL 有什么好处?为什么用?

在 Qt 的视频项目中使用 OpenGL 可以带来以下好处：
- *硬件加速和性能优化*：OpenGL 利用显卡的 GPU 来进行图形渲染，相比 CPU 进行软件渲染，可以获得更高的性能和更流畅的视觉效果。在视频项目中，使用 OpenGL 可以实现硬件加速的视频渲染，提供更快速、稳定和高效的视频播放和处理
- *跨平台性*：Qt 自带了对 OpenGL 的良好支持，而且 OpenGL 是跨平台的图形库。使用 OpenGL 进行视频渲染可以保证在不同操作系统和平台上的一致性和可移植性，从而方便开发者在不同设备上展示和播放视频
- *灵活的图形处理能力*：OpenGL 提供了强大的图形处理能力，可以进行各种复杂的图形操作和特效处理。在视频项目中，您可以使用 OpenGL 实现视频的缩放、旋转、反转、混合、滤镜等功能，以及实现各种特殊效果（如模糊、阴影、变形等），为视频增添更多的视觉吸引力
- *与 Qt 其他功能的集成*：Qt 提供了丰富的界面控件和功能模块。通过将 OpenGL 与 Qt 的其他功能（如 GUI 控件、多媒体模块、音频处理等）集成，您可以创建出更丰富、交互性更强的视频应用程序。例如，可以在 OpenGL场景中嵌入 Qt 的控件，实现与视频相互作用的用户界面

此外我个人的感受, 显示视频的时候, 为了适配当前界面的大小, 会对图片进行缩放, 如果使用 Qt 自带的缩放函数, 会对画质有所损失, 而使用 OpenGL, 画质不会损失, 这样说明使用 OpenGL 来做显示是比使用 Qt 函数显示清晰度更高. 

OpenGL 怎么使用
- 首先在项目 pro 文件中添加 OpenGL 模块:`QT += opengl`
- 然后需要添加依赖库: `LIBS += -lopengl32`



----
----

已给你放到右侧 canvas 里：一个 **现代 OpenGL（VAO + VBO + Shader）** 的 `ModernOpenGLWidget`，用 `QOpenGLShaderProgram`、`QOpenGLBuffer`、`QOpenGLVertexArrayObject`、`QOpenGLTexture` 实现，支持：

* 只在需要时上传纹理（`slot_setImage` → 延迟到下一帧 `uploadTexture()`）
* `QImage` 统一为 `RGBA8888` 并做垂直镜像，纹理方向正确
* 按窗口尺寸保持纵横比，自动居中显示
* 纯着色器管线，无 `glBegin/glEnd`、无固定矩阵

### 用法与工程配置

在你的 `.pro` 里加：

```pro
QT += widgets opengl
CONFIG += c++11
```

不需要手动 `-lopengl32`，Qt 的 `opengl` 模块会处理依赖。

示例：

```cpp
auto *w = new ModernOpenGLWidget;
w->slot_setImage(QImage(":/img/test.png"));
w->show();
```

如果需要我再给你一个**最小可运行的 MainWindow + 资源文件**示例，或增加**YUV/NV12 视频帧**直传纹理支持，也可以直接说。

----


```cpp title:"// ModernOpenGLWidget.h"
#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QImage>

class ModernOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  explicit ModernOpenGLWidget(QWidget* parent = nullptr);
  ~ModernOpenGLWidget() override;

  // 设置图像（线程安全建议在 GUI 线程调用）
  void slot_setImage(const QImage& img);

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

private:
  void initShaders();
  void initGeometry();
  void updateQuadVertices(); // 根据窗口和图片大小，更新矩形顶点坐标（保持比例居中）
  void uploadTexture();      // 将 m_image 上传/更新到 GPU 纹理

private:
  // GL 资源
  QOpenGLShaderProgram m_program;
  QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
  QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
  QOpenGLVertexArrayObject m_vao;
  QOpenGLTexture* m_texture{nullptr};

  // 数据
  QImage m_image;         // 原始（或转换后）图像
  QSize m_scaledSize;     // 适配窗口保持比例后的尺寸
  bool m_geometryReady{false};
  bool m_textureDirty{false};
};

```

```cpp title:"// ModernOpenGLWidget.cpp"
#include "ModernOpenGLWidget.h"
#include <QtMath>

// 顶点数据结构: [x, y, u, v]
// 我们用两个三角形构成一个矩形（GL_TRIANGLES），索引: 0-1-2, 2-3-0

ModernOpenGLWidget::ModernOpenGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {
  setMinimumSize(100, 100);
}

ModernOpenGLWidget::~ModernOpenGLWidget() {
  // 确保在当前上下文中安全释放 GL 资源
  makeCurrent();
  if (m_texture) { delete m_texture; m_texture = nullptr; }
  if (m_vbo.isCreated()) m_vbo.destroy();
  if (m_ebo.isCreated()) m_ebo.destroy();
  if (m_vao.isCreated()) m_vao.destroy();
  doneCurrent();
}

void ModernOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  initShaders();
  initGeometry();
}

void ModernOpenGLWidget::initShaders() {
  // 简单的 2D 顶点/片段着色器，无矩阵，直接使用 NDC 坐标
  const char* vsrc = R"GLSL(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aUV;
    out vec2 vUV;
    void main() {
      vUV = aUV;
      gl_Position = vec4(aPos, 0.0, 1.0);
    }
  )GLSL";

  const char* fsrc = R"GLSL(
    #version 330 core
    in vec2 vUV;
    out vec4 FragColor;
    uniform sampler2D uTex;
    void main() {
      FragColor = texture(uTex, vUV);
    }
  )GLSL";

  m_program.create();
  if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc)) {
    qWarning("Vertex shader compile error: %s", m_program.log().toUtf8().constData());
  }
  if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc)) {
    qWarning("Fragment shader compile error: %s", m_program.log().toUtf8().constData());
  }
  if (!m_program.link()) {
    qWarning("Shader program link error: %s", m_program.log().toUtf8().constData());
  }
}

void ModernOpenGLWidget::initGeometry() {
  // 创建 VAO / VBO / EBO
  m_vao.create();
  QOpenGLVertexArrayObject::Binder bindVAO(&m_vao);

  m_vbo.create();
  m_vbo.bind();
  // 先分配空间：4 顶点 * (x,y,u,v) * 4 字节
  m_vbo.allocate(4 * 4 * sizeof(float));

  m_ebo.create();
  m_ebo.bind();
  const GLuint indices[6] = { 0, 1, 2, 2, 3, 0 };
  m_ebo.allocate(indices, sizeof(indices));

  // 顶点属性: location 0 -> vec2 position, location 1 -> vec2 uv
  m_program.bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
  m_program.release();

  m_vbo.release();
  m_ebo.release();

  m_geometryReady = true;
  updateQuadVertices();
}

void ModernOpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT);

  if (!m_geometryReady) return;

  m_program.bind();

  // 绑定纹理（如果有）
  if (m_texture) {
    if (m_textureDirty) {
      uploadTexture();
    }
    glActiveTexture(GL_TEXTURE0);
    m_texture->bind(0);
    m_program.setUniformValue("uTex", 0);
  }

  QOpenGLVertexArrayObject::Binder bindVAO(&m_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  if (m_texture) m_texture->release();
  m_program.release();
}

void ModernOpenGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  updateQuadVertices();
}

void ModernOpenGLWidget::slot_setImage(const QImage& img) {
  if (img.isNull()) return;

  // 统一格式，避免 BGRA/ARGB 差异；OpenGL (0,0) 在左下，QImage 在左上，做垂直镜像
  m_image = img.convertToFormat(QImage::Format_RGBA8888).mirrored(false, true);

  m_textureDirty = true;           // 等下一帧上传
  updateQuadVertices();            // 因为尺寸变化可能影响顶点
  update();                        // 异步重绘
}

void ModernOpenGLWidget::uploadTexture() {
  if (!m_texture) {
    m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
  }

  // 将 QImage 数据上传到 GPU
  if (m_texture->isCreated()) {
    m_texture->destroy();
    m_texture->create();
  }
  m_texture->setData(m_image, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);

  m_textureDirty = false;
}

void ModernOpenGLWidget::updateQuadVertices() {
  if (!m_geometryReady) return;

  const int wndW = qMax(1, width());
  const int wndH = qMax(1, height());

  if (!m_image.isNull()) {
    // 计算保持比例后的显示尺寸
    m_scaledSize = m_image.size().scaled(QSize(wndW, wndH), Qt::KeepAspectRatio);
  } else {
    m_scaledSize = QSize(wndW, wndH);
  }

  // 将像素尺寸映射到 NDC：[-1,1]
  const float sx = (float)m_scaledSize.width()  / (float)wndW;  // 相对窗口宽度比例（0..1）
  const float sy = (float)m_scaledSize.height() / (float)wndH;  // 相对窗口高度比例（0..1）

  const float left   = -sx;
  const float right  =  sx;
  const float bottom = -sy;
  const float top    =  sy;

  // 顶点顺序：0-左下 1-右下 2-右上 3-左上
  const float vertices[16] = {
    left,  bottom,  0.0f, 0.0f,  // 0
    right, bottom,  1.0f, 0.0f,  // 1
    right, top,     1.0f, 1.0f,  // 2
    left,  top,     0.0f, 1.0f   // 3
  };

  QOpenGLVertexArrayObject::Binder bindVAO(&m_vao);
  m_vbo.bind();
  m_vbo.write(0, vertices, sizeof(vertices));
  m_vbo.release();
}

```

----


# Old Version

好的 👍
我先帮你写一个 **改进后的版本**，在保持逻辑简洁的同时，解决几个关键问题：

1. **只在 `slot_setImage` 上传纹理**（而不是每次 `paintGL` 重新上传）。
2. **使用 `update()` 而不是 `repaint()`**。
3. **统一 QImage 格式**，避免颜色错乱。
4. **移除没用的深度缓冲清除**。
5. **坐标系改为正交投影**，不用手动算比例。

> 我先保留了固定管线的写法（`glBegin/glEnd`），这样你能直接运行。
> 如果要完全升级成 **现代 OpenGL（VAO + VBO + Shader）**，代码会长一些，你可以先用这个过渡版本。

---

### 改进版头文件

```cpp
#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QImage>

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  explicit MyOpenGLWidget(QWidget *parent = nullptr);
  void slot_setImage(const QImage &img);

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

private:
  QImage m_image;    // 存储图像数据
  GLuint m_texture;  // 纹理ID
  QSize m_scaledSize; // 缩放后的图像大小
};

#endif // MYOPENGLWIDGET_H
```

---

### 改进版源文件

```cpp
#include "myopenglwidget.h"
#include <QDebug>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_texture(0) {}

void MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // 生成并绑定纹理
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  // 设置纹理参数
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glEnable(GL_TEXTURE_2D);
}

void MyOpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT);

  if (!m_image.isNull()) {
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // 计算居中位置
    float wndW = width();
    float wndH = height();
    float imgW = m_scaledSize.width();
    float imgH = m_scaledSize.height();

    float left   = -imgW / wndW;
    float right  =  imgW / wndW;
    float bottom = -imgH / wndH;
    float top    =  imgH / wndH;

    // 绘制矩形贴图
    glBegin(GL_QUADS);
      glTexCoord2f(0, 1); glVertex2f(left,  bottom);
      glTexCoord2f(1, 1); glVertex2f(right, bottom);
      glTexCoord2f(1, 0); glVertex2f(right, top);
      glTexCoord2f(0, 0); glVertex2f(left,  top);
    glEnd();
  }
}

void MyOpenGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);

  // 设置正交投影 [-1,1] 区间
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
}

void MyOpenGLWidget::slot_setImage(const QImage &img) {
  if (img.isNull()) return;

  // 转换成固定格式，避免颜色问题
  m_image = img.convertToFormat(QImage::Format_RGBA8888);

  // 缩放后的大小（保持比例）
  m_scaledSize = m_image.size().scaled(size(), Qt::KeepAspectRatio);

  // 上传纹理到 GPU
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               m_image.width(), m_image.height(),
               0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());

  // 触发异步重绘
  update();
}
```

---

### ✅ 改进点回顾

* `slot_setImage` **只上传一次纹理**。
* `update()` 异步刷新，不卡 UI。
* `QImage::Format_RGBA8888`，避免 BGRA/ARGB 的兼容问题。
* `resizeGL` 里设置了 **正交投影**，所以顶点坐标计算更直观。
* `paintGL` 里只做绑定和绘制。

---

要不要我再给你写一个 **现代 OpenGL（VAO + VBO + Shader）版本**，你可以对比下差异？

----
----

这个报错信息很关键：

```
undefined reference to `__imp_glBegin'
undefined reference to `__imp_glTexCoord2f'
undefined reference to `__imp_glVertex2f'
...
```

意思是：**链接阶段找不到 OpenGL 的函数实现**。

在 MinGW / Qt 下，情况是这样的：

* `glBegin / glVertex / glOrtho` 这些是 **固定管线 (旧式 OpenGL)** API，不在 Qt 的 `QOpenGLFunctions` 封装里。
* 你直接调用了它们，但链接器没找到对应的库 → 需要显式链接 **OpenGL32.lib**（Windows 下的 `opengl32.dll`）。

---

### ✅ 解决方法

#### 1. 在 `.pro` 文件里加上 OpenGL 库

在你的 Qt 工程 `.pro` 文件里，加入：

```pro
QT += opengl
LIBS += -lopengl32
```

> `-lopengl32` 就是告诉 MinGW 链接 Windows 的 `opengl32.dll`。

然后 **重新 qmake + 编译**。

---

#### 2. 建议（避免固定管线 API）

其实 Qt 已经不推荐直接用 `glBegin/glEnd`。
更标准的写法是通过 `QOpenGLFunctions` 提供的函数，比如：

```cpp
QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
f->glBegin(GL_QUADS);
f->glTexCoord2f(...);
f->glVertex2f(...);
f->glEnd();
```

这样就不用自己 `-lopengl32` 了，因为 Qt 会处理函数指针加载。

---

#### 3. 长远建议

如果你打算在 Qt 项目里继续用 OpenGL，最好完全抛弃 `glBegin/glEnd`，改成 **现代 OpenGL (VBO + Shader)**。
Qt 里有 `QOpenGLShaderProgram` / `QOpenGLBuffer` / `QOpenGLVertexArrayObject`，跨平台也不会有这种链接问题。

---

👉 你现在想要我给你改一个 **只用 `QOpenGLFunctions` 的版本**（不用 `glBegin/glEnd`，还能正常运行），还是直接帮你写一个 **现代 OpenGL（VAO + Shader）版本**？
