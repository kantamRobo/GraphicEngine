#include "RayTracingPSO.h"
#include "stdafx.h"
void raytracing::PSO::InitRTPSO(const std::shared_ptr<DescriptorHeaps> descriptorHeaps)
{
	m_srvUavCbvHeap = descriptorHeaps->GetSrvUavCbvDescriptorHeap();
	using namespace BuildSubObjctHelper;

	std::array<D3D12_STATE_SUBOBJECT>, 14 > subobjects;
	uint32_t index = 0;

	//DXIL���C�u�������쐬
	//���C�g���[�V���O�p�̃V�F�[�_�[�����[�h
	Shader raytraceShader;
	raytraceShader.LoadRaytracing(L"Assets/shader/sample.fx");

	D3D12_EXPORT_DESC libExport[eShader_Num];
	for (int i = 0; i < eShader_Num; i++)
	{
		libExport[i].Name = shaderDatas[i].entryPointName;
		libExport[i].ExportToRename = nullptr;
		libExport[i].Flags = D3D12_EXPORT_FLAG_NONE;
	};


	D3D12_DXIL_LIBRARY_DESC dxLibdesc;
	auto pBlob = raytraceShader.GetCompiledDxcBlob();
	dxLibdesc.DXILLibrary.pShaderBytecode = pBlob->GetBufferPointer();
	dxLibdesc.DXILLibrary.BytecodeLength = pBlob->GetBufferSize();
	dxLibdesc.NumExports = ARRAYSIZE(libExport);
	dxLibdesc.pExports = libExport;

	subobjects[index].Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	subobjects[index].pDesc = &dxLibdesc;

	index++;

	//HitGroup�̃T�u�I�u�W�F�N�g���쐬
	std::vector<HitGroupSubObject> hitGroupSos;
	hitGroupSOs.resize(eHitGroup_Num);
	for (int i = 0; i < eHitGroup_Num; i++) {
		hitGroupSOs[i].Init(hitGroups[i]);
		subobjects[index++] = hitGroupSOs[i].subObject; // 1 Hit Group

	}

	auto BuildAndRegistRootSignatureAndSubobjectFunc = [&](
		LocalRootSignatureSubObject& rsSO, ExportAssociationSubObject& ass, , ELocalRootSignature eRS, const WCHAR* exportNames[]
		)
	{
		if (eRS == eLocalRootSignature_Raygen) {
			rsSO.Init(CreateRayGenRootSignatureesc().desc, L"RayGenRootSignature");
		}
		if (eRS == eLocalRootSignature_PBRMaterialHit) {
			rsSO.Init(CreatePBRMatterialHitRootSignatureDesc().desc, L"PBRMaterialHitGenRootSignature");
		}
		if (eRS == eLocalRootSignature_Empty) {
			D3D12_ROOT_SIGNATURE_DESC emptyDesc = {};
			emptyDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
			rsSO.Init(emptyDesc, L"EmptyRootSignature");
		}
		subobjects[index] = rsSO.subobject;
		uint32_t rgSOIndex = index++;

		int useRootSignature = 0;
		for (auto& shaderData : shaderDatas) {
			if (shaderData.useLocalRootSignature == eRS) {
				//�����͎w�肳�ꂽ���[�g�V�O�l�`�����g���B
				exportNames[useRootSignature] = shaderData.entryPointName;
				useRootSignature++;
			}
		}
		ass.Init(exportNames, useRootSignature, &(subobjects[rgSOIndex]));
		subobjects[index++] = ass.subobject;
	};

	//���[�g�V�O�l�`���ƃV�F�[�_�[�̊֘A�t�����s���T�u�I�u�W�F�N�g������Ă���
	LocalRootSignatureSubObject rayGenSignatureSO, modelSignatureSO, emptySignatureSO;
	ExportAssociationSubobject rayGenAssSO, modelAssSO, emptuAssSO;

	const WCHAR* rayGenExportName[eShader_Num];
	const WCHAR* modelExportName[eShader_Num];
	const WCHAR* emptyExportName[eShader_Num];

	BuildAndRegistRootSignatureAndAssSubobjectFunc(rayGenSignatureSO, rayGenAssSO, eLocalRootSignature_Raygen, rayGenExportName);
	BuildAndRegistRootSignatureAndAssSubobjectFunc(modelSignatureSO, modelAssSO, eLocalRootSignature_PBRMaterialHit, modelExportName);
	BuildAndRegistRootSignatureAndAssSubobjectFunc(emptySignatureSO, emptyAssSO, eLocalRootSignature_Empty, emptyExportName);

	//Payload�̃T�C�Y�ƈ����̐��͂Ƃ肠�����Œ�ŁB
	struct RayPayload {
		Vector4 color;
		Vector4 reflectionColor;
		Vector4 hit_depth;

	};

	shaderConfig.Init(sizeof(float) : 2, sizeof(RayPayload));
	subobjects[index] = shaderConfig.subobject;

	uint32_t shaderConfigIndex = index++;
	ExportAssociationSubobject configAssociationSO;
	const WCHAR* entryPointNames[eShader_Num];
	for (int i = 0; i < eShader_Num; i++)
	{
		entryPointNames[i] = shaderDatas[i].entryPointName;
	}
	configAssociationSO.Init(entryPointNames, eShader_Num, &subobjects[shaderConfigIndex]);
	subobjects[index++] = configAssociationSO.subobject;

	// �p�C�v���C���ݒ�̃T�u�I�u�W�F�N�g���쐬�B
	PipelineConfigSubobject config;
	subobjects[index++] = config.subobject;

	// �O���[�o�����[�g�V�O�l�`���̃T�u�I�u�W�F�N�g���쐬�B
	GlobalRootSignatureSubobject root;
	m_emptyRootSignature = root.pRootSig;
	subobjects[index++] = root.subobject;
}
