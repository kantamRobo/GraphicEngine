#include "MeshParts.h"
#include "stdafx.h"
#include "Skeleton.h"
#include "Material.h"
#include "IndexBuffer.h"
#include "Matrix.h"
#include "Vector.h"
#include "DescriptorHeap.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <memory>
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

void MeshParts::InitFromAssimpFile(const aiScene* scene,const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, void* expandData, int expandDataSize, const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter)
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

void MeshParts::Draw(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj)
{
}

void MeshParts::DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance, const Matrix& mView, const Matrix& mProj)
{
	//�萔�o�b�t�@�̐ݒ�A�X�V�ȂǕ`��̋��ʏ��������s����B
	DrawCommon(rc, g_matIdentity, mView, mProj);

	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		//1. ���_�o�b�t�@��ݒ�B
		rc.SetVertexBuffer(mesh->m_vertexBuffer);
		//�}�e���A�����ƂɃh���[�B
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			//���̃}�e���A�����\���Ă��郁�b�V���̕`��J�n�B
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);
			//2. �f�B�X�N���v�^�q�[�v��ݒ�B
			rc.SetDescriptorHeap(m_descriptorHeap);
			//3. �C���f�b�N�X�o�b�t�@��ݒ�B
			auto& ib = mesh->m_indexBufferArray[matNo];
			rc.SetIndexBuffer(*ib);

			//4. �h���[�R�[�������s�B
			rc.DrawIndexedInstanced(ib->GetCount(), numInstance);
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
			m_descriptorHeap.RegistShaderResource(srvNo, mesh->m_materials[matNo]->GetAlbedoMap());			//�A���x�h�}�b�v�B
			m_descriptorHeap.RegistShaderResource(srvNo + 1, mesh->m_materials[matNo]->GetNormalMap());		//�@���}�b�v�B
			m_descriptorHeap.RegistShaderResource(srvNo + 2, mesh->m_materials[matNo]->GetSpecularMap());		//�X�y�L�����}�b�v�B
			m_descriptorHeap.RegistShaderResource(srvNo + 3, m_boneMatricesStructureBuffer);					//�{�[���̃X�g���N�`���[�h�o�b�t�@�B
			for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
				if (m_expandShaderResourceView[i]) {
					m_descriptorHeap.RegistShaderResource(srvNo + EXPAND_SRV_REG__START_NO + i, *m_expandShaderResourceView[i]);
				}
			}
			srvNo += NUM_SRV_ONE_MATERIAL;
			m_descriptorHeap.RegistConstantBuffer(cbNo, m_commonConstantBuffer);
			if (m_expandConstantBuffer.IsValid()) {
				m_descriptorHeap.RegistConstantBuffer(cbNo + 1, m_expandConstantBuffer);
			}
			cbNo += NUM_CBV_ONE_MATERIAL;
		}
	}
	m_descriptorHeap.Commit();
}