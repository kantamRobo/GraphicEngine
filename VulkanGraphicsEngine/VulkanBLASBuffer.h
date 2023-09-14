#include <vector>
#include <vulkan/vulkan.h>
#include "Raytracing.h"
#include "VkRaytracingInstance.h"

namespace raytracing {
	class VulkanBLASBuffer
	{
	public:
		//初期化
		void InitBLAS(RenderingContext& rc);

		//BLASBufferのリストを取得

		const std::vector<raytracing::AcclerationStructureBuffers>& Get()
		{

		}

	private:
		std::vector<raytracing::AcclerationStructureBuffers> m_bottomLevelASBuffers;
	};


}