#pragma once

class IVulkanShaderResource {
public:
	virtual ~IVulkanShaderResource()
	{

	}

	//IMV�ɓo�^
	virtual void RegistImageView(int bufferNo) = 0;


};