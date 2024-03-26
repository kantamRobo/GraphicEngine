#pragma once

class Material;
#include "stdafx.h"
namespace raytracing {
	
	struct Instance {
		VkAccelerationStructureBuildGeometryInfoKHR GeometryInfo;
		RWStructuredBuffer m_vertexBufferRWSB;			//���_�o�b�t�@�B
		RWStructuredBuffer m_indexBufferRWSB;			//�C���f�b�N�X�o�b�t�@�B
		Material* m_material = nullptr;
	};

}