#include "DX11RWStructuredBuffer.h"

void DX11RWStructuredBuffer::InitRWStructuredBuffer(ID3D11Device* device,
	ID3D11DeviceContext* devicecontext,
	int sizeOfElement, int numElement,void* srcData)
{


	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;	// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
	vbDesc.ByteWidth = sizeof(m_buffersOnGPU);					// �쐬����o�b�t�@�̃o�C�g�T�C�Y
	vbDesc.MiscFlags = 0;							// ���̑��̃t���O
	vbDesc.StructureByteStride = 0;					// �\�����o�b�t�@�̏ꍇ�A���̍\���̂̃T�C�Y
	vbDesc.Usage = D3D11_USAGE_DEFAULT;				// �쐬����o�b�t�@�̎g�p�@
	vbDesc.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA initData = { srcData, sizeOfElement, 0 };	// �������ރf�[�^


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
