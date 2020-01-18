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
	//glfw处理按键消息
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//初始化glfw库
	glfwInit();
	//所有的hint可以在https://www.glfw.org/docs/latest/window.html#window_hints查询到
	//通知glfw使用的OpenGL版本，linux上可以使用glxinfo查询，windows下使用OpenGL Extension Viewer或者GPU Caps Viewer
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//通知glfw使用的OpenGL模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//通知glfw是否保留OpenGL向前兼容性，核心模式下默认不保留向前兼容，使用废弃的OpenGL函数会抛出错误中止执行
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//创建一个glfw窗口，宽800，高600，标题HelloWindow
	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//设置当前线程的OpenGL上下文为glfw窗口的OpenGL上下文
	//(将glfw窗口对应的Surface绑定到OpenGL上下文，并且把OpenGL上下文绑定到当前线程,渲染时通过当前线程拿到线程对应的OpenGL上下文完成渲染)
	glfwMakeContextCurrent(window);

	//在调用OPENGL函数前初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//设置渲染视口的大小，左下角坐标(0,0)，宽800，高600，设置值最终会用于2D坐标转换(OPENGL坐标->屏幕坐标)
	glViewport(0, 0, 800, 600);

	//设备窗口大小变化回调，同步修改视口大小
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
		-0.5f, -0.5f, 0.0f, // a left
		 0.5f, -0.5f, 0.0f, // a right
		 0.0f,  0.5f, 0.0f, // a top
		-0.1f, -0.1f, 0.0f, // b left
		 0.1f, -0.1f, 0.0f, // b right
		 0.0f,  0.1f, 0.0f  // b top
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//顶点属性0由3个components组成，每个component的类型是GL_FLOAT
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)/* 0 */, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//设置清空颜色缓冲使用的颜色值
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//使用设置值清空颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//渲染结果保存在后端缓冲，前端缓冲用于显示，交换前后端缓冲显示图像
		glfwSwapBuffers(window);
		//glfw检查消息通知，处理所有的挂起消息(键盘输入、鼠标移动、窗口状态更新等)
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteProgram(shaderProgram);

	//释放glfw窗口
	glfwDestroyWindow(window);
	//释放glfw资源
	glfwTerminate();

	return 0;
}