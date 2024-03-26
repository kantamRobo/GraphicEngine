#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "DirectX11GraphicEngine.h"
class DX11ConstantBuffer
{
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	void* m_constBufferCPU[2] = { nullptr };//CPU側からアクセスできるする定数バッファのアドレス。
	int m_size = 0;
	int m_allocSize = 0;
	bool m_isValid = false;
	void InitConstantBuffer(DirectX11GraphicEngine* engine, int paramsize, void* srcData);
};