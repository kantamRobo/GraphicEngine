#pragma once
#include <d3d12.h>
#include <wrl.h>
class Shader;
class RootSignature
{
public:
	~RootSignature()
	{
		if (m_rootSignature)
		{
			m_rootSignature->Release();
		}
	}

	bool InitRootSignature(D3D12_FILTER samplerFilter,
		D3D12_TEXTURE_ADDRESS_MODE textureAddressModeU,
		D3D12_TEXTURE_ADDRESS_MODE textureAddressModeV,
		D3D12_TEXTURE_ADDRESS_MODE textureAddressModeW,
		UINT maxCbvDescriptor = 8,
		UINT maxSrvDescriptor = 32,
		UINT maxUavDescriptor = 8);

	bool InitRootSignature(D3D12_STATIC_SAMPLER_DESC* samplerDescArray,
		int numSampler,
		UINT maxCbvDescriptor = 8,
		UINT maxSrvDescriptor = 32,
		UINT maxUavDescriptor = 8,
		UINT offsetInDescriptorsFromTableStartCB = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
		UINT offsetInDescriptorsFromTableStartSRV = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
		UINT offsetInDescriptorsFromTableStartUAV = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);
		bool InitRootSignature(Shader& shader);

   		
		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetR() {
			return m_rootSignature;
		}
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
};

