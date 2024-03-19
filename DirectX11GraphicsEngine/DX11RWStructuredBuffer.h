#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "DirectX11IndexBuffer.h"
#include "DirectX11VertexBuffer.h"
class DX11RWStructuredBuffer
{
	~DX11RWStructuredBuffer();

	

	
	

	void InitRWStructuredBuffer(ID3D11Device* device, ID3D11DeviceContext* devicecontext, int sizeOfElement, int numElement, void* InitData);

	void InitRWStructuredBuffer(const DirectX11VertexBuffer& vb, bool isUpdateByCPU);

	void InitRWStructuredBuffer(const DirectX11IndexBuffer& ib, bool isUpdateByCPU);


	void RegistUnorderAccessView( int bufferNo);

	void RegistShaderResourceView(ID3D11Device* device, int bufferNo);



	void RegistShaderResourceView( int bufferNo);

	bool IsInited()const
	{
		return m_isInited;

	}

	/// <summary>
	/// CPUからアクセス可能なリソースを取得する。
	/// </summary>
	/// <returns></returns>
	void* GetResourceOnCPU();

	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView> m_srvbuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetD3DResource();
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffersOnGPU[2] = { nullptr };
	
	void* m_buffersOnCPU[2] = { nullptr };
	int m_sizeOfElement = 0;
	int m_numElement = 0;
	bool m_isInited = false;
};

