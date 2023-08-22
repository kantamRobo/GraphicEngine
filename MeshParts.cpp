#include "stdafx.h"

#include "Skeleton.h"
#include "Material.h"

#include "IndexBuffer.h"
#include "Matrix.h"
#include "Vector.h"
#include "DescriptorHeap.h"
#include <memory>
#include "MeshParts.h"
MeshParts::~MeshParts()
{
	for (auto& mesh : m_meshs)
	{
		//�C���f�b�N�X�o�b�t�@���폜�B
		for (auto& ib : mesh->m_indexBufferArray) {
			mesh.reset();
		}
		//�}�e���A�����폜
		for (auto& mat : mesh->m_materials)
		{
			mat.reset();
		}
	//���b�V�����폜
		mesh.reset();
	}


}

void MeshParts::InitFromAssimpFile(const aiScene* scene,const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, void* expandData, int expandDataSize, const std::array<std::shared_ptr<IShaderResource>, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter)
{
	//assimp���璸�_�����[�h���������A
	// ���������G�Ȃ̂Œ������Ă��烁�b�V���������m�ۂ�
	//����������

	int meshNo = 0;
	int materialNo = 0;

	CreateMeshFromAssimpMesh(meshNo,
		materialNo,
		fxFilePath,
		vsEntryPointFunc,
		vsSkinEntryPointFunc,
		vsSkinEntryPointFunc,
		psEntryPointFunc,
		,
		samplerFilter
	);

	//���ʒ萔�o�b�t�@�̍쐬�B
	m_commonConstantBuffer.InitConstBuffer(sizeof(SConstantBuffer), nullptr);

	//���[�U�[�g���p�̒萔�o�b�t�@���쐬�B
	if (expandData) {
		m_expandConstantBuffer.InitConstBuffer(expandDataSize, nullptr);
		m_expandData = expandData;
	}

	for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
		m_expandShaderResourceView[i] = expandShaderResourceView[i];
	}

	//�f�B�X�N���v�^�q�[�v���쐬�B
	CreateDescriptorHeaps();
}

void MeshParts::BindSkeleton(std::shared_ptr<Skeleton> skeleton)
{
	m_skeleton = skeleton;
	m_boneMatricesStructureBuffer->InitStructuredBuffer(
		sizeof(Matrix),
		m_skeleton->GetNumBones(),
		m_skeleton->GetBoneMatricesTopAddress()
		);


}

void MeshParts::DrawCommon(std::shared_ptr<RenderContext> rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj)
{
	//���b�V�����ƂɃh���[
	//�v���~�e�B�u�̃g�|���W�[�̓g���C�A���O�����X�g�̂݁B
	rc->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�萔�o�b�t�@���X�V����B
	SConstantBuffer cb;
	cb.mWorld = mWorld;
	cb.mView = mView;
	cb.mProj = mProj;
	m_commonConstantBuffer.CopyToVRAM(cb);

	if (m_expandData) {
		m_expandConstantBuffer.CopyToVRAM(m_expandData);
	}
	if (m_boneMatricesStructureBuffer->IsInited()) {
		//�{�[���s����X�V����B
		m_boneMatricesStructureBuffer->Update(m_skeleton->GetBoneMatricesTopAddress());
	}
}


void MeshParts::DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance, const Matrix& mView, const Matrix& mProj)
{
	//�萔�o�b�t�@�̐ݒ�A�X�V�ȂǕ`��̋��ʏ��������s����B
	DrawCommon(rc, g_matIdentity, mView, mProj);

	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		//1. ���_�o�b�t�@��ݒ�B
		rc->SetVertexBuffer(mesh->m_vertexBuffer);
		//�}�e���A�����ƂɃh���[�B
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			//���̃}�e���A�����\���Ă��郁�b�V���̕`��J�n�B
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);
			//2. �f�B�X�N���v�^�q�[�v��ݒ�B
			rc->SetDescriptorHeap(m_descriptorHeap.get());
			//3. �C���f�b�N�X�o�b�t�@��ݒ�B
			auto& ib = mesh->m_indexBufferArray[matNo];
			rc->SetIndexBuffer(*ib);

			//4. �h���[�R�[�������s�B
			rc->DrawIndexedInstanced(ib->GetCount(), numInstance);
			descriptorHeapNo++;
		}
	}

}


void MeshParts::Draw(std::shared_ptr<RenderContext> rc, 
	const Matrix& mWorld, 
	const Matrix& mView,
	const Matrix& mProj)
{
	//�萔�o�b�t�@�̐ݒ�A�X�V�ȂǕ`��̋��ʏ��������s����B
	DrawCommon(rc, mWorld, mView, mProj);

	int descriptorHeapNo = 0;

	for (auto& mesh : m_meshs)
	{
		rc->SetVertexBuffer(mesh->m_vertexBuffer);

		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo)
		{
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);

			rc->SetDescriptorHeap(m_descriptorHeap.get());

		}
	}

}

void MeshParts::DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance, const Matrix& mView, const Matrix& mProj)
{
	//�萔�o�b�t�@�̐ݒ�A�X�V�ȂǕ`��̋��ʏ��������s����B
	DrawCommon(rc, g_matIdentity, mView, mProj);

	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		//1. ���_�o�b�t�@��ݒ�B
		rc->SetVertexBuffer(mesh->m_vertexBuffer);
		//�}�e���A�����ƂɃh���[�B
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			//���̃}�e���A�����\���Ă��郁�b�V���̕`��J�n�B
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);
			//2. �f�B�X�N���v�^�q�[�v��ݒ�B
			rc->SetDescriptorHeap(m_descriptorHeap.get());
			//3. �C���f�b�N�X�o�b�t�@��ݒ�B
			auto& ib = mesh->m_indexBufferArray[matNo];
			rc->SetIndexBuffer(*ib);

			//4. �h���[�R�[�������s�B
			rc->DrawIndexedInstanced(ib->GetCount(), numInstance);
			descriptorHeapNo++;
		}
	}
}


void MeshParts::CreateDescriptorHeaps()
{

	//�f�B�X�N���v�^�q�[�v���\�z���Ă����B
	int srvNo = 0;
	int cbNo = 0;
	for (auto& mesh : m_meshs) {
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {

			//�f�B�X�N���v�^�q�[�v�Ƀf�B�X�N���v�^��o�^���Ă����B
			m_descriptorHeap->RegistShaderResource(srvNo, mesh->m_materials[matNo]->GetAlbedoMap());			//�A���x�h�}�b�v�B
			m_descriptorHeap->RegistShaderResource(srvNo + 1, mesh->m_materials[matNo]->GetNormalMap());		//�@���}�b�v�B
			m_descriptorHeap->RegistShaderResource(srvNo + 2, mesh->m_materials[matNo]->GetSpecularMap());		//�X�y�L�����}�b�v�B
			m_descriptorHeap->RegistShaderResource(srvNo + 3, m_boneMatricesStructureBuffer);					//�{�[���̃X�g���N�`���[�h�o�b�t�@�B
			for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
				if (m_expandShaderResourceView[i]) {
					m_descriptorHeap->RegistShaderResource(srvNo + EXPAND_SRV_REG__START_NO + i, m_expandShaderResourceView[i]);
				}
			}
			srvNo += NUM_SRV_ONE_MATERIAL;
			m_descriptorHeap->RegistConstantBuffer(cbNo, m_commonConstantBuffer);
			if (m_expandConstantBuffer.IsValid()) {
				m_descriptorHeap->RegistConstantBuffer(cbNo + 1, m_expandConstantBuffer);
			}
			cbNo += NUM_CBV_ONE_MATERIAL;
		}
	}
	m_descriptorHeap->Commit();
}

void MeshParts::CreateMeshFromAssimpMesh(int meshNo, int& materialNum, const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter)
{
	//1. ���_�o�b�t�@���쐬�B
	int numVertex = (int)tkmMesh.vertexBuffer.size();
	int vertexStride = sizeof(TkmFile::SVertex);
	auto mesh = new SMesh;
	mesh->skinFlags.reserve(tkmMesh.materials.size());
	mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
	mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

	auto SetSkinFlag = [&](int index) {
		if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
			//�X�L��������B
			mesh->skinFlags.push_back(1);
		}
		else {
			//�X�L���Ȃ��B
			mesh->skinFlags.push_back(0);
		}
	};
	//2. �C���f�b�N�X�o�b�t�@���쐬�B
	if (!tkmMesh.indexBuffer16Array.empty()) {
		//�C���f�b�N�X�̃T�C�Y��2byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer16Array) {
			auto ib = new IndexBuffer;
			ib->Init(static_cast<int>(tkIb.indices.size()) * 2, 2);
			ib->Copy((uint16_t*)&tkIb.indices.at(0));

			//�X�L�������邩�ǂ�����ݒ肷��B
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	else {
		//�C���f�b�N�X�̃T�C�Y��4byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer32Array) {
			auto ib = new IndexBuffer;
			ib->Init(static_cast<int>(tkIb.indices.size()) * 4, 4);
			ib->Copy((uint32_t*)&tkIb.indices.at(0));

			//�X�L�������邩�ǂ�����ݒ肷��B
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	//3. �}�e���A�����쐬�B
	mesh->m_materials.reserve(tkmMesh.materials.size());
	for (auto& tkmMat : tkmMesh.materials) {
		auto mat = new Material;
		mat->InitFromTkmMaterila(
			tkmMat,
			fxFilePath,
			vsEntryPointFunc,
			vsSkinEntryPointFunc,
			psEntryPointFunc,
			colorBufferFormat,
			NUM_SRV_ONE_MATERIAL,
			NUM_CBV_ONE_MATERIAL,
			NUM_CBV_ONE_MATERIAL * materialNum,
			NUM_SRV_ONE_MATERIAL * materialNum,
			samplerFilter
		);
		//�쐬�����}�e���A�������J�E���g����B
		materialNum++;
		mesh->m_materials.push_back(mat);
	}

	m_meshs[meshNo] = mesh;
}
