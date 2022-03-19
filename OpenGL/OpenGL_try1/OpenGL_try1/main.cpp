#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

//顶点数组定义
float vertices[] = {
    -0.5f,-0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f
};

//回调函数，当用户调整窗口时进行调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//检查按键是否按下
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    //初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //调整窗口的维度
    glViewport(0, 0, 800, 600);//第1,2个参数是描述左下角坐标，后面两个参数描述像素

    //注册函数，窗口调整大小时调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //VBO,顶点缓冲对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //绑定缓冲函数
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFER为VBO的缓冲类型
    //将定义的数据复制到当前绑定缓冲的函数
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //GL_STATIC_DRAW ：数据不会或几乎不会改变。
    //GL_DYNAMIC_DRAW：数据会被改变很多。
    //GL_STREAM_DRAW ：数据每次绘制时都会改变。

    //编译着色器，须运行时动态编译
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor\n"
        "void main()\n"
        "{"
        "   FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
        "}\0";

    //创建一个顶点着色器对象，用ID引用，所以用unsigned int
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //第一个参数为需要编译的着色器，第二个为传递的源码字符串数，
    //第三是顶点着色器真正的源码
    glCompileShader(vertexShader);

    //检测调用glCompileShader后是否编译成功，并确定错误
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //创建片段编译器对象
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //创建着色器程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //将着色器附着到程序对象上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //检查程序对象是否编译错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM;;COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //激活程序对象
    glUseProgram(shaderProgram);

    //删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    

    while (!glfwWindowShouldClose(window))//检查GLFW是否被要求退出
    {
        //输入事件
        processInput(window);

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用颜色
        glClear(GL_COLOR_BUFFER_BIT);//glClear清空屏幕颜色缓冲，须接受缓冲位GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT


        glfwSwapBuffers(window);//检查触发事件、更新窗口状态，并调用对应的回调函数
        glfwPollEvents();//交换颜色缓冲
    }

    //释放之前分配的资源
    glfwTerminate();

    return 0;
}