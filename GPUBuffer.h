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

	//UAV�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^

	void RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, int bufferNo);

	//SRV�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, int bufferNo);

	Microsoft::WRL::ComPtr<ID3D12Resource> Get()
	{
		return m_buffer;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer = nullptr;
	D3D12_RESOURCE_DESC m_desc;
};

