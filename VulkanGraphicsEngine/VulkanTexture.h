#pragma once
class VulkanTexture:public IVulkanShaderResource
{
public:
	//コンストラクタ
	VulkanTexture(){}
	//デストラクタ
	~VulkanTexture();

	//ファイルからテクスチャをロードする
	explicit VulkanTexture(const wchar_t* filePath, unsigned int size);
	void InitFromSTB(const char* memory, unsigned int size, int width, int height);

	void RegistImageView(int bufferNo, VkFormat format);
	
	void InitFromImageResource(VkBuffer texture);

	void RegistImageView(int bufferNo);


	bool IsValid()const
	{
		return m_texture != nullptr;
	}

	VkBuffer Get()
	{
		return m_texture;
	}
private:
	VkBuffer m_texture = nullptr;
	VkBufferCreateInfo m_textureInfo;
};

