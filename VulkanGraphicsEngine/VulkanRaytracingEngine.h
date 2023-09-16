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
		/// ジオメトリの登録を確定。
		/// </summary>
		void CommitRegistGeometry(RenderContext& rc);

	private:
		/// <summary>
		/// シェーダーテーブルを作成。
		/// </summary>
		/// <param name="rc"></param>
		/// 
		void CreateShaderTable(RenderingContext& rc);

		/// <summary>
		/// シェーダーリソースを作成。
		/// </summary>
		void CreateShaderResources();

	private:

		//カメラ
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
