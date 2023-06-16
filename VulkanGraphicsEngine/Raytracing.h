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

	const int MAX_TRACE_RECURSION_DEPTH = 4;

	//TODO:���[�J�����[�g�V�O�l�`���ɕC�G����@�\�𒲂ׂ�


	//�V�F�[�_�[
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
		const wchar_t* name;				//�q�b�g�O���[�v�̖��O�B
		const wchar_t* chsHitShaderName;	//�ł��߂��|���S���Ƀq�b�g�����Ƃ��ɌĂ΂��V�F�[�_�[�̖��O�B
		const wchar_t* anyHitShaderName;	//any shader???
	};
	//�q�b�g�O���[�v�̖��O�̔z��B
	const SHitGroup hitGroups[] = {
		{ L"HitGroup",			shaderDatas[eShader_PBRChs].entryPointName,	nullptr },
		{ L"ShadowHitGroup",	shaderDatas[eShader_ShadowChs].entryPointName, nullptr },
	};

	/// <summary>
	/// �V�F�[�_�[�e�[�u���ɓo�^����Ă���ImageView��1�v�f
	/// </summary>
	/// <remarks>
	/// ���̗񋓎q�̕��т�t���W�X�^�̔ԍ��ɂȂ�܂��B
	/// �V�F�[�_�[�e�[�u���ɂ͊e�C���X�^���X���ƂɃV�F�[�_�[���\�[�X�̃f�B�X�N���v�^���o�^����Ă��܂��B
	/// ���̗񋓎q���e�C���X�^���X�Ɋ��蓖�Ă��Ă���V�F�[�_�[���\�[�X��\���Ă��܂��B
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
		eNumRayGenerationSRV = eEndRayGenerationImageView - eStartRayGenerationImageView,	//���C�W�F�l���[�V�����V�F�[�_�[�Ŏg�p����SRV�̐��B

	 };

	enum EHitShaderBindingTable {
		eHitShaderDescriptorTable_ImageView_Uniform,	//SRV��CBV
		eHitShaderDescriptorTable_Sampler,	//�T���v��
		eHitShaderDescriptorTable_Num       //�e�[�u���̐��B
	};


}
