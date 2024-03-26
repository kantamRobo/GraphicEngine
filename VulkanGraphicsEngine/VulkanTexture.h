#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "CoreGraphicsEngine.h"
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

	int width;
	int height;
	VkImage m_texture = nullptr;
	VkImageCreateInfo m_textureInfo;
	//ステージングバッファを入れる？
	
	
	void LoadTextureFromStorage(CoreGraphicsEngine& coregraphicsengine, VkDevice device, const std::string& filepath, VkFormat& format, void* initialData);

};

