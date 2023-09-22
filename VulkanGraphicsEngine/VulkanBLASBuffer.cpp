#include <cstrin>
#include "RenderingContext.h"
#include "VulkanBLASBuffer.h"

void raytracing::VulkanBLASBuffer::InitBLAS(RenderingContext& rc)
{
	//ジオメトリを作る
	

	{
		// Generate the structures.
		VkDeviceSize resultOffset = 0;
		VkDeviceSize scratchOffset = 0;
		VkAccelerationStructureGeometryKHR geometry = {};
		geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		geometry.pNext = nullptr;
		geometry.geometryType = VK_GEOMETRY_TYPE_AABBS_KHR;
		geometry.geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
		geometry.geometry.aabbs.pNext = nullptr;
		geometry.geometry.aabbs.data.deviceAddress = scene.AabbBuffer().GetDeviceAddress();
		geometry.geometry.aabbs.stride = sizeof(VkAabbPositionsKHR);
		geometry.flags = isOpaque ? VK_GEOMETRY_OPAQUE_BIT_KHR : 0;

		VkAccelerationStructureBuildRangeInfoKHR buildOffsetInfo = {};
		buildOffsetInfo.firstVertex = 0;
		buildOffsetInfo.primitiveOffset = aabbOffset;
		buildOffsetInfo.primitiveCount = aabbCount;
		buildOffsetInfo.transformOffset = 0;

	}
	//バッファを作る
	{
		m_instanceBuffer = CreateBuffer(Device, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, size, VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);



		m_scratchBuffer = CreateBuffer(Device, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, size, VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);
		//デバイスメモリ( VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)



		//デバイスメモリ(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)

	}
	//加速構造を作る
	{
		// Generate the structures.
		VkDeviceSize resultOffset = 0;
		VkDeviceSize scratchOffset = 0;

		resultOffset += bottomAs_[i].BuildSizes().accelerationStructureSize;
		scratchOffset += bottomAs_[i].BuildSizes().buildScratchSize;


		// Create the acceleration structure.
		//CreateAccelerationStructure(m_resultBuffer, resultOffset);
		{
			VkAccelerationStructureCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
			createInfo.pNext = nullptr;
			createInfo.type = buildGeometryInfo_.type;
			createInfo.size = BuildSizes().accelerationStructureSize;
			createInfo.buffer = m_resultBuffer;
			createInfo.offset = resultOffset;

			Check(deviceProcedures_.vkCreateAccelerationStructureKHR(device_.Handle(), &createInfo, nullptr, &accelerationStructure_),
				"create acceleration structure");
			// Build the actual bottom-level acceleration structure
			const VkAccelerationStructureBuildRangeInfoKHR* pBuildOffsetInfo = geometries_.BuildOffsetInfo().data();

			buildGeometryInfo_.dstAccelerationStructure = Handle();
			buildGeometryInfo_.scratchData.deviceAddress = m_scratchBuffer.GetDeviceAddress() + scratchOffset;

			deviceProcedures_.vkCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildGeometryInfo_, &pBuildOffsetInfo);
		}

	}


}
