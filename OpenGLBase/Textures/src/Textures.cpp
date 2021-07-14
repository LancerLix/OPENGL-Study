#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb/stb_image.h"

constexpr bool kMixColor = true;

// 如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入.
// 当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了.
const char* vertexShaderSource =
	"#version 450 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"

	"out vec3 ourColor;\n"
	"out vec2 ourTexCoord;\n"

	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"   ourColor = aColor;\n"
	"   ourTexCoord = aTexCoord;\n"
	"}\0";

const char* fragmentShaderSource =
	"#version 450 core\n"
	"out vec4 FragColor;\n"

	"in vec3 ourColor;\n"
	"in vec2 ourTexCoord;\n"

	"uniform bool mixColor;\n"
	"uniform sampler2D ourTexSampler;\n"

	"void main()\n"
	"{\n"
	"	if (mixColor) {"
	"		FragColor = texture(ourTexSampler, ourTexCoord) * vec4(ourColor, 1.0);\n"
	"	} else {"
	"		FragColor = texture(ourTexSampler, ourTexCoord);\n"
	"	}\n"
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

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textures", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile VERTEX_SHADER\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile FRAGMENT_SHADER\n\n" << infoLog << std::endl;
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 生成纹理对象
	unsigned int texture;
	glGenTextures(1, &texture);

	// 绑定纹理对象到当前的OPENGL上下文
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置纹理环绕方式，决定纹理坐标在超出(0,0)~(1,1)的范围时从哪里采样纹理像素
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 设置纹理过滤方式，决定根据纹理坐标值采样纹理像素的方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载图像数据
	int width, height, nrChannels;
	unsigned char* data = stbi_load("resources/img/texture.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		// 设置纹理像素
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// 自动生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
		// 释放图片资源
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	float vertices[] = {
		// parallelogram(two triangle)
		-0.5f, -0.5f, 0.0f, // left bottom
		 1.0f,  0.0f, 0.0f, // lb color(rgb)
		 0.0f,  0.0f,       // lb texture coordinate
		 0.5f, -0.5f, 0.0f, // right bottom
		 0.0f,  1.0f, 0.0f, // rb color(rgb)
		 1.0f,  0.0f,       // rb texture coordinate
		 0.0f,  0.5f, 0.0f, // left top
		 0.0f,  0.0f, 1.0f, // lt color(rgb)
		 0.0f,  1.0f,       // lt texture coordinate
		 1.0f,  0.5f, 0.0f, // right top
		 1.0f,  1.0f, 0.0f, // rt color(rgb)
		 1.0f,  1.0f        // rt texture coordinate
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)/* 0 */, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		int mixColorLocation = glGetUniformLocation(shaderProgram, "mixColor");
		glUniform1i(mixColorLocation, kMixColor);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &VBO);

	glDeleteBuffers(1, &EBO);

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}