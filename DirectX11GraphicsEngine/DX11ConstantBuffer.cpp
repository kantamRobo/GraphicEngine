#include "DX11ConstantBuffer.h"
#include <stdexcept>

void DX11ConstantBuffer::InitConstantBuffer(DirectX11GraphicEngine* engine,int paramsize, void* srcData)
{

	//マップ可能な定数バッファの作成
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = paramsize + (paramsize % 16 == 0 ? 0 : 16 - paramsize % 16);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ID3D11Bufferを定数バッファとして使うよう宣言している
	desc.Usage = D3D11_USAGE_DYNAMIC;				//GPU上では読み込みだけをCPUから書き込みだけをできるように宣言している
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	//CPUとGPU間のデータ転送の時に使う構造体
	//ここではID3D11Bufferの初期データを設定するために使っている
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = srcData;
	initData.SysMemPitch = paramsize;
	
	auto hr = engine->m_device->CreateBuffer(&desc, &initData, m_constantBuffer.GetAddressOf());
	if (FAILED(hr)) {
		throw std::runtime_error("マップ可能な定数バッファの作成に失敗");
	}



	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//CPUとGPU間のデータ転送の時に使う構造体
	//ここではID3D11Bufferの初期データを設定するために使っている

	// リソースをマップしてCPUからアクセス可能なメモリを取得
	 hr = engine->m_deviceContext->Map(
		m_constantBuffer.Get(),  // マップするリソース
		0,                                     // サブリソースのインデックス
		D3D11_MAP_WRITE_DISCARD,               // マップのタイプ
		0,                                     // マップオプション
		&mappedResource);

	if (SUCCEEDED(hr)) {
		// 定数バッファにデータをコピー
		memcpy(mappedResource.pData, m_constantBuffer.Get(), sizeof(m_constantBuffer));
		// リソースのマップを解除
		engine->m_deviceContext->Unmap(m_constantBuffer.Get(), 0);
	}
}
