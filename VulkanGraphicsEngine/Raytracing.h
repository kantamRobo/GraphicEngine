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

	const int MAX_TRACE_RECURSION_DEPTH = 4;

	//TODO:ローカルルートシグネチャに匹敵する機能を調べる


	//シェーダー
	enum EShader {
		eShader_RayGeneration,
		eShader_Miss,
		eShader_PBRChs,
		eShader_ShadowChs,
		eShader_ShadowMiss,
		eShader_Num,

	};

	enum EShaderCategory {
		eShaderCategory_RayGenerator,
		eShaderCategory_Miss,
		eShaderCategory_ClosetHit,

	};
	struct ShaderData {
		const wchar_t* entryPointName;
		EShaderCategory category;
		EHitgroup hitgroup;
	};

	
	const ShaderData shaderDatas[] = {
		{L"rayGen",eShaderCategory_RayGenerator,eHitGroup_Undef},
		{L"miss",eShaderCategory_Miss,eHitGroup_Undef},
		{L"chs",eShaderCategory_ClosetHit,eHitGroup_Undef},
		{L"shadowChs",eShaderCategory_ClosetHit,eHitGroup_PBRShadowRay},
		{L"shadowMiss",eShaderCategory_Miss,eHitGroup_Undef}

	};



	static_assert(ARRAYSIZE(shaderDatas) == eShader_Num, "shaderDatas arraySize is invalid!! shaderDatas arraySize must be equal to eShader_Num");


	struct SHitGroup
	{
		const wchar_t* name;				//ヒットグループの名前。
		const wchar_t* chsHitShaderName;	//最も近いポリゴンにヒットしたときに呼ばれるシェーダーの名前。
		const wchar_t* anyHitShaderName;	//any shader???
	};
	//ヒットグループの名前の配列。
	const SHitGroup hitGroups[] = {
		{ L"HitGroup",			shaderDatas[eShader_PBRChs].entryPointName,	nullptr },
		{ L"ShadowHitGroup",	shaderDatas[eShader_ShadowChs].entryPointName, nullptr },
	};

	/// <summary>
	/// シェーダーテーブルに登録されているImageViewの1要素
	/// </summary>
	/// <remarks>
	/// この列挙子の並びがtレジスタの番号になります。
	/// シェーダーテーブルには各インスタンスごとにシェーダーリソースのディスクリプタが登録されています。
	/// この列挙子が各インスタンスに割り当てられているシェーダーリソースを表しています。
	 ///</remarks>
	 
	enum class ESRV_OneEntry {
		eStartRayGenerationImageView,
		eEndRayGenerationImageView,
		eTLAS = eStartRayGenerationImageView,
		eEndGenerataionSRV,
		eAlbedoMap = eEndGenerataionSRV,
		eNormalMap,
		eSpecularMap,
		eRefrectionMap,
		eVertexBuffer,
		eIndexBuffer,
		eNum,
		eNumRayGenerationSRV = eEndRayGenerationImageView - eStartRayGenerationImageView,	//レイジェネレーションシェーダーで使用するSRVの数。

	 };

	enum EHitShaderBindingTable {
		eHitShaderDescriptorTable_ImageView_Uniform,	//SRVとCBV
		eHitShaderDescriptorTable_Sampler,	//サンプラ
		eHitShaderDescriptorTable_Num       //テーブルの数。
	};


}
