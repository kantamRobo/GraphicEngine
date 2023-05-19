#pragma once
#include "stdafx.h"
class StructuredBuffer
{
public:
	~StructuredBuffer();

	//構造化バッファを初期化
	void InitStructuredBuffer(int sizeOfElement, int numElement, void* initData);


	//SRVに登録


	/// <summary>
	/// SRVに登録。
	/// </summary>
	/// <param name="descriptorHandle"></param>
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);
	Microsoft::WRL::ComPtr<ID3D12Resource> GetD3DResources();
	/// <summary>
	/// 構造化バッファの内容を更新。
	/// </summary>
	/// <param name="data"></param>
	void Update(void* data);
	/// <summary>
	/// 初期化されているか判定。
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

