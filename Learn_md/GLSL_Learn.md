# GLSL



### 数据类型



#### 向量

| 类型    | 含义                         |
| ------- | ---------------------------- |
| `vecn`  | 包含`n`个float分量的默认向量 |
| `bvecn` | bool                         |
| `ivecn` | int                          |
| `uvecn` | unsigned int                 |
| `dvecn` | double                       |

##### 重组

```cpp
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

```
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);
```

GLSL中的向量支持自由地重新组合，是一种非常灵活的数据类型。



### 输入与输出

GLSL定义了`in`和`out`关键字实现输入和输出。

顶点着色器的特殊之处在于它从顶点坐标直接接收输入，为了定义顶点数据如何管理，我们使用`location`指定输入变量，这样可以在CPU上配置顶点属性。

顶点着色器需要为它的输入提供一个额外`layout`的标识，这样才能连接到顶点数据。

 片段着色器需要一个`vec4`颜色输出变量，因为片段着色器需要生成一个最终输出的颜色，如果没有定义输出颜色，OpenGL将会将物体渲染成黑色（或者白色）

如果打算从一个着色器向另外一个着色器发送数据，我们必须要在发送方着色器中声明一个输出，在接收着色器中定义一个类似的输入。当类型和名字都一样的时候，OpenGL就会吧两个变量链接到一起，它们之间便可以发送数据。

**例**

顶点着色器

```c
#version 330 core
layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0

out vec4 vertexColor; // 为片段着色器指定一个颜色输出

void main()
{
    gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色
}
```

片段着色器

```c
#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

void main()
{
    FragColor = vertexColor;
}
```



### Uniform

Uniform 是一种CPU中应用向GPU中的着色器发送数据的方式，我们可以程序中设置uniform变量。

uniform是全局的，所以每一个着色器程序对象中uniform变量是独一无二的，可以被着色器程序的任意着色器访问；且无论uniform变量值被设置为什么，uniform会一直保存它们的数据，直到被重置或更新。

可以尝试在片段着色器中设置

```c
#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

void main()
{
    FragColor = ourColor;
}
```

注：uniform如果被声明但未被使用过，编译器会静默移除这个变量。

接下来我们需要在uniform中添加数据。首先找到着色器中uniform属性的索引值，之后更新它的值。

```c
//获得运行秒数
float timeValue = glfwGetTime();
//根据时间变换颜色
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//查询uniform变量myColor的位置值
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//在更新uniform之前必须先使用程序，因为是在着色器程序中设置uniform的
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```

上述后缀的含义如下表

| 后缀 | 定义                  |
| ---- | --------------------- |
| `f`  | 需要一个float作为其值 |
| `i`  | int                   |
| `ui` | unsigned int          |
| `3f` | 3个float              |
| `fv` | float向量/数组        |

以此可以类推

然后再在每次渲染迭代中更新这个uniform

```cpp
while (!glfwWindowShouldClose(window))//检查GLFW是否被要求退出
    {
        //输入事件
        processInput(window);

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	//设置清空屏幕所用颜色
        glClear(GL_COLOR_BUFFER_BIT);
    	//glClear清空屏幕颜色缓冲，须接受缓冲位GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //第一个参数是绘制OpenGL图元的类型
        //第二个参数制定了顶点数组起始索引
        //最后一个参数指定我们打算绘制多少个顶点
 
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //最后一个参数是EBO的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候）

        glfwSwapBuffers(window);//检查触发事件、更新窗口状态，并调用对应的回调函数
        glfwPollEvents();//交换颜色缓冲
    }
```



### 拥有更多属性

我们可以在点的数组中不仅设置位置，同时也可以设置颜色。

比如下面的代码将三角形的三个角分别指定为红色、绿色和蓝色

```cpp
float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};
```

有了更多的数据需要发送到顶点着色器，所以需要修改，使其也能接收颜色信息。

```c
#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}
```

然后可以在片段着色器中定义一个相同的outColor作为输入

```c
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
    //向量灵活的体现
}
```

然后我们就可以用glVertexAttribPointer函数更新顶点格式，

```cpp
// 位置属性
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// 颜色属性
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1);
```

根据glVertexAttribPoint函数各个参数对应的意义，分别修改填入。

