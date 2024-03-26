#include "DirectX11VertexBuffer.h"


HRESULT DirectX11VertexBuffer::InitVertexBuffer(ID3D11Device* device,const std::vector<Vertex> v)
{

   
	

	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
	vbDesc.ByteWidth = v.size()*sizeof(m_vertexbuffer);// 作成するバッファのバイトサイズ
	vbDesc.MiscFlags = 0;							// その他のフラグ
	vbDesc.StructureByteStride = 0;					// 構造化バッファの場合、その構造体のサイズ
	vbDesc.Usage = D3D11_USAGE_DEFAULT;				// 作成するバッファの使用法
	vbDesc.CPUAccessFlags = 0;
	this->m_sizeofElement = sizeof(v.data());
	this->m_numElement = v.size();

	D3D11_SUBRESOURCE_DATA initData = { v.data(), sizeof(v), 0};	// 書き込むデータ
	// 頂点バッファの作成
	device->CreateBuffer(&vbDesc, &initData, &m_vertexbuffer);
    return S_OK;
}


//※頂点バッファを動的に扱うときは、また別の初期化法を利用した頂点バッファの
//確保方法がある。