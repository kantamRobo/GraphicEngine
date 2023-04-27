#pragma once

#include "stdafx.h"
class ConstantBuffer
{
//コンストラクタ
	ConstantBuffer(){}

	//デストラクタ
	~ConstantBuffer();
	//ムーブコンストラクタ
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

	//初期化
	//定数バッファのサイズ
	//ソースデータ
	void InitConstBuffer(int size, void* srcData = nullptr);

	//利用可能なバッファかどうか判定

	bool IsValid()const
	{
		return m_isValid;
	}

	//データをVRAMをコピーする
	void CopyToVRAM(void* data);
	template <class T>
	void CopyToVRAM(T& data)
	{
		CopyToVRAM(&data);
	}
	/// <summary>
	/// ディスクリプタヒープにConstantBufferViewを登録。
	/// </summary>
	/// <param name="descriptorHandle"></param>
	void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);
	void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);
	/// <summary>
	/// VRAM上の仮想アドレスを取得する。
	/// </summary>
	/// <returns></returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer[2] = { nullptr };
	void* m_constBufferCPU[2] = { nullptr };//CPU側からアクセスできるする定数バッファのアドレス。
	int m_size = 0;
	int m_allocSize = 0;
	bool m_isValid = false;
};

