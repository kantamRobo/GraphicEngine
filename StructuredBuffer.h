#pragma once
#include "stdafx.h"
class StructuredBuffer
{
public:
	~StructuredBuffer();

	//�\�����o�b�t�@��������
	void InitStructuredBuffer(int sizeOfElement, int numElement, void* initData);


	//SRV�ɓo�^


	/// <summary>
	/// SRV�ɓo�^�B
	/// </summary>
	/// <param name="descriptorHandle"></param>
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);
	Microsoft::WRL::ComPtr<ID3D12Resource> GetD3DResources();
	/// <summary>
	/// �\�����o�b�t�@�̓��e���X�V�B
	/// </summary>
	/// <param name="data"></param>
	void Update(void* data);
	/// <summary>
	/// ����������Ă��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffersOnGPU[2] = { nullptr };
	void* m_buffersOnCPU[2] = { nullptr };
	int m_numElement = 0;
	int m_sizeOfElement = 0;
	bool m_isInited = false;

};

