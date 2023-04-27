#pragma once
#include "stdafx.h"
class GPUBuffer
{
	~GPUBuffer()
	{
		if (m_buffer != nullptr)
		{
			m_buffer->Release();
		}
	}

	void InitBuffer(D3D12_RESOURCE_DESC desc);

	//UAVとしてディスクリプタヒープに登録

	void RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, int bufferNo);

	//SRVとしてディスクリプタヒープに登録
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, int bufferNo);

	Microsoft::WRL::ComPtr<ID3D12Resource> Get()
	{
		return m_buffer;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer = nullptr;
	D3D12_RESOURCE_DESC m_desc;
};

