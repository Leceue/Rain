#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <shader/shader_s.h>

//�������鶨��
//float vertices[] = {
//    -0.5f,-0.5f,0.0f,
//    0.5f,-0.5f,0.0f,
//    0.0f,0.5f,0.0f,
//    
//};

//float vertices[] = {
//    0.5f, 0.5f, 0.0f,   // ���Ͻ�
//    0.5f, -0.5f, 0.0f,  // ���½�
//    -0.5f, -0.5f, 0.0f, // ���½�
//    -0.5f, 0.5f, 0.0f   // ���Ͻ�
//};

float vertices[] = {
    // λ��              // ��ɫ
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
};

unsigned int indices[] = {
    0,1,3,
    1,2,3
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

    //����һ��VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //��һ��������ʾ��������������VAO�������������

    //��VAO
    glBindVertexArray(VAO);

    //VBO,���㻺�����
    unsigned int VBO;
    glGenBuffers(1, &VBO);//��һ��������ʾ��������
    //�󶨻��庯��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFERΪVBO�Ļ�������
    //����������ݸ��Ƶ���ǰ�󶨻���ĺ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
    //GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
    //GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣

    //EBO,�����������
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //������ɫ����������ʱ��̬����
    //const char* vertexShaderSource = "#version 330 core\n"
    //    "layout (location = 0) in vec3 aPos;\n"
    //    //"out vec4 vertexColor;\n"
    //    "void main()\n"
    //    "{\n"
    //    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    //    //"   vertexColor = vec4(0.5,0.0,0.0,1.0);"
    //    "}\0";

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        //"out vec4 vertexColor;\n"
        "out vec3 ourColor;"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   ourColor = aColor;"
        //"   vertexColor = vec4(0.5,0.0,0.0,1.0);"
        "}\0";

   /* const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{"
        "   FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
        "}\0";*/
    //const char* fragmentShaderSource = "#version 330 core\n"
    //    "out vec4 FragColor;\n"
    //    "uniform vec4 ourColor;"
    //    "void main()\n"
    //    "{"
    //    "   FragColor = ourColor;\n"
    //    "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main()\n"
        "{"
        "   FragColor = vec4(ourColor,1.0);\n"
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
    //glUseProgram(shaderProgram);

    //ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //����OpenGL��ν�����������
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //��һ����������Ҫ���õĶ������ԣ������������˶�����ɫ��position=0�����Դ���0
    //�ڶ�������vec3��һ��vec3����������ֵ���
    //����������ָ����������
    //���ĸ����������Ƿ�ϣ�����ݱ�׼����GL_TRUEΪ�������ݻ�ӳ�䵽0���з���������-1����1֮��
    //���������������������������������ļ��
    //���һ��������ʾλ����������ʼλ�õ�ƫ����������λ������������Ŀ�ͷ�����Բ���Ϊ0
    //glEnableVertexAttribArray(0);//�Զ���������Ϊ���������ö�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    //�߿�ģʽ
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //����߿�ģʽ
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //std::cout<<glGetString(GL_SHADING_LANGUAGE_VERSION);

    while (!glfwWindowShouldClose(window))//���GLFW�Ƿ�Ҫ���˳�
    {
        //�����¼�
        processInput(window);

        //��Ⱦָ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ������ɫ
        glClear(GL_COLOR_BUFFER_BIT);//glClear�����Ļ��ɫ���壬����ܻ���λGL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT

        glUseProgram(shaderProgram);

        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //��һ�������ǻ���OpenGLͼԪ������
        //�ڶ��������ƶ��˶���������ʼ����
        //���һ������ָ�����Ǵ�����ƶ��ٸ�����

        
        
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //���һ��������EBO��ƫ���������ߴ���һ���������飬�������ǵ��㲻��ʹ��������������ʱ��

        glfwSwapBuffers(window);//��鴥���¼������´���״̬�������ö�Ӧ�Ļص�����
        glfwPollEvents();//������ɫ����
    }

    //�ͷ�֮ǰ�������Դ
    glfwTerminate();

    return 0;
}