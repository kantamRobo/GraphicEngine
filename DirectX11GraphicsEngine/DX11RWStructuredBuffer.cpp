#include "DX11RWStructuredBuffer.h"

void DX11RWStructuredBuffer::InitRWStructuredBuffer(ID3D11Device* device,
	ID3D11DeviceContext* devicecontext,
	int sizeOfElement, int numElement,void* srcData)
{


	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
	vbDesc.ByteWidth = sizeof(m_buffersOnGPU);					// 作成するバッファのバイトサイズ
	vbDesc.MiscFlags = 0;							// その他のフラグ
	vbDesc.StructureByteStride = 0;					// 構造化バッファの場合、その構造体のサイズ
	vbDesc.Usage = D3D11_USAGE_DEFAULT;				// 作成するバッファの使用法
	vbDesc.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA initData = { srcData, sizeOfElement, 0 };	// 書き込むデータ


	for (auto & buffer : m_buffersOnGPU)
	{
		device->CreateBuffer(&vbDesc, &initData, &buffer);
		

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));


		devicecontext->Map(m_buffersOnGPU->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		//  Update the vertex buffer here.
		memcpy(mappedResource.pData, srcData, m_sizeOfElement * m_numElement);
		//  Reenable GPU access to the vertex buffer data.
			
		}
	
	
	
}

devicecontext->Unmap(m_buffersOnGPU->Get(), 0);

void DX11RWStructuredBuffer::InitRWStructuredBuffer(const DirectX11VertexBuffer& vb, bool isUpdateByCPU)
{
	m_sizeOfElement = vb.GetStrideInBytes();
	m_numElement = vb.GetSizeInBytes() / m_sizeOfElement;
	if (isUpdateByCPU) {
		//未対応。
		std::abort();
	}
	else {

		for (auto& gpuBuffer : m_buffersOnGPU) {
			gpuBuffer = vb.GetID3DResourceAddress();
			gpuBuffer->AddRef();
		}
		//CPUからは変更できないのでマップしない。
		for (auto& cpuBuffer : m_buffersOnCPU) {
			cpuBuffer = nullptr;
		}
	}
	m_isInited = true;


}

void DX11RWStructuredBuffer::InitRWStructuredBuffer(const DirectX11IndexBuffer& ib, bool isUpdateByCPU)
{
}

void DX11RWStructuredBuffer::RegistUnorderAccessView(int bufferNo)
{
}

void DX11RWStructuredBuffer::RegistShaderResourceView(int bufferNo)
{
}
