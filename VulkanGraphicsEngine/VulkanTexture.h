#pragma once
#include <vulkan/vulkan.h>
class VulkanTexture:public IVulkanShaderResource
{

	//コンストラクタ
	VulkanTexture(){}
	//デストラクタ
	~VulkanTexture();

	///
	///ファイルからテクスチャをロードするコンストラクタ
	/// 
	/// ロードするテクスチャのファイルパス。
	
	explicit VulkanTexture(const wchar_t* filePath);

	VkImage Get()
	{
		return m_texture;
	}
	//テクスチャの幅を取得
	int GetWidth()const
	{
		m_texture.
	}
	//テクスチャの高さを取得
	int GetHeight()const
	{
		
	}

	///テクスチャのフォーマットを取得
	VkFormat GetFormat()const
	{
		return m_textureInfo.format;
	}

private:

	VkImage m_texture = nullptr;
	VkImageViewCreateInfo m_textureInfo;
};

