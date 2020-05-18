#include <iostream>
#include <glad/glad.h>
#include <GLFW\glfw3.h>

const char* vertexShaderSource =
	"#version 450 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 bPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x + bPos.x, aPos.y + bPos.y, aPos.z + bPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource =
	"#version 450 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	//glfw��������Ϣ
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//��ʼ��glfw��
	glfwInit();
	//���е�hint������https://www.glfw.org/docs/latest/window.html#window_hints��ѯ��
	//֪ͨglfwʹ�õ�OpenGL�汾��linux�Ͽ���ʹ��glxinfo��ѯ��windows��ʹ��OpenGL Extension Viewer����GPU Caps Viewer
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//֪ͨglfwʹ�õ�OpenGLģʽ����������Ϊ����ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//֪ͨglfw�Ƿ���OpenGL��ǰ�����ԣ�����ģʽ��Ĭ�ϲ�������ǰ���ݣ�ʹ�÷�����OpenGL�������׳�������ִֹ��
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//����һ��glfw���ڣ���800����600������HelloWindow
	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//���õ�ǰ�̵߳�OpenGL������Ϊglfw���ڵ�OpenGL������
	//(��glfw���ڶ�Ӧ��Surface�󶨵�OpenGL�����ģ����Ұ�OpenGL�����İ󶨵���ǰ�߳�,��Ⱦʱͨ����ǰ�߳��õ��̶߳�Ӧ��OpenGL�����������Ⱦ)
	glfwMakeContextCurrent(window);

	//�ڵ���OPENGL����ǰ��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//������Ⱦ�ӿڵĴ�С�����½�����(0,0)����800����600������ֵ���ջ�����2D����ת��(OPENGL����->��Ļ����)
	glViewport(0, 0, 800, 600);

	//�豸���ڴ�С�仯�ص���ͬ���޸��ӿڴ�С
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//����һ��������ɫ������
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//���ö�����ɫ�������Դ�룬�ַ���Դ������Ϊ1��Դ��ΪvertexShaderSource
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//���붥����ɫ������Դ��
	glCompileShader(vertexShader);

	//��ȡ������ɫ���������״������Ϣ
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile VERTEX_SHADER\n" << infoLog << std::endl;
	}

	//����һ��Ƭ����ɫ������
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//����Ƭ����ɫ�������Դ�룬�ַ���Դ������Ϊ1��Դ��ΪfragmentShaderSource
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//����Ƭ����ɫ������Դ��
	glCompileShader(fragmentShader);

	//��ȡƬ����ɫ���������״������Ϣ
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile FRAGMENT_SHADER\n\n" << infoLog << std::endl;
	}

	//����һ����ɫ���������
	int shaderProgram = glCreateProgram();
	//�Ѷ�����ɫ�����󸽼ӵ���ɫ���������
	glAttachShader(shaderProgram, vertexShader);
	//��Ƭ����ɫ�����󸽼ӵ���ɫ���������
	glAttachShader(shaderProgram, fragmentShader);
	//���������Ѿ����ӵ���ɫ�����󣬰�ÿ����ɫ����������ӵ��¸���ɫ��������
	glLinkProgram(shaderProgram);

	//��ȡ��ɫ��������������״������Ϣ
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//�ͷŶ�����ɫ������
	glDeleteShader(vertexShader);
	//�ͷ�Ƭ����ɫ������
	glDeleteShader(fragmentShader);

	float vertices[] = {
		//first triangle
		-0.5f, -0.5f, 0.0f, // a left
		-0.3f, -0.3f, 0.0f, // b left
		 0.5f, -0.5f, 0.0f, // a right
		 0.3f, -0.3f, 0.0f, // b right
		 0.0f,  0.5f, 0.0f, // a top
		 0.0f,  0.3f, 0.0f,  // b top
		//second triangle
		 0.0f,  0.5f, 0.0f, // a left
		 0.0f,  0.0f, 0.0f, // b left
		 0.5f, -0.5f, 0.0f, // a right
		 0.0f,  0.0f, 0.0f, // b right
		 1.0f,  0.5f, 0.0f, // a top
		 0.0f,  0.0f, 0.0f  // b top
	};

	unsigned int VBO, VAO;
	//����VBO��VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//��VBO��VAO��OPENGL������
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//��ʼ����VBO��VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//�ö�������0��3��components��ɣ�ÿ��component��������GL_FLOAT����VBO��ÿ���6*sizeof(float)ȡһ������ĸö�������
	//stride�������Ϊÿ2�������ͬһ������������VBO�еļ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)/* 0 */, (void*)0);
	glEnableVertexAttribArray(0);

	//���õ�2������������Ϊ���ԣ����ʹ�ܣ��������ε����ն���Ϊ�����������Ե�����ͣ���һ������ĸö������Եĵ�һ��componentλ��VBO��ʼƫ��(3*sizeof(float)λ��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//������VAO��VBO�󣬽�VAO��VBO��OPENGL�����Ľ����Ҫʱ��ֱ�Ӱ�ʹ��
	glBindVertexArray(0);
	//��ʹglVertexAttribPointer���¼VBO��Ϊ�˰�ȫ������ڽ��VAO���ٽ��VBO������EBO(IBO)һ��Ҫ��VAO�����ٽ�󣬲�ȻVAO�ᶪʧEBO״̬
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//���������ɫ����ʹ�õ���ɫֵ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//ʹ������ֵ�����ɫ����
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		//�ٴΰ�VAO����ֱ�ӻ�����VAO�������VBO��EBO��״̬
		//һ�������������ᴢ��������Щ���ݣ�
		//1.glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ���
		//2.ͨ��glVertexAttribPointer���õĶ�����������
		//3.ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����(VBO)
		//4.��Ŀ����GL_ELEMENT_ARRAY_BUFFER��ʱ��VAO�ᴢ��glBindBuffer�ĺ������á���Ҳ��ζ����Ҳ�ᴢ������ã�����ȷ����û���ڽ��VAO֮ǰ����������黺�壬��������û�����EBO�����ˡ�
		glBindVertexArray(VAO);

		//��VBO�е�0���������ݿ�ʼ��3�����㣬����������ΪͼԪװ�䣬���Ի�����һ��������
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//��VBO�е�3��������(�Զ���������������Գ���Ϊ��λ)��ʼ��3�����㣬����������ΪͼԪװ�䣬���Ի����ڶ���������
		//glDrawArrays(GL_TRIANGLES, 3, 3);

		//��VBO�е�0���������ݿ�ʼʹ�û�6�����㣬����������ΪͼԪװ�䣬���Խ�ǰ���������������ƽ���ı���
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//��Ⱦ��������ں�˻��壬ǰ�˻���������ʾ������ǰ��˻�����ʾͼ��
		glfwSwapBuffers(window);
		//glfw�����Ϣ֪ͨ���������еĹ�����Ϣ(�������롢����ƶ�������״̬���µ�)
		glfwPollEvents();
	}

	//�ͷ�VAO
	glDeleteVertexArrays(1, &VAO);
	//�ͷ�VB0
	glDeleteBuffers(1, &VBO);

	//�ͷ���ɫ���������
	glDeleteProgram(shaderProgram);

	//�ͷ�glfw����
	glfwDestroyWindow(window);
	//�ͷ�glfw��Դ
	glfwTerminate();

	return 0;
}