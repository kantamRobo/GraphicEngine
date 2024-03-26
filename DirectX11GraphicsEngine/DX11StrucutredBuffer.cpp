#include "DX11StrucutredBuffer.h"

void DX11StructuredBuffer::InitStructuredBuffer(ID3D11Device* device,int sizeOfElement, int numElement, void* initDatap)
{


	m_sizeOfElement = sizeOfElement;
	m_numElement = numElement;
	

	int bufferNo = 0;
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(m_buffersOnGPU);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�\�����o�b�t�@�̓V�F�[�_�[���\�[�X
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.Usage = D3D11_USAGE_DEFAULT;				//GPU��ł͓ǂݍ��݂�����CPU���珑�����݂������ł���悤�ɐ錾���Ă���
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.StructureByteStride = sizeof(ID3D11Buffer);
		//RW��������Ă���B;

	
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = initDatap;
	initData.SysMemPitch = sizeof(initDatap);


		for (auto& buffer : m_buffersOnGPU)
		{

			auto hr = device->CreateBuffer(&desc, &initData, buffer.GetAddressOf());
			if (initDatap != nullptr) {
				memcpy(m_buffersOnCPU[bufferNo], initDatap, m_sizeOfElement * m_numElement);
			}

			bufferNo++;
		}
		m_isInited = true;

}



void DX11StructuredBuffer::RegistShaderResourceView(ID3D11Device* device,int bufferNo)
{
	if (!m_isInited)
	{
		return;
	}

	

	//�V�F�[�_���\�[�X�r���[�����K�v������
//�܂��AFormat�͕K��DXGI_FORMAT_UNKNOWN�ɂ��Ȃ��Ƃ����Ȃ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 1;
	// ���̃R�[�h�ł��ݒ�o����
	//srvDesc.Buffer.ElementOffset = 0;
	//srvDesc.Buffer.ElementWidth = 1;
	// ViewDimension��D3D11_SRV_DIMENSION_BUFFEREX���w�肵�Ă�OK
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	//srvDesc.BufferEx.FirstElement = 0;
	//srvDesc.BufferEx.Flags = 0;
	//srvDesc.BufferEx.NumElements = 1;
	for (auto& buffer : m_buffersOnGPU)
	{
	auto  hr = device->CreateShaderResourceView(buffer.Get(), &srvDesc, this->m_SRVbuffer.GetAddressOf());



	bufferNo++;
	}
}