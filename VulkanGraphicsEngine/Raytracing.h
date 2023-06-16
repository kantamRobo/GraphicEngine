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
		eHitGroup_PBRCameraRay,	//PBRマテリアルにカメラレイが衝突するときのヒットグループ。
		eHitGroup_PBRShadowRay,	//PBRマテリアルにシャドウレイが衝突するときのヒットグループ。
		eHitGroup_Num,			//ヒットグループの数。
	};

	//TODO VulkanでHeapPropertiesに匹敵するものはないか
	//調べる




}