#include "RenderingContext.h"
#include "VulkanBLASBuffer.h"

void raytracing::VulkanBLASBuffer::InitBLAS(RenderingContext& rc)
{

	
	raytracing::AcclerationStructureBuffers blasbuffer;

	m_bottomLevelASBuffers[0].pScratch = CreateBuffer(device, asBuildSizesInfo.accelerationStructureSize, usage, flags);

	m_bottomLevelASBuffers[0].pResult = CreateBuffer();
   


}
