#pragma once
class Model;
class RenderContext;
#include <functional>
namespace raytracing {
	class World
	{

		//���C�g�����[���h
	public:
		//�W�I���g����o�^
		void RegistGeometry(std::shared_ptr<Model> model);

		//�W�I���g���̓o�^���m��
		void CommitRegistGeometry(std::shared_ptr<RenderContext> rc);

		//���C�g�����[���h�̃C���X�^���X�ɑ΂��ăN�G�����s��

		void QueryInstances(std::function<void(Instance&)> queryfunc)const
		{
			for (auto& instance : m_instances)
			{
				queryFunc(*instance);
			}
		}

		//�C���X�^���X�̐����擾
		int GetNumInstance()const
		{
			return static_cast<int>(m_instances.size());
		}

		//BLAS�o�b�t�@�[���擾
		const std::shared_ptr<BLASBuffer> GetBLASBuffer()
		{

			return m_blasBuffer;
		}

		//TLAS�o�b�t�@�[���擾�B

		const std::shared_ptr<TLASBuffer> GetTLASBuffer()const
		{
			return m_topLevelASBuffers;
		}

	private:

		//�J����

		struct Camera {
			Matrix mRot;//��]�s��
			Vector3 pos;//���_
			float aspect;//�A�X�y�N�g��
			float fFar;//������
			float fNear;//�ߕ���

		};

		Camera m_camera;//���C�g���J����
		std::vector<InstancePtr> m_instances;//���C�g�����[���h�̃C���X�^���̔z��B
		BLASBuffer m_blasBuffer;//BLAS
		TLASBuffer m_topLevelASBuffers;//TLAS
	};

}
