#include <GLFW/glfw3.h>
#include "CoreGraphicsEngine.h"

#include "stdafx.h"
#include <vector>
#include "RenderingContext.h"


CoreGraphicsEngine* g_graphicsEngine = nullptr;	//グラフィックスエンジン
Camera* g_camera2D = nullptr;				//2Dカメラ。
Camera* g_camera3D = nullptr;				//3Dカメラ。
bool CoreGraphicsEngine::InitializeEngine(GLFWwindow* window, unsigned int frameBufferWidth, unsigned int frameBufferHeight)
{
	g_graphicsEngine = this;

	//デバイス作成
	//論理デバイス
	
	
	//物理デバイス

	return false;
}

bool CoreGraphicsEngine::CreateDevice()
{
	const float defaultQueuePriority(1.0f);
	VkDeviceQueueCreateInfo devQueueCI{};
	devQueueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	devQueueCI.queueFamilyIndex = m_graphicsQueueIndex;
	devQueueCI.queueCount = 1;
	devQueueCI.pQueuePriorities = &defaultQueuePriority;


	std::vector<VkExtensionProperties> devExtProps;
	{
		uint32_t count = 0;
		vkEnumerateDeviceExtensionProperties(PDevice, nullptr, &count, nullptr);

		devExtProps.resize(count);
		vkEnumerateDeviceExtensionProperties(PDevice, nullptr, &count, devExtProps.data());

		std::vector<const char*> extensions;

		for (const auto& v : devExtProps)
		{
			extensions.push_back(v.extensionName);
		}

		VkDeviceCreateInfo ci{};

		ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		ci.pQueueCreateInfos = &devQueueCI;
		ci.queueCreateInfoCount = 1;
		ci.enabledExtensionCount = uint32_t(extensions.size());

		auto result = vkCreateDevice(PDevice, &ci, nullptr, &m_Device);
		if (!result)
		{
			return false;
		 }

		//デバイスキューの取得
		vkGetDeviceQueue(m_Device, m\m_graphicsQueueIndex, 0, &devicequeue);
	}
	return false;
}
