#### OpenGL学习

现基于VS进行开发

## [Hello Triangle](http://www.learnopengl.com/#!Getting-started/Hello-Triangle)

---

### 基本概念

#### 着色器渲染简介

![img](../img/pipeline.png)



1. 一个顶点（Vertex）是一个3D坐标的数据集合，顶点数据用顶点属性表示（Vertex Attribute）表示。
2. 顶点着色器将单独的点作为输入，主要目的是将3D坐标转化为另外一种3D坐标，同时顶点着色器允许对顶点属性进行基本处理。
3. 图元，对于数据渲染进行提示，绘制指令调用将图元传递给OpenGL，例如：GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP。
4. 图元输出会传递给几何着色器。几何着色器将图元形式的一系列定点作为输入，通过产生新顶点构造新的图元或其它形状。
5. 几何着色器的输出被传入光栅化阶段，把图元映射到屏幕上，并且会每一个片段传入片段着色器。在片段着色器运行之前进行裁切，提高效率。
6. 片段着色器主要计算一个像素的最终颜色，包含3D场景的数据。
7. 在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，我们叫做Alpha测试和混合(Blending)阶段。这个阶段检测片段的对应的深度（和模板(Stencil)）值，用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。【本md未提及】



### 模拟操作



#### 顶点输入

每一个顶点都有一个3D坐标，是我们需要指定的，我们一般将他们以标准化设备坐标的形式定义为`float`数组，包含`x,y,z`三轴，其中'z'轴我们可以理解为深度。

**标准化设备坐标**：`x,y,z`三轴将会映射到-1到1的区间之内，该区间之外的所有值将会被抛弃以节省资源。标准化设备坐标接着会变换为屏幕空间坐标，这是使用`glViewport`函数提供的数据，进行视口变换完成的。

我们一般采用下面的表示方式：

```cpp
//顶点数组定义
float vertices[] = {
    -0.5f,-0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f,
    
};

float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};
```



#### VBO

VBO，顶点缓冲对象，用来管理顶点内存，会在GPU内存中储存大量顶点，可以一次性发送大量数据至显卡，因为CPU将数据发送到显卡相对较慢。数据到达显存后，顶点着色器几乎能够立即访问顶点。

```cpp
    //VBO,顶点缓冲对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);//第一个参数表示对象数量
    //绑定缓冲函数
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFER为VBO的缓冲类型
    //将定义的数据复制到当前绑定缓冲的函数
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //GL_STATIC_DRAW ：数据不会或几乎不会改变。
    //GL_DYNAMIC_DRAW：数据会被改变很多。
    //GL_STREAM_DRAW ：数据每次绘制时都会改变。
```



#### 顶点着色器

##### 编写

我们需要用GLSL编写顶点着色器，并编译。

例：

```c
#version 330 core
//版本声明
layout (location = 0) in vec3 aPos;
//GLSL有一个向量类型，包含1到4个float分量，即后缀数字
//vec.x,vec.y,vec.z,vec.w,其中vec.w用于透视除法
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

###### 编译

```cpp
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
```

我们需要动态编译源代码，所以创建一个着色器对象

```cpp
//创建一个顶点着色器对象，用ID引用，所以用unsigned int
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
```

然后将源代码传入着色器

```cpp
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //第一个参数为需要编译的着色器，第二个为传递的源码字符串数，
    //第三是顶点着色器真正的源码
    glCompileShader(vertexShader);
```

我们可以用glCompileShader检测是否编译成功，并且显示编译日志

```cpp
 //检测调用glCompileShader后是否编译成功，并确定错误
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
```

#### 片段着色器

##### Code

```c
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```

因为片段着色器只需要一个输出变量，这是一个四分量向量，表示最终输出颜色。

声明输出变量用`out`关键字。

vec4数组：红色、绿色、蓝色和alpha（透明度）分量，通常缩写为RGBA。每个分量强度设置在0.0到1.0之间。

编译过程同顶点着色器相似，只是使用GL_FRAGMENT_SHADER作为着色器类型：

```cpp
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{"
        "   FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
        "}\0";

	//创建片段编译器对象
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
```



#### 着色器程序

我们需要将刚刚编译的着色器链接（Link）为一个着色器程序对象，以便于在渲染对象时激活这个对象。

```cpp
    //创建着色器程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //将着色器附着到程序对象上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
```

我们可以采取同上面相似的方式进行检查编译，只不过用的函数是`glGetProgramiv`和`glGetProgramInfoLog`：

```cpp
    //检查程序对象是否编译错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM;;COMPILATION_FAILED\n" << infoLog << std::endl;
    }
```

#### 链接顶点属性

![image-20220322111316974](..\img\Vertex1.png)

我们用`glVertexAttribPointer`函数告诉OpenGL如何解析顶点数据：

```cpp
	//告诉OpenGL如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //第一个参数是需要配置的顶点属性，在上面设置了顶点着色器position=0，所以传入0
    //第二个参数vec3是一个vec3，由是三个值组成
    //第三个参数指定数据类型
    //第四个参数定义是否希望数据标准化，GL_TRUE为所有数据会映射到0（有符号数据是-1）到1之间
    //第五个参数步长，定义连续顶点属性组的间隔
    //最后一个参数表示位置数据在起始位置的偏移量，由于位置数据在数组的开头，所以参数为0
    glEnableVertexAttribArray(0);//以顶点属性作为参数，启用顶点属性
```

注：每个顶点属性从一个VBO管理的内存中获得它的数据，而具体是从哪个VBO（程序中可以有多个VBO）获取则是通过在调用`glVertexAttribPointer`时绑定到`GL_ARRAY_BUFFER`的VBO决定的。由于在调用`glVertexAttribPointer`之前绑定的是先前定义的VBO对象，顶点属性`0`现在会链接到它的顶点数据。

#### 顶点数组对象

一个顶点数组对象所储存的内容

- glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
- 通过glVertexAttribPointer设置的顶点属性配置。
- 通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。

![img](D:\Project_File\Rain\img\vertex_array_objects.png)

**创建VAO**

```cpp
	//创建一个VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //第一个参数表示创建的数量，而VAO可以用数组代替

    //绑定VAO
    glBindVertexArray(VAO);
```

#### 绘制

```cpp
 		glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //第一个参数是绘制OpenGL图元的类型
        //第二个参数制定了顶点数组起始索引
        //最后一个参数指定我们打算绘制多少个顶点
```

#### EBO

EBO，索引缓冲对象，可以用来引导点的绘制，并可以重复利用点，达到节省资源的目的。

```cpp
 //EBO,索引缓冲对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

同VBO相似，只不过储存的是索引的

用EBO绘制

```cpp
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//最后一个参数是EBO的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候）
```

