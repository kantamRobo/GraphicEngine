#pragma once

#include "stdafx.h"
class ConstantBuffer
{
//�R���X�g���N�^
	ConstantBuffer(){}

	//�f�X�g���N�^
	~ConstantBuffer();
	//���[�u�R���X�g���N�^
	ConstantBuffer(ConstantBuffer&& r)
	{
		m_constantBuffer[0] = r.m_constantBuffer[0];
		m_constantBuffer[1] = r.m_constantBuffer[1];
		m_constBufferCPU[0] = r.m_constBufferCPU[0];
		m_constBufferCPU[1] = r.m_constBufferCPU[1];
		m_size = r.m_size;
		m_allocSize = r.m_allocSize;
		m_isValid = r.m_isValid;

		r.m_constantBuffer[0] = nullptr;
		r.m_constantBuffer[1] = nullptr;
		r.m_constBufferCPU[0] = nullptr;
		r.m_constBufferCPU[1] = nullptr;
	}

	//������
	//�萔�o�b�t�@�̃T�C�Y
	//�\�[�X�f�[�^
	void InitConstBuffer(int size, void* srcData = nullptr);

	//���p�\�ȃo�b�t�@���ǂ�������

	bool IsValid()const
	{
		return m_isValid;
	}

	//�f�[�^��VRAM���R�s�[����
	void CopyToVRAM(void* data);
	template <class T>
	void CopyToVRAM(T& data)
	{
		CopyToVRAM(&data);
	}
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v��ConstantBufferView��o�^�B
	/// </summary>
	/// <param name="descriptorHandle"></param>
	void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);
	void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);
	/// <summary>
	/// VRAM��̉��z�A�h���X���擾����B
	/// </summary>
	/// <returns></returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer[2] = { nullptr };
	void* m_constBufferCPU[2] = { nullptr };//CPU������A�N�Z�X�ł��邷��萔�o�b�t�@�̃A�h���X�B
	int m_size = 0;
	int m_allocSize = 0;
	bool m_isValid = false;
};

