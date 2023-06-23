#pragma once

#include "resource.h"
#include <vulkan/vulkan_core.h>
#include <vector>
#include <memory>
struct FrameCommandBuffer {
	VkCommandBuffer commandBuffer;
	VkFence fence;
};
class VulkanGraphicsEngine
{
public:
	~VulkanGraphicsEngine();

	//初期化
	//ウインドウハンドル
	//フレームバッファの幅
	//フレームバッファの高さ
	bool Init(int width, int height);
	//1フレームのレンダリングの開始時に呼び出す
	void BeginRender();
	//1フレームのレンダリングの終了時に呼び出す
	void EndRender();

	VkInstance GetInstance()const
	{
		return m_instance;
	}

	VkPhysicalDevice GetPhySicalDevice()const
	{
		return m_physicalDevice;
	}


	VkDevice GetDevice()const
	{
		return m_device;
	}

	unsigned int GetBackBufferIndex()const
	{
		return m_frameIndex;
	}


	//コマンドキューを取得
	VkQueue GetQueue()const
	{
		return m_commandQueue;
	}

	//コマンドバッファを取得
	std::vector<FrameCommandBuffer> GetCommandBuffers()
	{
		return m_commandBuffers;
	}

	//TODO:定数とシェーダーのディスクリプタサイズについて調べておく

	RenderingContext& GetRenderingContext()
	{
		return m_renderingcontext;
	}


	unsigned int GetFrameBufferWidth()const
	{
		return m_frameBufferWidth;
	}

	unsigned int GetFrameBufferHeight()const
	{
		return m_frameBufferHeight;
	}

	void ChangeRenderTargetToFrameBuffer(RenderingContext& rc);

	void CopyToFrameBuffer(RenderingContext& rc, VkBuffer vkbuffer)
	{
//rc.copyresource
		
	}

	//ヌルテクスチャマップを取得
	const NullTextureMaps& GetNullTextureMaps() const
	{
		return m_nullTextureMaps;
	}
private:

	//GPUベンダー定義
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
	
	std::vector<FrameCommandBuffer> m_commandBuffers;

	VkDebugReportCallbackEXT  m_debugReport = VK_NULL_HANDLE;


	// バックバッファのフォーマット指定.
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

	//GPUとの同期で使用する変数
	unsigned int m_frameIndex = 0;
	unsigned int m_frameBufferWidth = 0;				//フレームバッファの幅。
	unsigned int m_frameBufferHeight = 0;
	NullTextureMaps m_nullTextureMaps;			//ヌルテクスチャマップ。

	Camera m_camera2D;							//2Dカメラ。
	Camera m_camera3D;
	
	//3Dカメラ。
};

extern VulkanGraphicsEngine* g_graphicengine;
extern std::shared_ptr < Camera> g_camera2D;					//2Dカメラ。
extern std::shared_ptr < Camera> g_camera3D;					//3Dカメラ。