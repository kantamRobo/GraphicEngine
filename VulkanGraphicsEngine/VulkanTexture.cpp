#include "VulkanTexture.h"
#include "IVulkanShaderResource.h"
#include "stdafx.h"

#include "stb/stb_include.h"
#include "stb/stb_image.h"
VulkanTexture::VulkanTexture(const wchar_t* filePath, unsigned int size)
{
}

void VulkanTexture::InitFromSTB(const char* memory, unsigned int size,int width,int height)
{
	unsigned char* pixels=nullptr;
	int bpp = 0;
	pixels = stbi_load(memory, &width, &height, &bpp, 0);

}


void VulkanTexture::RegistImageView(int bufferNo,VkFormat format,)
{
	auto device = g_graphicsEngine->GetDevice();

	VkImageViewCreateInfo imageviewinfo = {};

	imageviewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageviewinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageviewinfo.format = format;
	imageviewinfo.components = {

		 VK_COMPONENT_SWIZZLE_R,
	  VK_COMPONENT_SWIZZLE_G,
	  VK_COMPONENT_SWIZZLE_B,
	  VK_COMPONENT_SWIZZLE_A,
	};

	imageviewinfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    imageviewinfo.image = TODO:SwapChainをどこからか。おそらくグラフィックエンジンからだと思われる。
}