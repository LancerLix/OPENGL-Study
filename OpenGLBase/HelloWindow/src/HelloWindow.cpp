#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	// glViewport(0, 0, width * 0.5, height * 0.5);
}

void processInput(GLFWwindow* window)
{
	// glfw处理按键消息
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// 初始化glfw库
	glfwInit();
	// 所有的hint可以在https://www.glfw.org/docs/latest/window.html#window_hints查询到
	// 通知glfw使用的OpenGL版本，linux上可以使用glxinfo查询，windows下使用OpenGL Extension Viewer或者GPU Caps Viewer
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	// 通知glfw使用的OpenGL模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 通知glfw是否保留OpenGL向前兼容性，核心模式下默认不保留向前兼容，使用废弃的OpenGL函数会抛出错误中止执行
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 创建一个glfw窗口，宽800，高600，标题HelloWindow
	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWindow", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 设置当前线程的OpenGL上下文为glfw窗口的OpenGL上下文
	// (将glfw窗口对应的EGLSurface绑定到OpenGL上下文，并且把OpenGL上下文绑定到当前线程,渲染时通过当前线程拿到线程对应的OpenGL上下文完成渲染)
	glfwMakeContextCurrent(window);

	// 在调用OPENGL函数前初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置渲染视口的大小，左下角坐标(0,0)，宽800，高600，设置值最终会用于视口变换(OPENGL NDC坐标->屏幕坐标)
	glViewport(0, 0, 800, 600);
	// glViewport(0, 0, 800 * 0.5, 600 * 0.5);

	// 设备窗口大小变化回调，同步修改视口大小
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// 设置清空颜色缓冲使用的颜色值
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 使用设置值清空颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT);

		// 渲染结果保存在后端缓冲，前端缓冲用于显示，交换前后端缓冲显示图像
		glfwSwapBuffers(window);
		// glfw检查消息通知，处理所有的挂起消息(键盘输入、鼠标移动、窗口状态更新等)
		glfwPollEvents();
	}

	// 释放glfw窗口
	glfwDestroyWindow(window);
	// 释放glfw资源
	glfwTerminate();

	return 0;
}