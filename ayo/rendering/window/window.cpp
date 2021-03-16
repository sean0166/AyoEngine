#include "window.h"
#include "../constants/gl_constants.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Ayo
{
	void frameBufferSizeCB(GLFWwindow* window, int width, int height);

	Window::Window(int height, int width, const std::string& title)
		: m_height(height),
		  m_width(width),
		  m_title(title)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLConstants::kGLMajorVer);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLConstants::kGLMinorVer);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

		if (m_pWindow == nullptr)
		{
			std::cout << "Unable To Create GLFWwindow!" << std::endl;
			glfwTerminate();
			return;
		}
		glfwSetFramebufferSizeCallback(m_pWindow, frameBufferSizeCB);

		glfwMakeContextCurrent(m_pWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize Glad!" << std::endl;
			return;
		}
		glEnable(GL_DEPTH_TEST);

	}

	Window::~Window()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	void Window::update() const
	{

		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
	void Window::clearColorBuffer() const
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::processInput() const
	{
		if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_pWindow, true);
	}
	
	bool Window::isCloseRequested() const
	{
		return glfwWindowShouldClose(m_pWindow);
	}
	
	double Window::getTime() const
	{
		return glfwGetTime();
	}

	void frameBufferSizeCB(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}