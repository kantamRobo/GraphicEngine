#include "DX11ConstantBuffer.h"
#include <stdexcept>

void DX11ConstantBuffer::InitConstantBuffer(ID3D11Device* device,int paramsize, void* srcData = nullptr)
{

	//マップ可能な定数バッファの作成
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = paramsize + paramsize % 16 == 0 ? 0 : 16 - paramsize % 16);//サイズは16の倍数でないといけない
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ID3D11Bufferを定数バッファとして使うよう宣言している
	desc.Usage = D3D11_USAGE_DYNAMIC;				//GPU上では読み込みだけをCPUから書き込みだけをできるように宣言している
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//CPUとGPU間のデータ転送の時に使う構造体
	//ここではID3D11Bufferの初期データを設定するために使っている
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = srcData;
	initData.SysMemPitch = paramsize;

	auto hr = device->CreateBuffer(&desc, &initData, m_constantBuffer->GetAddressOf());
	if (FAILED(hr)) {
		throw std::runtime_error("マップ可能な定数バッファの作成に失敗");
	}


}
