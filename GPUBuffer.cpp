#include "GPUBuffer.h"


void GPUBuffer::InitBuffer(D3D12_RESOURCE_DESC desc)
{
	m_desc = desc;

	const D3D12_HEAP_PROPERTIES kDefaultHeapProps =
	{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};


	auto d3dDevice =  g_graphicEngine->GetD3DDevice();

	d3dDevice->CreateCommittedResource(
		&kDefaultHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		nullptr,
		IID_PPV_ARGS(&m_buffer)
	);

}


void GPUBuffer::RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, int bufferNo)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	if (m_desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D) {
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	}
	else {
		//未対応。
		std::abort();
	}

	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateUnorderedAccessView(
		m_buffer,
		nullptr,
		&uavDesc,
		cpuHandle
	);
}

void GPUBuffer::RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	if (m_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D)
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	}

	else {
		//未対応。
		std::abort();
	}
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateShaderResourceView(
		m_buffer,
		nullptr,
		&srvDesc,
		descriptorHandle
	);


}
