#include "DirectX11IndexBuffer.h"

HRESULT DirectX11IndexBuffer::InitIndexbuffer(std::vector<UINT> indices)
{
    // Fill in a buffer description.
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(unsigned int) * 3;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    // Define the resource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = indices.data();
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    // Create the buffer with the device.
   auto hr = g_pd3dDevice->CreateBuffer(&bufferDesc, &InitData, &m_IndexBuffer);
   if (FAILED(hr)) {
       return hr; 
   }
    return hr
}
