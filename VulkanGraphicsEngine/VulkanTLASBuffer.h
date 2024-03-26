#pragma once
#include <vulkan/vulkan.h>
class VulkanTLASBuffer
{

	//インスタンス
	//バッファ二種類(バッファ・スクラッチバッファ)
	VkBuffer m_tlasbuffer;
	VkBuffer m_tlasScracthbuffer;
	//デバイスメモリ二種類
	VkDeviceMemory tlasbuffermemory;
	VkDeviceMemory tlasscratchbuffermemory;
	void InitTLAS();

};

