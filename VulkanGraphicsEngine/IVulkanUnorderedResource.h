#pragma once
#include "IVulkanShaderResource.h"
class IVulkanUnorderdResrouce : public IVulkanShaderResource {
public:
	virtual ~IVulkanUnorderdResrouce() {};
	/// <summary>
	/// UAV�ɓo�^�B
	/// </summary>
	/// <param name=""></param>
	/// <param name="bufferNo"></param>
	virtual void RegistUnorderAccessView(int bufferNo) = 0;
};