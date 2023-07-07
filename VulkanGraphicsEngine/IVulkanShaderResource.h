
class IVulkanShaderResource {
public:
	virtual ~IVulkanShaderResource()
	{
	}
	/// <summary>
	/// SRVÇ…ìoò^ÅB
	/// </summary>
	/// <param name="descriptorHandle"></param>
	virtual void RegistShaderResourceView( int bufferNo) = 0;

};