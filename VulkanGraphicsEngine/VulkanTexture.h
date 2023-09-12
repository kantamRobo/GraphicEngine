#pragma once
#include <vulkan/vulkan.h>
#include <string>
class VulkanTexture:public IVulkanShaderResource
{
public:
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
	void LoadTextureFromStorage(VkDevice device, const std::string& filepath, VkFormat& format);

	int width;
	int height;
	VkImage m_texture = nullptr;
	VkImageCreateInfo m_textureInfo;
	//ステージングバッファを入れる？
	
};

