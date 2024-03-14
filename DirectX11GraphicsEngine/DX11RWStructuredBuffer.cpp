#include "DX11RWStructuredBuffer.h"

void DX11RWStructuredBuffer::InitRWStructuredBuffer(ID3D11Device* device,
	ID3D11DeviceContext* devicecontext,
	int sizeOfElement, int numElement,void* srcData)
{


	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
	vbDesc.ByteWidth = sizeof(m_buffersOnGPU);					// 作成するバッファのバイトサイズ
	vbDesc.MiscFlags = 0;							// その他のフラグ
	vbDesc.StructureByteStride = sizeof(ID3D11Buffer);					// 構造化バッファの場合、その構造体のサイズ
	vbDesc.Usage = D3D11_USAGE_DEFAULT;				// 作成するバッファの使用法
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE| D3D11_CPU_ACCESS_READ;


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
//しおり
//devicecontext->Unmap(m_buffersOnGPU->Get(), 0);

void DX11RWStructuredBuffer::InitRWStructuredBuffer(const DirectX11VertexBuffer& vb, bool isUpdateByCPU)
{
	m_sizeOfElement = vb.m_sizeofElement;
	m_numElement = vb.m_numElement;
	if (isUpdateByCPU) {
		//未対応。
		std::abort();
	}
	else {

		for (auto& gpuBuffer : m_buffersOnGPU) {
			gpuBuffer = vb.m_vertexbuffer;
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

	m_sizeOfElement = ib.m_sizeofElement;
	m_numElement = ib.m_numElement;
	if (isUpdateByCPU) {
		//未対応。
		std::abort();
	}
	else {

		for (auto& gpuBuffer : m_buffersOnGPU) {
			gpuBuffer = ib.m_IndexBuffer;
			gpuBuffer->AddRef();
		}
		//CPUからは変更できないのでマップしない。
		for (auto& cpuBuffer : m_buffersOnCPU) {
			cpuBuffer = nullptr;
		}
	}
	m_isInited = true;
}

void DX11RWStructuredBuffer::RegistUnorderAccessView(int bufferNo)
{



}

void DX11RWStructuredBuffer::RegistShaderResourceView(ID3D11Device* device,int bufferNo)
{

	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = static_cast<UINT>(m_numElement);
	
	auto result = device->CreateShaderResourceView(m_buffersOnGPU[bufferNo].Get(), 
		&desc,m_srvbuffer.ReleaseAndGetAddressOf());




}
