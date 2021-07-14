#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr bool kUseEBO = true;

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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloTriangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ����һ��������ɫ������
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// ���ö�����ɫ�������Դ�룬�ַ���Դ������Ϊ1��Դ��ΪvertexShaderSource
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// ���붥����ɫ������Դ��
	glCompileShader(vertexShader);

	// ��ȡ������ɫ���������״������Ϣ
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile VERTEX_SHADER\n" << infoLog << std::endl;
	}

	// ����һ��Ƭ����ɫ������
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// ����Ƭ����ɫ�������Դ�룬�ַ���Դ������Ϊ1��Դ��ΪfragmentShaderSource
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// ����Ƭ����ɫ������Դ��
	glCompileShader(fragmentShader);

	// ��ȡƬ����ɫ���������״������Ϣ
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile FRAGMENT_SHADER\n\n" << infoLog << std::endl;
	}

	// ����һ����ɫ���������
	int shaderProgram = glCreateProgram();
	// �Ѷ�����ɫ�����󸽼ӵ���ɫ���������
	glAttachShader(shaderProgram, vertexShader);
	// ��Ƭ����ɫ�����󸽼ӵ���ɫ���������
	glAttachShader(shaderProgram, fragmentShader);
	// ���������Ѿ����ӵ���ɫ�����󣬰�ÿ����ɫ����������ӵ��¸���ɫ��������
	glLinkProgram(shaderProgram);

	// ��ȡ��ɫ��������������״������Ϣ
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// �ͷŶ�����ɫ������
	glDeleteShader(vertexShader);
	// �ͷ�Ƭ����ɫ������
	glDeleteShader(fragmentShader);

	float vertices[] = {
		// first triangle
		-0.5f, -0.5f, 0.0f, // a left
		-0.3f, -0.3f, 0.0f, // b left
		 0.5f, -0.5f, 0.0f, // a right
		 0.3f, -0.3f, 0.0f, // b right
		 0.0f,  0.5f, 0.0f, // a top
		 0.0f,  0.3f, 0.0f, // b top
		// second triangle
		 0.0f,  0.5f, 0.0f, // a left
		 0.0f,  0.0f, 0.0f, // b left
		 0.5f, -0.5f, 0.0f, // a top
		 0.0f,  0.0f, 0.0f, // b top
		 1.0f,  0.5f, 0.0f, // a right
		 0.0f,  0.0f, 0.0f  // b right
	};

	// ʹ��EBO������Ч��Լ��������, �����ظ�����Ŀ���
	// ���vertices���������������ε��ظ�����
	float ebo_vertices[] = {
		// parallelogram(two triangle)
		-0.5f, -0.5f, 0.0f, // a left bottom
		-0.3f, -0.3f, 0.0f, // b left bottom
		 0.5f, -0.5f, 0.0f, // a right bottom
		 0.3f, -0.3f, 0.0f, // b right bottom
		 0.0f,  0.5f, 0.0f, // a left top
		 0.0f,  0.3f, 0.0f, // b left top
		 1.0f,  0.5f, 0.0f, // a right top
		 0.0f,  0.0f, 0.0f  // b right top
	};

	unsigned int ebo_indices[] = { // ע��������0��ʼ!
		0, 1, 2, // first triangle
		1, 2, 3  // second triangle
	};

	// VBO, Vertex Buffer Object, ���㻺�����, �������е����붥������
	// VAO, Vertex Array Object, �����������, �������еĶ�������ָ��״̬, ����VBO�����붥��������η��䵽��Ӧ�Ķ�������
	// EBO, Element Buffer Object, �����������, ������ƶ��������, �����������˳��
	unsigned int VBO, VAO, EBO;
	// ����VBO��VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// ����EBO
	if (kUseEBO) glGenBuffers(1, &EBO);

	// ��VBO��VAO��OPENGL������
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ��EBO��OPENGL������
	if (kUseEBO) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// ��ʼ����VAO

	// ����VBO
	if (kUseEBO) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(ebo_vertices), ebo_vertices, GL_STATIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}
	// ����EBO
	if (kUseEBO) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_indices), ebo_indices, GL_STATIC_DRAW);

	// ÿ���������Դ�һ��VBO������ڴ��л���������ݣ��������Ǵ��ĸ�VBO�������п����ж��VBO����ȡ����ͨ���ڵ���glVertexAttribPointerʱ�󶨵�GL_ARRAY_BUFFER��VBO������
	// �����ڵ���glVertexAttribPointer֮ǰ�󶨵�����ǰ�����VBO���󣬶�������0���ڻ����ӵ����Ķ�������

	// �ö�������0��3��components��ɣ�ÿ��component��������GL_FLOAT����VBO��ÿ���6*sizeof(float)ȡһ������ĸö�������
	// stride�������Ϊÿ2�������ͬһ������������VBO�еļ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)/* 0 */, (void*)0);
	glEnableVertexAttribArray(0);

	// ���õ�2������������Ϊ���ԣ����ʹ�ܣ��������ε����ն���Ϊ�����������Ե�����ͣ�
	// �ö�������1�붥������0���ƣ���һ������ĸö������Եĵ�һ��componentλ��VBO��ʼƫ��(3*sizeof(float)λ��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// ��ʹ�ܵ�2���������ԣ�������ɫ��ʹ��Ĭ��ֵ(0)���м���
	// glEnableVertexAttribArray(1);

	// ������VAO��VBO�󣬽�VAO��VBO��OPENGL�����Ľ����Ҫʱ��ֱ�Ӱ�ʹ��
	glBindVertexArray(0);
	// ��ʹglVertexAttribPointer���¼VBO��Ϊ�˰�ȫ������ڽ��VAO���ٽ��VBO������EBO(IBO)һ��Ҫ��VAO�����ٽ�󣬲�ȻVAO�ᶪʧEBO״̬
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (kUseEBO) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ����ʹ�õ���ɫ������
		glUseProgram(shaderProgram);

		// �ٴΰ�VAO����ֱ�ӻ�����VAO�������VBO��EBO��״̬
		// һ�������������ᴢ��������Щ���ݣ�
		// 1.glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ���
		// 2.ͨ��glVertexAttribPointer���õĶ�����������
		// 3.ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����(VBO)
		// 4.��Ŀ����GL_ELEMENT_ARRAY_BUFFER��ʱ��VAO�ᴢ��glBindBuffer�ĺ������á���Ҳ��ζ����Ҳ�ᴢ������ã�����ȷ����û���ڽ��VAO֮ǰ����������黺�壬��������û�����EBO�����ˡ�
		glBindVertexArray(VAO);

		// ��VBO�е�0���������ݿ�ʼ��3�����㣬����������ΪͼԪװ�䣬���Ի�����һ��������
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// ��VBO�е�3��������(�Զ���������������Գ���Ϊ��λ)��ʼ��3�����㣬����������ΪͼԪװ�䣬���Ի����ڶ���������
		// glDrawArrays(GL_TRIANGLES, 3, 3);

		if (kUseEBO) {
			// ��EBO�еĵ�0�����ݿ�ʼ����ȡ��ӦVBO�ж������ݵ���������6�����㣬����������ΪͼԪװ��
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else {
			// ��VBO�е�0���������ݿ�ʼ����6�����㣬����������ΪͼԪװ�䣬���Խ�ǰ���������������ƽ���ı���
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// ����˵VBO��EBO����ͬʱΪVAO����ģ�VAO���ջ��������Ӧ��VBO��EBO
		// ������Ⱦѭ����ֻ�÷����󶨽��VAO
		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// �ͷ�VAO
	glDeleteVertexArrays(1, &VAO);
	// �ͷ�VB0
	glDeleteBuffers(1, &VBO);
	// �ͷ�EBO
	if (kUseEBO) glDeleteBuffers(1, &EBO);

	// �ͷ���ɫ���������
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}