#pragma once

https://2.bp.blogspot.com/-EpIVpOGFHYI/XuGTbz7skEI/AAAAAAAACdA/7zx2iybU85AqJfUROEHu1NACRXI5aeCUgCLcBGAsYHQ/s1600/20200611.jpg

class RWStructuredBuffer{
void Init()
{
    struct BufferObject
    {
        VkBuffer buffer;
        VkDeviceMemory memory;
    };


    VkMemoryAllocateInfo info{
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        reqs.size,
        GetMemoryTypeIndex(reqs.memoryTypeBits, props)
    };


    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(logical_device, buffer, &memory_requirements);



    //バッファ作成
    VkResult result = vkAllocateMemory(logical_device, &buffer_memory_allocate_info, nullptr, &memory_object);


    VkResult result = vkBindBufferMemory(logical_device, buffer, memory_object, 0);





}


void RegistShaderResourceView( int bufferNo)
{
    VkImageCreateInfo image_create_info = {
     VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,                // VkStructureType          sType
     nullptr,                                            // const void             * pNext
     cubemap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u, // VkImageCreateFlags       flags
     type,                                               // VkImageType              imageType
     format,                                             // VkFormat                 format
     size,                                               // VkExtent3D               extent
     num_mipmaps,                                        // uint32_t                 mipLevels
     cubemap ? 6 * num_layers : num_layers,              // uint32_t                 arrayLayers
     samples,                                            // VkSampleCountFlagBits    samples
     VK_IMAGE_TILING_OPTIMAL,                            // VkImageTiling            tiling
     usage_scenarios,                                    // VkImageUsageFlags        usage
     VK_SHARING_MODE_EXCLUSIVE,                          // VkSharingMode            sharingMode
     0,                                                  // uint32_t                 queueFamilyIndexCount
     nullptr,                                            // const uint32_t         * pQueueFamilyIndices
     VK_IMAGE_LAYOUT_UNDEFINED                           // VkImageLayout            initialLayout
    };

    VkResult result = vkCreateImage(logical_device, &image_create_info, nullptr, &image);
    if (VK_SUCCESS != result) {
        std::cout << "Could not create an image." << std::endl;
        return false;
    }
    return true;

}

}



