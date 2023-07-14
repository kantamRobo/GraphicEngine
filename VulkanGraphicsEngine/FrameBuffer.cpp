#include "FrameBuffer.h"

bool FrameBuffer::InitFrameBuffer( CoreGraphicsEngine* in_engine,const unsigned int width, const unsigned int height,VkFormat depthstencilformat)
{
	m_coreGraphicsengine = in_engine;
   
	//フレームバッファ用のテクスチャ作成();
	CreateTextureforFramebuffer(m_coreGraphicsengine->m_renderpass, width, height);
	//デプスステンシルテクスチャの作成();
	CreateDepthStencilTexture(m_coreGraphicsengine, width, height, depthstencilformat);
	//フレームバッファ用のインフォを作成();


	return false;
}

void FrameBuffer::CreateTextureforFramebuffer(const VkRenderPass& renderpass,unsigned int width,unsigned int height)
{

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(width);
	imageInfo.extent.height = static_cast<uint32_t>(height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	auto result = vkCreateImage(m_coreGraphicsengine->Device, &imageInfo, nullptr, &framebuffertexture);

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_coreGraphicsengine->Device, framebuffertexture, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	auto allocateresult = vkAllocateMemory(m_coreGraphicsengine->Device, &allocInfo, nullptr, &framebuffermemory);
		

	vkBindImageMemory(m_coreGraphicsengine->Device, framebuffertexture, framebuffermemory, 0);

	 VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateImage(m_coreGraphicsengine->Device, &imageInfo, nullptr, &framebuffertexture);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_coreGraphicsengine->Device, framebuffertexture, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    vkAllocateMemory(m_coreGraphicsengine->Device, &allocInfo, nullptr, &framebuffermemory);
      

    vkBindImageMemory(m_coreGraphicsengine->Device, framebuffertexture, framebuffermemory, 0);
}

bool FrameBuffer::CreateDepthStencilTexture(CoreGraphicsEngine* ge, const unsigned int width, const unsigned int height,VkFormat format)
{



	VkImageCreateInfo imageinfo = {};

	imageinfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageinfo.imageType = VK_IMAGE_TYPE_2D;
	imageinfo.format = format;
	imageinfo.extent = { width,height,1 };
	imageinfo.mipLevels = 1;
	imageinfo.arrayLayers = 1;
	imageinfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageinfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageinfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	auto result = vkCreateImage(m_coreGraphicsengine->Device, &imageinfo, nullptr, &depthstencil);
	
		VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_coreGraphicsengine->Device, depthstencil, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT); // 適切なメモリタイプを選択する

	VkDeviceMemory depthStencilMemory;
	vkAllocateMemory(m_coreGraphicsengine->Device, &allocInfo, nullptr, &depthStencilMemory);

	vkBindImageMemory(m_coreGraphicsengine->Device, depthstencil, depthStencilMemory, 0);


	VkImageViewCreateInfo viewinfo = {};

	viewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewinfo.image = depthstencil;
	viewinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewinfo.format = format;
	viewinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	viewinfo.subresourceRange.baseMipLevel = 0;
	viewinfo.subresourceRange.levelCount = 1;
	viewinfo.subresourceRange.baseArrayLayer = 0;
	viewinfo.subresourceRange.layerCount = 1;

	vkCreateImageView(m_coreGraphicsengine->Device, &viewinfo, nullptr, &depthstencilview);

	VkAttachmentDescription depthStencilAttachment = {};
	depthStencilAttachment.format = format; // デプスステンシルフォーマットを指定する
	depthStencilAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // レンダーパスの前にバッファをクリアするかどうかを指定する
	depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthStencilAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthStencilAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthStencilAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthStencilAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthStencilAttachmentRef = {};
	depthStencilAttachmentRef.attachment = attachmentIndex; // レンダーパス内のアタッチメントのインデックスを指定する
	depthStencilAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

}
