![](https://cdn.nlark.com/yuque/0/2023/png/39075610/1702188873207-c65d2fd1-aa9f-47be-86d6-7ae4a307e32d.png)

`#include  <GL/glew.h>`这个GL/是路径，我一直以为这也是一个头文件

nullptr 是 C++11 中引入的关键字，用来代表空指针（Null Pointer）。在之前的 C++ 版本中，通常使用   NULL   或者   0   来表示空指针。 

 图像或者视频数据一旦离开CPU，必须通过4个步骤，才会到达显示器： 

 1、从总线进入GPU(Graphics Processing Unit,图形处理器)：将CPU送来的数据总线，再从总线送到GPU里面进行处理
 2、从GPU进入帧缓冲存储器（或称显存)：将GPU芯片处理完的数据送到显存
 3、从显存进入视频控制器：视频控制器有可能是DAC(Digital Analog Converter，随机读写存储数—模转换器)，从显存读取出数据再送到RAM DAC进行数据转换的工作(数字信号转模拟信号)；但是如果是DVI接口类型的显卡，则不需要经过数字信号转模拟信号。而直接输出数字信号
 4、从视频控制器进入显示器：将转换完的模拟信号送到显示屏。 

 屏幕坐标系如下 

![](https://cdn.nlark.com/yuque/0/2023/png/39075610/1702193670518-f6129576-8a0b-4bf6-ac97-ad9558cb5cc8.png)

红色是x轴，绿色是y轴，垂直于他们的是z轴

### 基本类型（一般C++的数据类型都支持，不过要加GL）
1. GLint 是一种数据类型，它是OpenGL（开放图形库）中用来表示整数值的类型之一。OpenGL 是一种用于实现2D和3D图形的跨平台图形库，GLint 是其提供的一种整型数据类型 

###  基本函数 
   1. `glfwInit()`
 是使用GLFW库时初始化其函数库的第一步。GLFW是一个跨平台的开源库，用于创建窗口和OpenGL上下文等。在使用GLFW库的任何其他部分之前，必须先说用  glfwInit()  初始化GLFW库。 

 当应用程序调用  glfwInit()  函数时，GLFW将初始化和配置所需的底层库和硬件设备，并为该应用程序创建必要的OpenGL上下文。如果初始化失败，则必须在继续更多的GLFW库调用之前退出应用程序。如果初始化成功，则可以继续调用其他GLFW库函数，如创建窗口或处理输入事件等。 

      2.`glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号`   




      	`glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//副版本号`   
      	`glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);`   
 glfwWindowHint()   函数是用来设置 GLFW 窗口的一些选项、限制和属性。其中，  GLFW_CONTEXT_VERSION_MAJOR   和   GLFW_CONTEXT_VERSION_MINOR   分别是用来设置所需 OpenGL 渲染上下文版本的主版本号和副版本号，而   GLFW_OPENGL_PROFILE   用于指定使用的 OpenGL profile 。具体解释如下： 

    3.    `glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);`   
 GLFW_RESIZABLE   是一个用于指定窗口是否可以调整大小的选项。通过将其设置为   GL_FALSE  ，将禁止窗口的大小调整功能，用户将无法手动调整窗口的大小。 

 4. 

```cpp
GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGl", nullptr, nullptr);
```

+  WIDTH     和     HEIGHT     是窗口的宽度和高度。 
+  "Learn OpenGl"     是窗口的标题，将显示在窗口的标题栏上。 
+  第四个参数是一个可选的指向 GLFWmonitor 结构的指针，用于指定窗口的要求显示器。通过传递     nullptr  ，可以让 GLFW 自动选择一个显示器。 
+  第五个参数是一个可选的 GLFWwindow 结构的指针，用于指定共享 OpenGL 上下文的窗口。通过传递   nullptr  ，表示当前窗口不会与其他窗口共享 OpenGL 上下文。 

   5.`glfwGetFramebufferSize(window, &screenWidth, &screenHeight);`
 glfwGetFramebufferSize   是 GLFW 库中的一个函数，用于获取帧缓冲（framebuffer）的大小。 帧缓冲可以理解为着色器渲染之后将要显示在窗口上的所有颜色信息，深度信息和模版信息的数据集合，这些数据都保存在内存中，最后经由显示器显示在窗口中。窗口都有一个默认的帧缓冲，来存放最终要显示的所有信息。   

 GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGl", nullptr, nullptr);这个WIDTH和HEIGHT设置的只是窗口的物理大小，而硬件构成一个像素的基础单元是远超过一个像素的，屏幕显示的像素是直接对显存映射的，screenWidth和screenHeight就获得实际显存占用的空间大小。 

   6.`glfwMakeContextCurrent(window);`
 glfwMakeContextCurrent(window)   是 GLFW 库的一个函数调用，用于将当前上下文设置为特定的窗口。它会将指定的   window   参数所代表的窗口设置为当前上下文，以便进行 OpenGL 的渲染操作。 

    7.`glViewport(0, 0, screenWidth, screenHeight); `
 glViewport(0, 0, screenWidth, screenHeight)     是一个用于设置视口（Viewport）的 OpenGL 函数调用。 

 视口是 OpenGL 中用于指定渲染结果在窗口中的显示区域的概念。它定义了 OpenGL 可以进行渲染的目标区域，并将渲染结果投影到该区域内。通过调整视口，我们可以控制渲染结果在窗口中的位置和大小。 

    `glfwPollEvents(); `
 在 GLFW 中，窗口事件包括例如鼠标移动、按键按下、窗口大小变化等一系列用户输入或系统消息事件。当这些事件发生时，GLFW 会将它们存储在一个事件队列中，等待程序处理。 

    `glfwSwapBuffers(window);` 
 glfwSwapBuffers(window)     是 GLFW 库中的一个函数调用，用于交换指定窗口的前后缓冲区。 

 在双缓冲渲染中，通常使用两个缓冲区来绘制图形。一个是前缓冲（front buffer），用于显示当前帧的内容；另一个是后缓冲（back buffer），用于绘制下一帧的内容。 

 而   glfwSwapBuffers(window)   函数的作用就是将绘制好的内容从后缓冲区交换到前缓冲区，使之展示在窗口上。这样，用户就可以看到最新的绘制结果。 

```cpp
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);//着色器也是程序 
  	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL); 
  	glCompileShader(vertexShader); 
```
 这段代码使用 OpenGL API 创建了一个顶点着色器对象，并初始化并编译了这个着色器对象。 

+  第一行     glCreateShader(GL_VERTEX_SHADER)     用于创建一个顶点着色器对象，并返回这个着色器对象的 ID。 
+  第二行     glShaderSource(vertexShader, 1, &vertexShaderCode, NULL)     用于将着色器代码与着色器对象相关联，将之前定义的顶点着色器代码赋值到着色器对象上。第一个参数是着色器对象 ID，第二个参数表示代码字符串数量，这里是 1。第三个参数是一个指向字符串数组的指针，这里指向之前定义的     vertexShaderCode     字符串，第四个参数为 NULL 表示每个字符串的长度是由字符串本身确定的。 
+  第三行     glCompileShader(vertexShader)     用于编译着色器对象，将上一步中指定的着色器代码编译为可供 GPU 执行的代码。这个函数只是编译了着色器代码，并不会将其和其它代码关联起来。 

 执行这些代码后，我们会得到一个已经编译好的顶点着色器对象，但是这个对象仍然需要被链接到一个 OpenGL 程序中，继续与下一步代码。 

```cpp
GLuint shaderProgram = glCreateProgram(); 
glAttachShader(shaderProgram, vertexShader); 
glAttachShader(shaderProgram, fragmentShader); 
glLinkProgram(shaderProgram);//生成的shaderProgram是机器代码，可以直接在GPU上运行 
```
 这段代码将之前已经编译好的顶点着色器对象和片段着色器对象链接到一个 OpenGL 程序中，并进行链接操作，最终创建了一个可供使用的着色器程序对象。 

+  第一行     glCreateProgram()     创建了一个程序对象，并返回该程序对象的 ID。 
+  第二行     glAttachShader(shaderProgram, vertexShader)     将之前编译好的顶点着色器对象链接到程序对象上。第一个参数是程序对象 ID，第二个参数是顶点着色器对象 ID。 
+  第三行     glAttachShader(shaderProgram, fragmentShader)     将之前编译好的片段着色器对象链接到程序对象上。第一个参数是程序对象 ID，第二个参数是片段着色器对象 ID。 
+  第四行     glLinkProgram(shaderProgram)     链接程序对象，将着色器对象合并为一个着色器程序对象。在此过程中，着色器程序会检查并优化连接后的代码，同时，还将属性键关联到着色器程序中，完成了顶点着色器和片段着色器之间的数据传递与处理。 

 执行这些代码之后，会得到一个已经创建好的着色器程序对象，可以后续使用该对象进行渲染和操作。 

```cpp
GLuint VAO, VBO;//一个负责传输数据到显卡位置，另一个负责解释数据 
glGenVertexArrays(1, &VAO);//负责解释数据于着色器联系 
glGenBuffers(1, &VBO);//负责传输数据到显卡里并给出索引位置 
glBindVertexArray(VAO); 
glBindBuffer(GL_ARRAY_BUFFER, VBO);//绑定VAO和VBO，GL_ARRAY_BUFFER用来存储许多数据，VBO自动映射到这个位置 
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//传输数据到显存GL_ARRAY_BUFFER 
```
vertices 确实存放在内存中，但我们需要使用 OpenGL API 将存放在内存的数据拷贝到显存中。在显存中，我们需要一个类似 vertices 对象来表示这块显存，而这样的对象就是 VBO。  

我们对从 vbo 中获取顶点属性数据，进而送给 shader 进行渲染的过程进行梳理，发现如果要从显存中顺利拿到数据，需要给定一系列的参数，包括 size、stride 等等，还要指定从哪个 vbo 里拿。

有的时候，我们要渲染的模型很多，如果在使用模型前都进行一遍参数的设置，那这个过程会非常的繁琐。人们就想，能不能用一个对象来存放这些东西，于是就出现了 VAO（Vertex Array Object）。

   `glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);`
在 OpenGL 中我们使用 `glVertexAttribPointer` 来设置顶点属性数组属性和位置，它将顶点属性数组的数据格式和位置存储在当前绑定的 VAO 中，以便在渲染时使用。

 指定顶点属性的位置（0）是在 `glVertexAttribPointer` 函数的第一个参数中完成的。顶点属性的位置是一个整数值，用来标识不同的顶点属性。在这里，位置（position）为0，表示这个顶点属性是位置属性，即顶点的坐标信息。这个位置值需要与着色器程序中定义的顶点属性的位置保持一致，以确保顶点属性能正确地传递给着色器程序进行处理。 

` glVertexAttribPointer` 其实用到之前绑定的 vao 和 vbo，但函数参数中却没有任何体现，导致这段代码在理解上是“断层”的。主要原因是 OpenGL API 后面隐藏着对 OpengGL Context 属性的修改和访问   

+  index - 指定要修改的位置或索引（即在着色器程序中定义的变量索引），不同的位置对应不同的顶点属性。 
+  size - 指定每个顶点属性的组件数，即每个顶点属性包含多少个数值（如坐标位置是3，纹理坐标是2，颜色是4）。 
+  type - 指定顶点属性数组中每个组件的类型，如 GL_FLOAT，GL_UNSIGNED_INT，GL_BYTE 等。 
+  normalized - 指定是否应将非浮点数型的数据标准化为浮点数型。通常为 GL_FALSE。 
+  stride - 指定连续两个顶点属性之间的字节偏移，即顶点属性数组中相邻两顶点间的偏移距离，为整个结构体的字节大小。例如，我们同时传入了坐标位置和颜色，那么，stride 就应该等于     (3 + 4) * sizeof(float)  。 
+  pointer - 指定第一个顶点属性在缓冲区中的偏移量。通常需要进行一次显式类型转换 (GLvoid*)，如果是从头开始，则为  0  。 

 根据您提供的代码   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);  ，它的作用是将当前绑定的VBO中的第一个位置（位置为0）的顶点属性数组与加载到该位置的顶点数据缓冲区相链接，这里传入了一些参数： 

![](https://cdn.nlark.com/yuque/0/2023/png/39075610/1702258744763-c54a1d4d-cdda-4257-be76-06c2b093f7df.png)

![](https://cdn.nlark.com/yuque/0/2023/png/39075610/1702260892912-d0e8817e-2681-4699-8efe-1b348f6fcb2f.png)



   `glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑定`
   	`glBindVertexArray(0);`
 如果不解绑VAO和VBO，在使用其他对象时可能会出现以下后果： 

1.  修改了顶点属性设置：如果我们在不解绑前一个VAO的情况下绑定新的VAO，并上传新的顶点数据，那么新的数据将与之前的属性设置关联。这可能会导致属性设置错误，例如新的数据无法正确地被解释或渲染。 
2.  冲突的渲染状态：之前绑定的VBO和VAO状态可能会与新的对象冲突。例如，如果我们在绑定新的VAO之前没有解绑先前的VAO，新的数据可能会与先前的数据混合，导致不可预测的渲染结果。 
3.  内存泄漏：如果我们忘记解绑VAO和VBO并继续调用其他OpenGL函数或在程序结束时没有正确释放资源，可能会导致内存泄漏。 

  

    EBO 
```cpp
 GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//传递索引到显存
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);
```

 使用 glBindBuffer 函数将 EBO 绑定到 GL_ELEMENT_ARRAY_BUFFER 目标上。GL_ELEMENT_ARRAY_BUFFER 目标用于表示这是一个用于元素索引数据的缓冲对象。 

 接下来，使用 glBufferData 函数将索引数据拷贝到 EBO 绑定的缓冲对象中。 

```cpp
glBindVertexArray(VAO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

 我们会先创建和绑定VAO，然后配置顶点属性和绑定相应的VBO。当需要将元素索引数据用于绘制时，我们会绑定EBO并上传索引数据。在最后进行渲染时，只需要绑定VAO并调用绘制函数，OpenGL会根据VAO中的配置和EBO中的索引数据来进行渲染。 

 EBO不需要将数据上传到着色器，顶点着色器会根据索引数据自动获取对应的顶点数据，并进行绘制。因为索引数据也在显存中。 

  

    纹理映射 
```cpp
GLuint texture;//生成纹理
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	unsigned char* image = SOIL_load_image(".\\res\\image\\OIP-C.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);//传入图片到GL_TEXTURE_2D
	glGenerateMipmap(GL_TEXTURE_2D);
```

1.  glGenTextures(1, &texture1);  ：该函数生成一个纹理对象，并将其ID存储在变量     texture1     中。 
2.  glBindTexture(GL_TEXTURE_2D, texture1);  ：将纹理对象绑定到     GL_TEXTURE_2D     目标上，确保后续的纹理操作都作用在这个纹理对象上。 
3.  设置纹理的参数： 
    -  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     和     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  ：设置纹理的环绕方式为重复（即纹理坐标超出 0~1 的部分会重复显示）。 
    -  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);     和     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  ：设置纹理的放大和缩小过滤方式为线性滤波。 
4.  载入图片数据： 
    -  unsigned char* image = SOIL_load_image(".\\res\\image\\OIP-C.jpg", &width, &height, 0, SOIL_LOAD_RGBA);  ：使用 SOIL 库加载指定路径的图片，并存储在     image     中，同时获取图片的宽度和高度。 
5.  将图片数据传入纹理对象中： 
    -  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);  ：将图像数据传递给纹理对象，使纹理对象包含了图像的像素数据。 
6.  生成纹理的 mipmap： 
    -  glGenerateMipmap(GL_TEXTURE_2D);  ：为当前绑定的纹理自动生成所有需要的mipmap。 

 通过上述步骤，您成功地创建了一个纹理对象并将图像数据加载到该纹理对象中。这个纹理对象可以用于在渲染时应用到物体表面上。 

  

    glEnable(GL_DEPTH_TEST); 
    	glDepthFunc(GL_LESS); 
    	glEnable(GL_BLEND); 
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
 这段代码开启了深度测试和混合，允许在渲染场景时进行正确的深度排序和透明度混合。 

+  glEnable(GL_DEPTH_TEST)     启用深度测试。深度测试决定哪些片段将会被渲染，哪些将被丢弃。启用深度测试后，OpenGL会比较每个片段的深度值与深度缓冲区中已经存在的深度值，只有当片段的深度值较小时才会保留。 
+  glDepthFunc(GL_LESS)     设置深度测试函数。  GL_LESS     将检查片段的深度值是否小于深度缓冲区中对应像素的深度值。这意味着深度值更小的片段将被认为是“更接近”相机，并最终被渲染到屏幕上。 
+  glEnable(GL_BLEND)     开启混合。混合是将场景中的透明物体与背景混合的过程，以产生透明效果。在混合过程中，片段的颜色可以与已经存在于颜色缓冲区中的颜色进行组合。 
+  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)   设置混合函数。在这个函数中，  GL_SRC_ALPHA   表示源颜色将乘以其自身的 alpha 值作为因子，  GL_ONE_MINUS_SRC_ALPHA   表示目标颜色将乘以 (1 - 源颜色的 alpha 值) 作为因子。这个混合函数可以实现常见的半透明混合效果。 

  

```cpp
    glm::mat4 transform = glm::mat4(1.0f); 
    		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f)); 
    		transform = glm::rotate(transform, glm::radians(20.0f)*static_cast (glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f)); 
    		transform=glm::scale(transform,glm::vec3(0.5f,0.5f,0.5f)); 
    		//transform = glm::translate(transform, glm::vec3(0.0f, 0.4f, 0.0f)); 
    		GLuint transLoc = glGetUniformLocation(ourShader.Program, "transform"); 
    		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(transform)); 
```

 这段代码使用了glm库对一个变换矩阵进行操作，并将其传递给着色器进行渲染。 

 首先，定义并初始化了一个单位矩阵     transform  ，即     glm::mat4(1.0f)  。接着，对该矩阵进行了一系列的变换操作： 

+  transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f))  ：将矩阵沿着 (0, 0, -2) 方向进行平移。 
+  transform = glm::rotate(transform, glm::radians(20.0f) * static_cast (glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f))  ：将矩阵绕着 (1, 1, 1) 方向旋转，旋转角度为当前时间乘以 20 度的弧度值。 
+  transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f))  ：将矩阵在 x、y、z 轴上进行缩放，每个轴都缩放为原来的一半。 

 然后，使用   glGetUniformLocation   函数获取着色器程序中名为   transform   的 uniform 变量的位置，并使用   glUniformMatrix4fv   函数将变换矩阵   transform   传递给该 uniform 变量。 

  

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f); 
    glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); 
 这段代码使用了glm库创建了一个透视矩阵     projection  ，并将其传递给着色器进行渲染。 

 glm::perspective()     函数用于创建一个透视矩阵，接受四个参数：视野角度、宽高比、近平面和远平面。 

 该透视矩阵的计算方式与实际观测到的物体在距离视点较远时出现的视觉变形有关，即离视点较远的物体看上去比较小。这种效果可以在 3D 游戏中用于模拟远近景深的感觉。 

 在这里，使用     glm::perspective()     函数创建透视矩阵     projection  ，其中视野角度为 45 度，宽高比为屏幕宽度与高度的比例，近平面为 0.1，远平面为 100。 

 最后，使用     glGetUniformLocation()     函数获取着色器程序中名为     projection     的 uniform 变量的位置，并使用     glUniformMatrix4fv()     函数将透视矩阵     projection     传递给该 uniform 变量。 

 这段代码的作用是创建并传递一个透视矩阵给着色器，以在渲染过程中产生透视效果。 

  

  

##  延迟着色法 
在渲染中，当有很多物体时，在进行深度测试时，前面的片段会遮挡后面的片段,所以后面的片段如果被遮挡住了那么其不需要在片段着色器中继续渲染了，或着点光源在光源传输过程中会有光的衰减，那么有些物体可能不会被照到，所以这个物体也不需要这个光源的渲染，所以这就需要延迟着色法了。

首先我们设置物体的纹理等各种信息，然后延迟着色法先进行几何处理阶段，生成位置信息法线信息漫反射信息和镜面反射信息，而深度只会保留深度值最浅的信息，然后这些信息会保存到G-buffer中,

```cpp
// G-buffer的创建
unsigned int gBuffer;
glGenFramebuffers(1, &gBuffer);
glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

unsigned int gPosition, gNormal, gAlbedoSpec;
//position-位置信息
glGenTextures(1, &gPosition);
glBindTexture(GL_TEXTURE_2D, gPosition);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
//normal-法线信息
glGenTextures(1, &gNormal);
glBindTexture(GL_TEXTURE_2D, gNormal);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
//albedospec-颜色信息
glGenTextures(1, &gAlbedoSpec);
glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

```

然后使用G缓冲内的纹理数据将其传输到片段着色器并对每一个片段进行渲染。

然而，延迟渲染本身并不能支持非常大量的光源，因为必须要对场景中的每一个光源计算每一个片段的光照分量。而真正让大量光源成为可能的是对延迟渲染管线引用一个非常棒的优化：Light Volumes。

![](https://cdn.nlark.com/yuque/0/2024/png/39075610/1704994881468-5a683124-d0cb-4121-987b-418616ebb315.png)

![](https://cdn.nlark.com/yuque/0/2024/png/39075610/1704994897321-276bd4ec-33df-494f-926c-9d51878273b0.png)

![](https://cdn.nlark.com/yuque/0/2024/png/39075610/1704994908949-cdfaf6d1-8498-4004-938d-8215c87c5a8e.png)

