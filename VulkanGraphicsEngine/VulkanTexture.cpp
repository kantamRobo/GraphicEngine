

#include "IVulkanShaderResource.h"
#include "stb_image.h"
#include "VulkanTexture.h"


//IOから画像をロードし、テクスチャを作る。
void VulkanTexture::LoadTextureFromStorage(CoreGraphicsEngine& coregraphicsengine,VkDevice device,const std::string& filepath,VkFormat& format,void* initialData)
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
	uint32_t imageSize = width * height * sizeof(uint32_t);
	VkBuffer staging_buffer;
	VkDeviceMemory staging_memory;
	VkBufferCreateInfo stagingci{};
	stagingci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingci.size = imageSize;

		auto result = vkCreateBuffer(device, &stagingci, nullptr, &staging_buffer);


	VkMemoryRequirements reqs;
	vkGetBufferMemoryRequirements(device, staging_buffer, &reqs);
	VkMemoryAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = reqs.size;
	// メモリタイプの判定
	info.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);
	// メモリの確保
	vkAllocateMemory(device, &info, nullptr, &staging_memory);

	// メモリのバインド
	vkBindBufferMemory(device, staging_buffer, staging_memory, 0);

	if ((flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 &&
		initialData != nullptr)
	{
		void* p;
		vkMapMemory(device, staging_memory, 0, VK_WHOLE_SIZE, 0, &p);
		memcpy(p, initialData, imageSize);
		vkUnmapMemory(device, staging_memory);
	}
	/*ステージングバッファというの分からないので、後日調べる。
	{
VkBuffer staging_buffer;
VkDeviceMemory  staging_memory;
VkBufferCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.usage = usage;
  ci.size = size;
  auto result = vkCreateBuffer(m_device, &ci, nullptr, &obj.buffer);
  checkResult(result);

   // メモリ量の算出
  VkMemoryRequirements reqs;
  vkGetBufferMemoryRequirements(m_device, obj.buffer, &reqs);
  VkMemoryAllocateInfo info{};
  info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.allocationSize = reqs.size;
  // メモリタイプの判定
  info.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);
  // メモリの確保
  vkAllocateMemory(m_device, &info, nullptr, &obj.memory);


  // メモリ量の算出
  VkMemoryRequirements reqs;
  vkGetBufferMemoryRequirements(m_device, stagingbuffer, &reqs);
  VkMemoryAllocateInfo info{};
  info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.allocationSize = reqs.size;
  // メモリタイプの判定
  info.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);
  // メモリの確保
  vkAllocateMemory(m_device, &info, nullptr, stagingmemory);




  // メモリのバインド
  vkBindBufferMemory(m_device, stagingbuffer, stagingmemory, 0);


  // メモリのバインド
  vkBindBufferMemory(m_device,stagingbuffer,  stagingmemory, 0);

  if ((flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 &&
	initialData != nullptr)
  {
	void* p;
	vkMapMemory(m_device,  stagingmemory, 0, VK_WHOLE_SIZE, 0, &p);
	memcpy(p, initialData, size);
	vkUnmapMemory(m_device,  stagingmemory);
  }

  if ((flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 &&
	initialData != nullptr)
  {
	void* p;
	vkMapMemory(m_device, stagingmemory, 0, VK_WHOLE_SIZE, 0, &p);
	memcpy(p, initialData, size);
	vkUnmapMemory(m_device,  stagingmemory);
  }
  return obj;
}
     VkBufferCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.usage = usage;
  ci.size = size;
  auto result = vkCreateBuffer(m_device, &ci, nullptr, &stagingbuffer);
  checkResult(result);
	

	// メモリ量の算出
  VkMemoryRequirements reqs;
  vkGetBufferMemoryRequirements(m_device, stagingbuffer, &reqs);
  VkMemoryAllocateInfo info{};
  info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.allocationSize = reqs.size;
  // メモリタイプの判定
  info.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);
  // メモリの確保
  vkAllocateMemory(m_device, &info, nullptr, &stagingmemory);



  // メモリ量の算出
  VkMemoryRequirements reqs;
  vkGetBufferMemoryRequirements(m_device, stagingbuffer, &reqs);
  VkMemoryAllocateInfo info{};
  info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.allocationSize = reqs.size;
  // メモリタイプの判定
  info.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);
  // メモリの確保
  vkAllocateMemory(m_device, &info, nullptr, &stagingmemory);




  // メモリのバインド
  vkBindBufferMemory(m_device,stagingbuffer, stagingmemory, 0);


  // メモリのバインド
  vkBindBufferMemory(m_device, stagingbuffer, stagingmemory, 0);

	}
	*/

	VkCommandBufferBeginInfo commandBI{};
	commandBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkBeginCommandBuffer(coregraphicsengine.commandbuffer, &commandBI);
	setImageMemoryBarrier(coregraphicsengine.commandbuffer, texture.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vkCmdCopyBufferToImage(coregraphicsengine.commandbuffer, staging_buffer, texture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

	setImageMemoryBarrier(coregraphicsengine.commandbuffer, texture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	vkEndCommandBuffer(coregraphicsengine.commandbuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &coregraphicsengine.commandbuffer;
	vkQueueSubmit(coregraphicsengine.devicequeue, 1, &submitInfo, VK_NULL_HANDLE);
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
		vkCreateImageView(device, &ci, nullptr, &texture.view);
	}

	vkDeviceWaitIdle(device);
	vkFreeCommandBuffers(device, coregraphicsengine.commandpool, 1, &coregraphicsengine.commandbuffer);

}
