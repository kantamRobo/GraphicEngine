#pragma once
#include <vector>
#include "RayTracingInstance.h"
namespace raytracing {
	/// <summary>
	/// BLASBuffer
	/// </summary>
	/// <remark>
	/// BLAS( Bottom level acceleration structures )とは
	/// レイトレワールドに登録されているジオメトリのデータ構造です。
	/// BLASに3Dモデルのポリゴン情報が登録されることになります。
	/// </remark
	class BLASBuffer
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="instance"></param>

		void InitBLASBuffer(std::shared_ptr<RenderContext> rc, const std::vector<InstancePtr>& instances);


		/// <summary>
			/// BLASBufferのリストを取得。
			/// </summary>
			/// <returns></returns>
		const std::vector< AccelerationStructureBuffers>& Get() const
		{
			return m_bottomLevelASBuffers;
		}
	private:

		std::vector< AccelerationStructureBuffers> m_bottomLevelASBuffers;	//BLASBuffer
	};

}