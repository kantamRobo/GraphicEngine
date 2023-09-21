#include "RenderingContext.h"
#include "VulkanBLASBuffer.h"

void raytracing::VulkanBLASBuffer::InitBLAS(RenderingContext& rc)
{
	VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo_{};
	buildGeometryInfo_.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	buildGeometryInfo_.flags = flags_;
	buildGeometryInfo_.geometryCount = static_cast<uint32_t>(geometries_.Geometry().size());
	buildGeometryInfo_.pGeometries = geometries_.Geometry().data();
	buildGeometryInfo_.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
	buildGeometryInfo_.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	buildGeometryInfo_.srcAccelerationStructure = nullptr;
	
	raytracing::AcclerationStructureBuffers blasbuffer;

	m_bottomLevelASBuffers[0].pScratch = CreateBuffer(device, asBuildSizesInfo.accelerationStructureSize, usage, flags);

	m_bottomLevelASBuffers[0].pResult = CreateBuffer();
   
	VkAccelerationStructureCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	createInfo.buffer = m_resultBuffer;
	createInfo.pNext = nullptr;
	createInfo.size =

		//インスタンスを作成

	
}
