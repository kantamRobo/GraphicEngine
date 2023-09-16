#include "VkRaytracingInstance.h"
#include "RenderingContext.h"
#include "UniformBuffer.h"
#include "VulkanRaytracingEngine.h"
#include "Material.h"


namespace raytracing {
	using namespace std;


	VkDeviceMemory AllocateMemory(VkBuffer buffer, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProps)
	{
		VkDeviceMemory memory;
		VkMemoryRequirements reqs;
		vkGetBufferMemoryRequirements(m_device, buffer, &reqs);
		VkMemoryAllocateInfo info{
		  VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		  nullptr,
		  reqs.size,
		  GetMemoryTypeIndex(reqs.memoryTypeBits, memProps);
		};
	}
	VkBuffer CreateBuffer(
		VkDevice Device,
		uint64_t size,
		VkBufferCreateFlags flags

	) {

		VkBufferCreateInfo bufferCI
		{

			 VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		  nullptr
		};
		bufferCI.size = size;
		bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		VkBuffer buffer;
		vkCreateBuffer(Device, &bufferCI, memory, &buffer);
		// メモリの確保.
		auto memory = AllocateMemory(buffer, bufferCI.usage, memProps);
		vkBindBufferMemory(m_device, buffer, memory, 0);

		ret.m_buffer = buffer;
		ret.m_memory = memory;
		ret.m_memProps = memProps;
		ret.m_usage = usage;

		if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) {
			ret.m_deviceAddress = GetDeviceAddress(ret.m_buffer);
		}




	}

	void Engine::CreateShaderResources()
	{
		auto Device = g_graphicEngine->GetDevice();

		//レイトレの結果の出力先のテクスチャを作成
		VkImageCreateInfo texinfo = {};

		texinfo.flags =
			texinfo.mipLevels = 1;
		texinfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		texinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		texinfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		texinfo.usage =
		texinfo.extent.height= g_graphicsEngine->GetFrameBufferHeight();
		texinfo.extent.width = g_graphicsEngine->GetFrameBufferWidth();

		//レイジェネレーション用の定数バッファ。
		Camera cam;
		cam.pos = g_camera3D->GetPosition();
		cam.mRot = g_camera3D->GetCameraRotation();
		cam.aspect = g_camera3D->GetAspect();
		cam.fNear = g_camera3D->GetNear();
		cam.fFar = g_camera3D->GetFar();
		m_rayGenerationCB.Init(sizeof(Camera), &cam);

	}


	void Engine::Dispatch(RenderingContext& rc)
	{
		//カリカリ
		Camera cam;
		cam.pos = g_camera3D->GetPosition();
		cam.mRot = g_camera3D->GetCameraRotation();
		cam.aspect = g_camera3D->GetAspect();
		cam.fNear = g_camera3D->GetNear();
		cam.fFar = g_camera3D->GetFar();
		m_rayGenerationCB.CopyToVRAM(cam);

		//todoバリア





	}

}