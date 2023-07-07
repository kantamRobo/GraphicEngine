#pragma once
class UniformBuffer
{
public:

	//コンストラクタ
	UniformBuffer() {};
	//デストラクタ
	~UniformBuffer();
	//ムーブコンストラクタ
	UniformBuffer(UniformBuffer&& r)
	{
		m_uniformbuffer[0] = r.m_uniformbuffer[0];
		m_uniformbuffer[1] = r.m_uniformbuffer[1];
		m_uniformbuffer[0] = r.m_uniformbuffer[0];
		m_uniformbuffer[1] = r.m_uniformbuffer[1];
		m_size = r.m_size;
		m_allocSize = r.m_allocSize;
		m_isValid = r.m_isValid;

		r.m_uniformbuffer[0] = nullptr;
		r.m_uniformbuffer[1] = nullptr;
		r.m_uniformBufferCPU[0] = nullptr;
		r.m_uniformBufferCPU[1] = nullptr;
	}

	void InitUniformBuffer(int size, void* srcData = nullptr);

	
	bool IsValid()const
	{
		return m_isValid;
	}

	void CopyToVRAM(void* data);

template<class T>
void CopyToVRAM(T& data)
{
	CopyToVRAM(&data);
}

/// <summary>
	/// ディスクリプタヒープにConstantBufferViewを登録。
	/// </summary>
	/// <param name="descriptorHandle"></param>
void RegistUniformBufferView();
void RegistConstantBufferView( int bufferNo);


private:
	VkBuffer m_uniformbuffer[2] = { nullptr };
	void* m_uniformBufferCPU[2] = { nullptr };
	int m_size = 0;
	int m_allocSize = 0;
	bool m_isValid = false;

};

