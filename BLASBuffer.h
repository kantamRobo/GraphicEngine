#pragma once
#include <vector>
#include "RayTracingInstance.h"
class BLASBuffer
{
public:
	//������

	void InitBLASBuffer(std::shared_ptr<RenderContext> rc, const std::vector<InstancePtr>& instances);
	

	/// <summary>
		/// BLASBuffer�̃��X�g���擾�B
		/// </summary>
		/// <returns></returns>
	const std::vector< AccelerationStructureBuffers>& Get() const
	{
		return m_bottomLevelASBuffers;
	}
private:

	std::vector< AccelerationStructureBuffers> m_bottomLevelASBuffers;	//BLASBuffer
};

