#include "Raytracing.h"
#include "VulkanTLASBuffer.h"
using namespace raytracing;

void VulkanTLASBuffer::InitTLAS()
{
	//インスタンス作成
	{const auto& device = bottomLevelAs.Device();
	const auto& deviceProcedure = bottomLevelAs.DeviceProcedures();

	VkAccelerationStructureDeviceAddressInfoKHR addressInfo = {};
	addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
	addressInfo.accelerationStructure = bottomLevelAs.Handle();

	const VkDeviceAddress address = deviceProcedure.vkGetAccelerationStructureDeviceAddressKHR(device.Handle(), &addressInfo);

	VkAccelerationStructureInstanceKHR instance = {};
	instance.instanceCustomIndex = instanceId;
	instance.mask = 0xFF; // The visibility mask is always set of 0xFF, but if some instances would need to be ignored in some cases, this flag should be passed by the application.
	instance.instanceShaderBindingTableRecordOffset = hitGroupId; // Set the hit group index, that will be used to find the shader code to execute when hitting the geometry.
	instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR; // Disable culling - more fine control could be provided by the application
	instance.accelerationStructureReference = address;

	// The instance.transform value only contains 12 values, corresponding to a 4x3 matrix,
	// hence saving the last row that is anyway always (0,0,0,1).
	// Since the matrix is row-major, we simply copy the first 12 values of the original 4x4 matrix
	std::memcpy(&instance.transform, &transform, sizeof(instance.transform));
	}

	{
		// Create and copy instances buffer (do it in a separate one-time synchronous command buffer).
	BufferUtil::CreateDeviceBuffer(CommandPool(), "TLAS Instances", VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, instances, instancesBuffer_, instancesBufferMemory_);

	// Memory barrier for the bottom level acceleration structure builds.
	AccelerationStructure::MemoryBarrier(commandBuffer);
	
	}
	//バッファ作成
	m_tlasbuffer = CreateBuffer(Device, size, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	//デバイスメモリ VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	
	//デバイスメモリ VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT


	{
		// Create the acceleration structure.
		CreateAccelerationStructure(resultBuffer, resultOffset);

		// Build the actual bottom-level acceleration structure
		VkAccelerationStructureBuildRangeInfoKHR buildOffsetInfo = {};
		buildOffsetInfo.primitiveCount = instancesCount_;

		const VkAccelerationStructureBuildRangeInfoKHR* pBuildOffsetInfo = &buildOffsetInfo;

		buildGeometryInfo_.dstAccelerationStructure = Handle();
		buildGeometryInfo_.scratchData.deviceAddress = scratchBuffer.GetDeviceAddress() + scratchOffset;

		deviceProcedures_.vkCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildGeometryInfo_, &pBuildOffsetInfo);

	}
}