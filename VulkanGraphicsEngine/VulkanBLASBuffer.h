#include <vector>
#include <vulkan/vulkan.h>
#include "Raytracing.h"
#include "VkRaytracingInstance.h"

namespace raytracing {
	class VulkanBLASBuffer
	{
	public:
		//������
		void InitBLAS(RenderingContext& rc);

		//BLASBuffer�̃��X�g���擾

		const std::vector<raytracing::AcclerationStructureBuffers>& Get()
		{

		}

	private:
		std::vector<raytracing::AcclerationStructureBuffers> m_bottomLevelASBuffers;
	};


}