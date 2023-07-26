#include "stdafx.h"
#include "RootSignature.h"
#include <array
#include "d3dx12.h"
enum {
    enDescriptorHeap_CB,
    enDescriptorHeap_SRV,
    enDescriptorHeap_UAV,
    enNumDescriptorHeap
};

bool RootSignature::InitRootSignature(D3D12_FILTER samplerFilter, D3D12_TEXTURE_ADDRESS_MODE textureAddressModeU, D3D12_TEXTURE_ADDRESS_MODE textureAddressModeV, D3D12_TEXTURE_ADDRESS_MODE textureAddressModeW, UINT maxCbvDescriptor, UINT maxSrvDescriptor, UINT maxUavDescriptor)
{
    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = samplerFilter;
    sampler.AddressU = textureAddressModeU;
    sampler.AddressV = textureAddressModeV;
    sampler.AddressW = textureAddressModeW;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = D3D12_FLOAT32_MAX;
    sampler.ShaderRegister = 0;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;



    return InitRootSignature(&sampler, 1, maxCbvDescriptor, maxSrvDescriptor, maxUavDescriptor);
}

bool RootSignature::InitRootSignature(D3D12_STATIC_SAMPLER_DESC* samplerDescArray, int numSampler, UINT maxCbvDescriptor, UINT maxSrvDescriptor, UINT maxUavDescriptor, UINT offsetInDescriptorsFromTableStartCB, UINT offsetInDescriptorsFromTableStartSRV, UINT offsetInDescriptorsFromTableStartUAV)
{
    auto d3dDevice = g_graphicsEngine->GetD3DDevice();

    std::array<CD3DX12_DESCRIPTOR_RANGE1, enNumDescriptorHeap> ranges;
    std::array<CD3DX12_ROOT_PARAMETER1, enNumDescriptorHeap> rootParameters;

    ranges[enDescriptorHeap_CB].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, maxCbvDescriptor, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC, offsetInDescriptorsFromTableStartCB);
    rootParameters[enDescriptorHeap_CB].InitAsDescriptorTable(1, &ranges[enDescriptorHeap_CB]);

    ranges[enDescriptorHeap_SRV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, maxSrvDescriptor, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC, offsetInDescriptorsFromTableStartSRV);
    rootParameters[enDescriptorHeap_SRV].InitAsDescriptorTable(1, &ranges[enDescriptorHeap_SRV]);

    ranges[enDescriptorHeap_UAV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, maxUavDescritor, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC, offsetInDescriptorsFromTableStartUAV);
    rootParameters[enDescriptorHeap_UAV].InitAsDescriptorTable(1, &ranges[enDescriptorHeap_UAV]);

    // Allow input layout and deny uneccessary access to certain pipeline stages.
    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(rootParameters.size(), rootParameters.data(), numSampler, samplerDescArray, rootSignatureFlags);
    
    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    auto hr =D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    
    hr = d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
    if (FAILED(hr)) {
        //ルートシグネチャの作成に失敗した。
        return false;
    }
    return false;
}

bool RootSignature::InitRootSignature(Shader& shader)
{
    ComPtr<ID3DBlob> sig = nullptr;

    auto shaderBlob = shader.GetCompiledBlob();
    auto hr = D3DGetBlobPart(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
        D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
    //ルートシグネチャの生成
    auto d3dDevice = g_graphicsEngine->GetD3DDevice();
    hr = d3dDevice->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
        IID_PPV_ARGS(&m_rootSignature));
    return hr ==S_OK;
}
