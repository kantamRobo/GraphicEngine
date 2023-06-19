#include "RWStructuredBuffer.h"
#include "stdafx.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"

void RWStructuredBuffer::RegistImageView()
{
	VkImage image;
    VkImageCreateInfo imageCI{
          VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr,
      0,
      VK_IMAGE_TYPE_2D,
      VK_FORMAT_R8G8B8A8_UNORM, { uint32_t(width), uint32_t(height), 1u },
      1, 1,
      VK_SAMPLE_COUNT_1_BIT,
      VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      0, nullptr,
      VK_IMAGE_LAYOUT_UNDEFINED
    };
    VkImage image;
    VkImageViewCreateInfo viewCI{
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      nullptr, 0,
      image,
      VK_IMAGE_VIEW_TYPE_2D, imageCI.format,
      book_util::DefaultComponentMapping(),
      { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}
    };
    VkImageView view;
    vkCreateImageView(m_device, &viewCI, nullptr, &view);

}
