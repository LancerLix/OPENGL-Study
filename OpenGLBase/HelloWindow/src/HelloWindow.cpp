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
	// glfw��������Ϣ
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// ��ʼ��glfw��
	glfwInit();
	// ���е�hint������https://www.glfw.org/docs/latest/window.html#window_hints��ѯ��
	// ֪ͨglfwʹ�õ�OpenGL�汾��linux�Ͽ���ʹ��glxinfo��ѯ��windows��ʹ��OpenGL Extension Viewer����GPU Caps Viewer
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	// ֪ͨglfwʹ�õ�OpenGLģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ֪ͨglfw�Ƿ���OpenGL��ǰ�����ԣ�����ģʽ��Ĭ�ϲ�������ǰ���ݣ�ʹ�÷�����OpenGL�������׳�������ִֹ��
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// ����һ��glfw���ڣ���800����600������HelloWindow
	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWindow", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// ���õ�ǰ�̵߳�OpenGL������Ϊglfw���ڵ�OpenGL������
	// (��glfw���ڶ�Ӧ��EGLSurface�󶨵�OpenGL�����ģ����Ұ�OpenGL�����İ󶨵���ǰ�߳�,��Ⱦʱͨ����ǰ�߳��õ��̶߳�Ӧ��OpenGL�����������Ⱦ)
	glfwMakeContextCurrent(window);

	// �ڵ���OPENGL����ǰ��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ������Ⱦ�ӿڵĴ�С�����½�����(0,0)����800����600������ֵ���ջ������ӿڱ任(OPENGL NDC����->��Ļ����)
	glViewport(0, 0, 800, 600);
	// glViewport(0, 0, 800 * 0.5, 600 * 0.5);

	// �豸���ڴ�С�仯�ص���ͬ���޸��ӿڴ�С
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// ���������ɫ����ʹ�õ���ɫֵ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// ʹ������ֵ�����ɫ����
		glClear(GL_COLOR_BUFFER_BIT);

		// ��Ⱦ��������ں�˻��壬ǰ�˻���������ʾ������ǰ��˻�����ʾͼ��
		glfwSwapBuffers(window);
		// glfw�����Ϣ֪ͨ���������еĹ�����Ϣ(�������롢����ƶ�������״̬���µ�)
		glfwPollEvents();
	}

	// �ͷ�glfw����
	glfwDestroyWindow(window);
	// �ͷ�glfw��Դ
	glfwTerminate();

	return 0;
}