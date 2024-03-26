#pragma once
#include <d3d11.h>
#include <wrl.h>
class DX11GPUBuffer
{
public:
	~DX11GPUBuffer()
	{
		if (m_buffer != nullptr)
		{
			m_buffer->Release();
		}
	}

	void InitBuffer(D3D11_BUFFER_DESC desc);

	//UAV�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^

	void RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, int bufferNo);

	//SRV�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, int bufferNo);

	Microsoft::WRL::ComPtr<ID3D11Resource> Get()
	{
		return m_buffer;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Resource> m_buffer = nullptr;
	D3D11_BUFFER_DESC m_desc;
};

