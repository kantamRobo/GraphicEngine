#include <vector>
#include <vulkan/vulkan.h>
#include "Raytracing.h"
#include "VkRaytracingInstance.h"

namespace raytracing {
	class VulkanBLASBuffer
	{
	public:
		//‰Šú‰»
		void InitBLAS(RenderingContext& rc);

		//BLASBuffer‚ÌƒŠƒXƒg‚ğæ“¾

		const std::vector<raytracing::AcclerationStructureBuffers>& Get()
		{

		}

	private:
		std::vector<raytracing::AcclerationStructureBuffers> m_bottomLevelASBuffers;
	};


}