#pragma once
class Model;
class RenderContext;
#include <functional>
#include <memory>
namespace raytracing {
	class World
	{

		//レイトレワールド
	public:
		//ジオメトリを登録
	
		void RegistGeometry(Model& model);

		

		//ジオメトリの登録を確定
		void CommitRegistGeometry(RenderContext& rc);

		//レイトレワールドのインスタンスに対してクエリを行う

		void QueryInstances(std::function<void(Instance&)> queryfunc)const
		{
			for (auto& instance : m_instances)
			{
				queryFunc(*instance);
			}
		}

		//インスタンスの数を取得
		int GetNumInstance()const
		{
			return static_cast<int>(m_instances.size());
		}

		//BLASバッファーを取得
		const std::shared_ptr<BLASBuffer> GetBLASBuffer()
		{

			return m_blasBuffer;
		}

		//TLASバッファーを取得。

		const std::shared_ptr<TLASBuffer> GetTLASBuffer()const
		{
			return m_topLevelASBuffers;
		}

	private:

		//カメラ

		struct Camera {
			EngineMath::Matrix mRot;//回転行列
			EngineMath::Vector3 pos;//視点
			float aspect;//アスペクト比
			float fFar;//遠平面
			float fNear;//近平面

		};

		Camera m_camera;//レイトレカメラ
		std::vector<InstancePtr> m_instances;//レイトレワールドのインスタンの配列。
		BLASBuffer m_blasBuffer;//BLAS
		TLASBuffer m_topLevelASBuffers;//TLAS
	};

}

