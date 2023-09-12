

#include "IVulkanShaderResource.h"
#include "stb_image.h"
#include "VulkanTexture.h"


//IOから画像をロードし、テクスチャを作る。

void VulkanTexture::LoadTextureFromStorage(VkDevice device,const std::string& filepath,VkFormat& format)
{
	int channels = 0;
	auto* pImage = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

	{
		//テクスチャのVkImageを生成

		m_textureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		m_textureInfo.extent = { uint32_t(width),uint32_t(height),1 };
		m_textureInfo.format = format;
		m_textureInfo.imageType = VK_IMAGE_TYPE_2D;
		m_textureInfo.arrayLayers = 1;
		m_textureInfo.mipLevels = 1;
		m_textureInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		m_textureInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		vkCreateImage(device, &m_textureInfo, nullptr, &m_texture);

		//メモリ量の算出
		VkMemoryRequirements reqs = {};
		vkGetImageMemoryRequirements(device, m_texture, &reqs);
		VkMemoryAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.allocationSize = reqs.size;

		// メモリタイプの判定
		info.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		//メモリの確保
		vkAllocateMemory(device, &info, nullptr, );

			//VkAllocateMemoryに渡すVkDeviceMemoryはどうするか・・・

			//メモリのバインド
			vkBindImageMemory(device, m_texture, , 0);
	}

	ステージングバッファというの分からないので、後日調べる。
	{

	
	
	}

	VkCommandBufferBeginInfo commandBI{};
	commandBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkBeginCommandBuffer(command, &commandBI);
	setImageMemoryBarrier(command, texture.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vkCmdCopyBufferToImage(command, stagingBuffer.buffer, texture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

	setImageMemoryBarrier(command, texture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	vkEndCommandBuffer(command);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &command;
	vkQueueSubmit(m_deviceQueue, 1, &submitInfo, VK_NULL_HANDLE);
	{
		// テクスチャ参照用のビューを生成
		VkImageViewCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ci.image = texture.image;
		ci.format = format;
		ci.components = {
		  VK_COMPONENT_SWIZZLE_R,
		  VK_COMPONENT_SWIZZLE_G,
		  VK_COMPONENT_SWIZZLE_B,
		  VK_COMPONENT_SWIZZLE_A,
		};
		ci.subresourceRange = {
		  VK_IMAGE_ASPECT_COLOR_BIT,0,1,0,1
		};
		vkCreateImageView(m_device, &ci, nullptr, &texture.view);
	}

	vkDeviceWaitIdle(m_device);
	vkFreeCommandBuffers(m_device, m_commandPool, 1, &command);

}
