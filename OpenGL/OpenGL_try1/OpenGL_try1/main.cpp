#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

//�������鶨��
float vertices[] = {
    -0.5f,-0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f
};

//�ص����������û���������ʱ���е���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//��鰴���Ƿ���
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    //��ʼ��GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //�������ڶ���
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //�������ڵ�ά��
    glViewport(0, 0, 800, 600);//��1,2���������������½����꣬��������������������

    //ע�ắ�������ڵ�����Сʱ����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //VBO,���㻺�����
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //�󶨻��庯��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFERΪVBO�Ļ�������
    //����������ݸ��Ƶ���ǰ�󶨻���ĺ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
    //GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
    //GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣

    //������ɫ����������ʱ��̬����
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

    //����һ��������ɫ��������ID���ã�������unsigned int
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //��һ������Ϊ��Ҫ�������ɫ�����ڶ���Ϊ���ݵ�Դ���ַ�������
    //�����Ƕ�����ɫ��������Դ��
    glCompileShader(vertexShader);

    //������glCompileShader���Ƿ����ɹ�����ȷ������
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //����Ƭ�α���������
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //������ɫ���������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //����ɫ�����ŵ����������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //����������Ƿ�������
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM;;COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //����������
    glUseProgram(shaderProgram);

    //ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    

    while (!glfwWindowShouldClose(window))//���GLFW�Ƿ�Ҫ���˳�
    {
        //�����¼�
        processInput(window);

        //��Ⱦָ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ������ɫ
        glClear(GL_COLOR_BUFFER_BIT);//glClear�����Ļ��ɫ���壬����ܻ���λGL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT


        glfwSwapBuffers(window);//��鴥���¼������´���״̬�������ö�Ӧ�Ļص�����
        glfwPollEvents();//������ɫ����
    }

    //�ͷ�֮ǰ�������Դ
    glfwTerminate();

    return 0;
}