#pragma once
class UniformBuffer
{
	//コンストラクタ
	UniformBuffer(){}

	//デストラクタ
	~UniformBuffer();

	//ムーブコンストラクタ
	UniformBuffer(UniformBuffer&& r)
	{
		m_uniformBuffer[0] = r.m_uniformBuffer[0];
		m_uniformBuffer[1] = r.m_uniformBuffer[1];
		m_uniformBufferCPU[0] = r.m_uniformBufferCPU[0];
		m_uniformBufferCPU[1] = r.m_uniformBufferCPU[1];
		m_allocSize = r.m_allocSize;

		r.m_uniformBuffer[0] = nullptr;
		r.m_uniformBuffer[1] = nullptr;
		r.m_uniformBufferCPU[0] = nullptr;
		r.m_uniformBufferCPU[1] = nullptr;
	}

	void InitUniformBuffer(int size, void* srcData, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags);

	//利用可能なバッファかどうか判定
	bool IsValid()const
	{
		return m_isValid;
	}

	//データをVRAMにコピーする
	void CopyToVRAM(void* data);
	template<class T>
	void CopyToVRAM(T& data)
	{
		CopyToVRAM(&data);
	}

	//TODO:ここはユニフォームバッファの仕様を調べる。
	//家でやる

private:
	VkBuffer m_uniformBuffer[2] = { nullptr };
	void* m_uniformBufferCPU[2] = { nullptr };
	int m_size = 0;
	int m_allocSize = 0;
	bool m_isValid = false;


	
};


