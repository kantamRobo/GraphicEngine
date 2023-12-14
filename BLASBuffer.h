#pragma once
#include <vector>
#include "RayTracingInstance.h"
class BLASBuffer
{
public:
	//‰Šú‰»

	void InitBLASBuffer(std::shared_ptr<RenderContext> rc, const std::vector<InstancePtr>& instances);
	

	/// <summary>
		/// BLASBuffer‚ÌƒŠƒXƒg‚ğæ“¾B
		/// </summary>
		/// <returns></returns>
	const std::vector< AccelerationStructureBuffers>& Get() const
	{
		return m_bottomLevelASBuffers;
	}
private:

	std::vector< AccelerationStructureBuffers> m_bottomLevelASBuffers;	//BLASBuffer
};

