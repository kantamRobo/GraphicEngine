#pragma once
class FrameBuffer
{

	//デストラクタ
	~FrameBuffer();
	unsigned int getTextureMemoryTypeIndex(unsigned int requestBits, VkMemoryPropertyFlags requestProps, VkPhysicalDeviceMemoryProperties physMemProps);
	bool CreateFrameBuffer(int w, int h, int mipLevel, VkFormat colorformat, VkFormat depthstencilFormat, float clearColor[4], VkPhysicalDeviceMemoryProperties physMemProps);
	//フレームバッファの作成
	//フレームバッファの幅
	//フレームバッファの高さ
	//ミップマップレベル
		/// <param name="mipLevel">ミップマップレベル。0を指定した場合はミップマップがサポートされているGPUでは1*1ピクセルまでのミップマップが作成される。</param>
	//テクスチャ配列のサイズ
	//カラーバッファのフォーマット
	//深度ステンシルバッファのフォーマット
	bool CreateFramebufferTexture(VulkanGraphicsEngine& ge, VkDevice Device, int w,
		int h, int miplevel,
		 float clearColor[4],
		VkPhysicalDeviceMemoryProperties physMemProps);
	bool CreateImageView(VulkanGraphicsEngine& ge, VkDevice device);
	VulkanTexture& GetRendarTargetTexture()
	{
		return m_FramebufferTexture;
	}



	bool CreateDepthStencilTexture(VulkanGraphicsEngine& ge,
		const VkDevice& Device,
		int w,
		int h,
		VkFormat format);


	//デプスステンシルバッファが存在しているか判定
	bool IsExistDepthStencilBuffer()const
	{
		return m_depthStencilTexture;
	}

	int GetWidth()const
	{
		return m_width;
	}

	int GetHeight()const
	{
		return m_height;
	}
	VkFormat GetColorBufferFormat()const
	{
		return m_FramebufferTexture.GetFormat();
	}


	float GetDSVClearValue()const
	{
		return m_dsvClearValue;
	}

private:

	bool CreateDescriptorPool(VulkanGraphicsEngine& ge, VkDevice device);
	bool CreateFramebufferTexture(VulkanGraphicsEngine& ge, VkDevice Device, int w, int h, int miplevel, int arraySize, VkFormat format, float clearColor[4], VkPhysicalDeviceMemoryProperties physMemProps);	//レンダリングーターゲットとなるテクスチャを作成
	//グラフィックエンジン
	//デバイス
	//テクスチャの幅
	//テクスチャの高さ
	//ミップマップレベル
	//テクスチャ配列のサイズ
	//テクスチャのフォーマット
	//trueが返ってきたら成功


	/// <summary>
	/// 深度ステンシルバッファとなるテクスチャを作成。
	/// </summary>
	/// <param name="ge">グラフィックエンジン</param>
	/// <param name="d3dDevice">D3Dデバイス</param>
	/// <param name="w">テクスチャの幅</param>
	/// <param name="h">テクスチャの高さ</param>
	/// <param name="format">テクスチャのフォーマット</param>
	/// <returns>trueが返ってきたら成功</returns>
	bool CreateDepthStencilTexture(
		VulkanGraphicsEngine& ge,
		const VkDevice& d3dDevice,
		int w,
		int h,
		VkFormat format);

	
private:
	VulkanTexture m_FramebufferTexture;
	VkBuffer m_framebuffertextureVulkan = nullptr;
	VkBuffer m_depthStencilTexture = nullptr;
	VkImage m_depthImage = nullptr;
	VkImage m_FrameBufferImage = nullptr;
	VkDescriptorPool m_FrameBuffertextureDP = nullptr;
	VkDescriptorPool m_depthstenciltextureDP = nullptr;
	VkDevice m_device;
	/*
		UINT m_rtvDescriptorSize = 0;						//フレームバッファのディスクリプタのサイズ。
	UINT m_dsvDescriptorSize = 0;						//深度ステンシルバッファのディスクリプタのサイズ。
	*/
	int m_width = 0;
	int m_height = 0;
	float m_framebufferClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	float m_dsvClearValue = 1.0f;
	VkDeviceMemory FrameBufferdevicememory;
};

