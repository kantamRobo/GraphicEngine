#pragma once
#include <DirectXMath.h>

struct SceneConstant
{
	
	DirectX::XMMATRIX xmworld;
	DirectX::XMMATRIX xmview;
	DirectX::XMMATRIX xmproj;
	float padding[48];
};