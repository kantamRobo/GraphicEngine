#pragma once
#include "../VulkanGraphicsEngine/stdafx.h"
namespace raytracing {
	extern VkBuffer CreateBuffer(VkDevice pDevice,
		uint64_t size,
		VkBufferUsageFlags  usage,
		VkBufferCreateFlags create
	);


	struct AcclerationStructureBuffers {
		VkBuffer pScratch = nullptr;
		VkBuffer pResult = nullptr;
		VkBuffer pInstanceDesc = nullptr;


	};

	enum EHitgroup {
		eHitGroup_Undef = -1,
		eHitGroup_PBRCameraRay,	//PBR�}�e���A���ɃJ�������C���Փ˂���Ƃ��̃q�b�g�O���[�v�B
		eHitGroup_PBRShadowRay,	//PBR�}�e���A���ɃV���h�E���C���Փ˂���Ƃ��̃q�b�g�O���[�v�B
		eHitGroup_Num,			//�q�b�g�O���[�v�̐��B
	};

	//TODO Vulkan��HeapProperties�ɕC�G������̂͂Ȃ���
	//���ׂ�




}