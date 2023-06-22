#pragma once

class IVulkanShaderResource {
public:
	virtual ~IVulkanShaderResource()
	{

	}

	//IMV‚É“o˜^
	virtual void RegistImageView(int bufferNo) = 0;


};