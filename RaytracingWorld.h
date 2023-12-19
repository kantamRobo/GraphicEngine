#pragma once
class TLASBuffer;
class BLASBuffer;
class Model;
class RenderContext;
#include "Matrix.h"
#include "Vector.h"
#include "TLASBuffer.h"
#include "BLASBuffer.h"
#include <functional>
#include <memory>


namespace raytracing {
	
	class World
	{
		
		//���C�g�����[���h
	public:
		//�W�I���g����o�^
	
		void RegistGeometry(Model& model);

		

		//�W�I���g���̓o�^���m��
		void CommitRegistGeometry(RenderContext& rc);
		/// <summary>
		/// ���C�g�����[���h�̃C���X�^���X�ɑ΂��ăN�G�����s���B
		/// </summary>
		/// <param name="queryFunc"></param>
		void QueryInstances(std::function<void(Instance&)> queryFunc) const
		{
			for (auto& instance : m_instances) {
				queryFunc(*instance);
			}
		}

		//�C���X�^���X�̐����擾
		int GetNumInstance()const
		{
			return static_cast<int>(m_instances.size());
		}

		//BLAS�o�b�t�@�[���擾
		const BLASBuffer& GetBLASBuffer()
		{

			return m_blasBuffer;
		}

		//TLAS�o�b�t�@�[���擾�B
        const TLASBuffer& GetTLASBuffer()const
		{
			return m_topLevelASBuffers;
		}

	private:

		//�J����

		struct Camera {
			EngineMath::Matrix mRot;//��]�s��
			EngineMath::Vector3 pos;//���_
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

