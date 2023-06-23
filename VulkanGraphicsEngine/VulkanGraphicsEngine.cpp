#include "VulkanGraphicsEngine.h"
#include "stdafx.h"
#include "RenderingContext.h"
#include <GLFW/glfw3.h>
bool VulkanGraphicsEngine::Init(int width,int height)
{

	g_graphicengine = this;
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	m_frameBufferHeight = height;
	m_frameBufferWidth = width;
	auto window = glfwCreateWindow(m_frameBufferWidth, m_frameBufferHeight, "MainWindow", nullptr, nullptr);
	if (!window)
	{
		return false;
	}


	
	
	

	return true;
}
