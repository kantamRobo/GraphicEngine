#pragma once
#include <d3d11.h>
#include <wrl.h>
class DX11ConstantBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer[2] = { nullptr };
	void* m_constBufferCPU[2] = { nullptr };//CPU������A�N�Z�X�ł��邷��萔�o�b�t�@�̃A�h���X�B
	int m_size = 0;
	int m_allocSize = 0;
	bool m_isValid = false;
	void InitConstantBuffer(ID3D11Device* device, int size, void* srcData = nullptr);
};

