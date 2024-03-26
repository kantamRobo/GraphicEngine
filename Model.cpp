
#include <array>

#include "MiniEngine.h"




#include "MyAssert.h"



#include "Material.h"
#include "Camera.h"
#include "TkmFIle.h"

#include "Model.h"





void Model::InitModel(const ModelInitData& initData)
{
	/* ���C�g�������̏������̎��ɂ�m_fxFilePath�͎w�肳��Ă��Ȃ��ꍇ������̂ŃX���[����B
	MY_ASSERT(
		initData.m_fxFilePath,
		"error : initData.m_fxFilePath���w�肳��Ă��܂���B"
	);
	*/
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePath���w�肳��Ă��܂���B"
	);

	if (initData.m_skeleton != nullptr) {
		//�X�P���g�����w�肳��Ă���B
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}


	auto tkmFile = g_engine->GetTkmFileFromBank(initData.m_tkmFilePath);
	if (tkmFile == nullptr) {
		//���o�^
		tkmFile = new TkmFile;
		tkmFile->Load(initData.m_tkmFilePath);
		g_engine->RegistTkmFileToBank(initData.m_tkmFilePath, tkmFile);
	}
	m_tkmFile = tkmFile;
	m_meshParts.InitFromTkmFile(
		*m_tkmFile,
		initData.m_fxFilePath,
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView,
		initData.m_colorBufferFormat,
		initData.m_samplerFilter
	);

	UpdateWorldMatrix(EngineMath::g_vec3Zero, EngineMath::g_quatIdentity, EngineMath::g_vec3One);

	m_isInited = true;
}

void Model::UpdateWorldMatrix(EngineMath::Vector3 pos, EngineMath::Quaternion rot, EngineMath::Vector3 scale)
{
	m_world = CalcWorldMatrix(pos, rot,scale);
}

void Model::ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
{
	m_meshParts.QueryMeshs([&](const SMesh& mesh) {
		//todo �}�e���A������tkm�t�@�C���ɏo�͂��ĂȂ������B
		//todo ���͑S�}�e���A�������ւ��܂�
		for (auto& material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
		});
	//�f�B�X�N���v�^�q�[�v�̍č쐬�B
	m_meshParts.CreateDescriptorHeaps();

}
void Model::Draw(RenderContext& rc)
{
	m_meshParts.Draw(
		rc,
		m_world,
		g_camera3D->GetViewMatrix(),
		g_camera3D->GetProjectionMatrix()
	);
}
void Model::Draw(RenderContext& rc, Camera& camera)
{
	Draw(rc, camera.GetViewMatrix(), camera.GetProjectionMatrix());
}

void Model::Draw(RenderContext& rc, const EngineMath::Matrix& viewMatrix, const EngineMath::Matrix& projMatrix)
{
	m_meshParts.Draw(
		rc,
		m_world,
		viewMatrix,
		projMatrix
	);
}

void Model::DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance)
{
	// �C���X�^���X�̐���0�ȏ�Ȃ�`��B
	m_meshParts.DrawInstancing(
		rc,
		numInstance,
		g_camera3D->GetViewMatrix(),
		g_camera3D->GetProjectionMatrix()
	);
}
