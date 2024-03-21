#pragma once
#include <DirectXMath.h>

struct SceneConstant
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
	float padding[48];
};