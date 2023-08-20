#pragma once
class RWStructuredBuffer:public IUnorderAccessResrouce{
public:
	~RWStructuredBuffer();

	void InitRWStructuredBuffer(int sizeOfElement, int numElement, void* initData);

	void InitRWStructuredBuffer(const VertexBuffer& vb, bool isUpdateByCPU);

	void RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

	void InitRWStructuredBuffer(const IndexBuffer& ib, bool isUpdateByCPU);

	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

	bool IsInited()const
	{
		return m_isInited;

	}

	/// <summary>
	/// CPUからアクセス可能なリソースを取得する。
	/// </summary>
	/// <returns></returns>
	void* GetResourceOnCPU();


	Microsoft::WRL::ComPtr<ID3D12Resource> GetD3DResource();
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffersOnGPU[2] = { nullptr };

	void* m_buffersOnCPU[2] = { nullptr };
	int m_sizeOfElement = 0;
	int m_numElement = 0;
	bool m_isInited = false;
};