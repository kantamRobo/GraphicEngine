#pragma once

class Material;
#include "stdafx.h"
namespace raytracing {
	
	struct Instance {
		VkAccelerationStructureBuildGeometryInfoKHR GeometryInfo;
		RWStructuredBuffer m_vertexBufferRWSB;			//頂点バッファ。
		RWStructuredBuffer m_indexBufferRWSB;			//インデックスバッファ。
		Material* m_material = nullptr;
	};

}