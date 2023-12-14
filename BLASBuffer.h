#pragma once
#include <vector>
#include "RayTracingInstance.h"
class BLASBuffer
{
public:
	//初期化

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

