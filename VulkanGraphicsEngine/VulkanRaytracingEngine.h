#pragma once
class Model;

namespace raytracing {

	class Engine
	{

		void Dispatch(RenderingContext& rc);

		void RegistGeometry(Model& model) {

			m_world.RegistGeometry(model);
		}

		/// <summary>
		/// �W�I���g���̓o�^���m��B
		/// </summary>
		void CommitRegistGeometry(RenderContext& rc);

	private:
		/// <summary>
		/// �V�F�[�_�[�e�[�u�����쐬�B
		/// </summary>
		/// <param name="rc"></param>
		/// 
		void CreateShaderTable(RenderingContext& rc);

		/// <summary>
		/// �V�F�[�_�[���\�[�X���쐬�B
		/// </summary>
		void CreateShaderResources();

	private:

		//�J����
		struct Camera {
			Matrix mRot;
			Vector3 pos;
			float aspect;
			float fFar;
			float fNear;

		};
		UniformBuffer m_rayGenerationCB;
		World m_world;
		PSO  m_pipelineStateObject;
		ShaderTable m_shaderTable;
		DescriptorHeaps m_descriptorHeaps;
		GPUBuffer m_outputResource;
	};



}
