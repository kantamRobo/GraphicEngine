#pragma once

#include "resource.h"
#include <vulkan/vulkan_core.h>
#include <vector>
#include <memory>
class VulkanGraphicsEngine
{


private:

	//GPU�x���_�[��`
	enum GPU_Vender {
		GPU_VenderNvidia,
		GPU_VenderAMD,
		GPU_VenderIntel,
		Num_GPUVender,

	};

	
	VkInstance m_instance = VK_NULL_HANDLE;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device = VK_NULL_HANDLE;
	
	VkPhysicalDeviceMemoryProperties m_memProps;
	VkPhysicalDeviceProperties  m_physicalDeviceProperties;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	VkSurfaceKHR    m_surface = VK_NULL_HANDLE;
	VkExtent2D      m_surfaceExtent;
	VkSwapchainKHR  m_swapchain = VK_NULL_HANDLE;
	std::vector<vk::ImageResource> m_renderTargets;
	struct FrameCommandBuffer {
		VkCommandBuffer commandBuffer;
		VkFence fence;
	};
	std::vector<FrameCommandBuffer> m_commandBuffers;

	VkDebugReportCallbackEXT  m_debugReport = VK_NULL_HANDLE;


	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�w��.
	VkSurfaceFormatKHR BackBufferFormat = {
		VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
	};
	uint32_t m_frameIndex = 0;
	VkSemaphore m_renderCompleted;
	VkSemaphore m_presentCompleted;

	VkQueue m_commandQueue = VK_NULL_HANDLE;
	VkDescriptorPool m_framebufferpool = VK_NULL_HANDLE;
	VkDescriptorPool m_dsvpool = VK_NULL_HANDLE;
	VkPipeline m_pipeline = VK_NULL_HANDLE;

	VkViewport viewport = {};

	VkRect2D vkrect2D = {};

	RenderingContext m_renderingcontext;

	//GPU�Ƃ̓����Ŏg�p����ϐ�
	unsigned int m_frameIndex = 0;
	unsigned int m_frameBufferWidth = 0;				//�t���[���o�b�t�@�̕��B
	unsigned int m_frameBufferHeight = 0;
	NullTextureMaps m_nullTextureMaps;			//�k���e�N�X�`���}�b�v�B

	Camera m_camera2D;							//2D�J�����B
	Camera m_camera3D;							//3D�J�����B
};

extern std::shared_ptr<VulkanGraphicsEngine> g_graphicengine;
extern std::shared_ptr < Camera> g_camera2D;					//2D�J�����B
extern std::shared_ptr < Camera> g_camera3D;					//3D�J�����B