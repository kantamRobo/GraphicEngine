#include "DirectX11VertexBuffer.h"
#include <wrl.h>

HRESULT DirectX11VertexBuffer::Init(const std::vector<Vertex> v)
{

   


	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
	vbDesc.ByteWidth = sizeof(m_vertexbuffer);					// �쐬����o�b�t�@�̃o�C�g�T�C�Y
	vbDesc.MiscFlags = 0;							// ���̑��̃t���O
	vbDesc.StructureByteStride = 0;					// �\�����o�b�t�@�̏ꍇ�A���̍\���̂̃T�C�Y
	vbDesc.Usage = D3D11_USAGE_DEFAULT;				// �쐬����o�b�t�@�̎g�p�@
	vbDesc.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA initData = { &v[0], sizeof(v), 0 };	// �������ރf�[�^
	// ���_�o�b�t�@�̍쐬
	D3D.m_device->CreateBuffer(&vbDesc, &initData, &m_vertexbuffer);
    return E_NOTIMPL;
}
