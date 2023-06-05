#pragma once

class Material;
#include <atlbase.h>
#include "RWStructuredBuffer.h"
namespace raytracing {
	using ID3D12DescriptorHeapPtr = CComPtr<ID3D12DescriptorHeap>;
	struct Instance {
		D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc;
		RWStructuredBuffer m_vertexBufferRWSB;
		RWStructuredBuffer m_indexBufferRWSB;
		std::shared_ptr<Material> m_material = nullptr;
	};

}
