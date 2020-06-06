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

	//创建一个顶点着色器对象
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//设置顶点着色器对象的源码，字符串源码数量为1，源码为vertexShaderSource
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译顶点着色器对象源码
	glCompileShader(vertexShader);

	//获取顶点着色器对象编译状况和信息
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile VERTEX_SHADER\n" << infoLog << std::endl;
	}

	//创建一个片段着色器对象
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//设置片段着色器对象的源码，字符串源码数量为1，源码为fragmentShaderSource
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//编译片段着色器对象源码
	glCompileShader(fragmentShader);

	//获取片段着色器对象编译状况和信息
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile FRAGMENT_SHADER\n\n" << infoLog << std::endl;
	}

	//创建一个着色器程序对象
	int shaderProgram = glCreateProgram();
	//把顶点着色器对象附加到着色器程序对象
	glAttachShader(shaderProgram, vertexShader);
	//把片段着色器对象附加到着色器程序对象
	glAttachShader(shaderProgram, fragmentShader);
	//链接所有已经附加的着色器对象，把每个着色器的输出链接到下个着色器的输入
	glLinkProgram(shaderProgram);

	//获取着色器程序对象的链接状况和信息
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//释放顶点着色器对象
	glDeleteShader(vertexShader);
	//释放片段着色器对象
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
	//分配VBO，VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//绑定VBO，VAO到OPENGL上下文
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//开始配置VBO，VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//该顶点属性0由3个components组成，每个component的类型是GL_FLOAT，在VBO中每间隔6*sizeof(float)取一个顶点的该顶点属性
	//stride可以理解为每2个顶点的同一个顶点属性在VBO中的间隔
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)/* 0 */, (void*)0);
	glEnableVertexAttribArray(0);

	//设置第2个顶点属性作为测试，如果使能，则三角形的最终顶点为两个顶点属性的坐标和，第一个顶点的该顶点属性的第一个component位于VBO起始偏移(3*sizeof(float)位置
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//设置完VAO和VBO后，将VAO和VBO从OPENGL上下文解绑，需要时再直接绑定使用
	glBindVertexArray(0);
	//即使glVertexAttribPointer会记录VBO，为了安全起见，在解绑VAO后再解绑VBO，但是EBO(IBO)一定要在VAO解绑后再解绑，不然VAO会丢失EBO状态
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//设置使用的着色器程序
		glUseProgram(shaderProgram);

		//再次绑定VAO可以直接获得与该VAO相关联的VBO和EBO的状态
		//一个顶点数组对象会储存以下这些内容：
		//1.glEnableVertexAttribArray和glDisableVertexAttribArray的调用
		//2.通过glVertexAttribPointer设置的顶点属性配置
		//3.通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象(VBO)
		//4.当目标是GL_ELEMENT_ARRAY_BUFFER的时候，VAO会储存glBindBuffer的函数调用。这也意味着它也会储存解绑调用，所以确保你没有在解绑VAO之前解绑索引数组缓冲，否则它就没有这个EBO配置了。
		glBindVertexArray(VAO);

		//从VBO中第0个顶点数据开始画3个顶点，按照三角形为图元装配，可以画出第一个三角形
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//从VBO中第3顶点数据(以顶点的整个顶点属性长度为单位)开始画3个顶点，按照三角形为图元装配，可以画出第二个三角形
		//glDrawArrays(GL_TRIANGLES, 3, 3);

		//从VBO中第0个顶点数据开始使用画6个顶点，按照三角形为图元装配，可以将前面两个三角形组成平行四边形
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	//释放VAO
	glDeleteVertexArrays(1, &VAO);
	//释放VB0
	glDeleteBuffers(1, &VBO);
	//释放着色器程序对象
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}