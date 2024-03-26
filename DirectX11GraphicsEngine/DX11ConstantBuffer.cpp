#include "DX11ConstantBuffer.h"
#include <stdexcept>
//a
void DX11ConstantBuffer::InitConstantBuffer(DirectX11GraphicEngine* engine,int paramsize, void* srcData)
{

	//�}�b�v�\�Ȓ萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = paramsize + (paramsize % 16 == 0 ? 0 : 16 - paramsize % 16);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ID3D11Buffer��萔�o�b�t�@�Ƃ��Ďg���悤�錾���Ă���
	desc.Usage = D3D11_USAGE_DYNAMIC;				//GPU��ł͓ǂݍ��݂�����CPU���珑�����݂������ł���悤�ɐ錾���Ă���
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//CPU��GPU�Ԃ̃f�[�^�]���̎��Ɏg���\����
	//�����ł�ID3D11Buffer�̏����f�[�^��ݒ肷�邽�߂Ɏg���Ă���
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = srcData;
	initData.SysMemPitch = paramsize;

	auto hr = device->CreateBuffer(&desc, &initData, m_constantBuffer.GetAddressOf());
	if (FAILED(hr)) {
		throw std::runtime_error("�}�b�v�\�Ȓ萔�o�b�t�@�̍쐬�Ɏ��s");
	}


}
