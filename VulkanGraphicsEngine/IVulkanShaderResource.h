
class IVulkanShaderResource {
public:
	virtual ~IVulkanShaderResource()
	{
	}
	/// <summary>
	/// SRV�ɓo�^�B
	/// </summary>
	/// <param name="descriptorHandle"></param>
	virtual void RegistShaderResourceView( int bufferNo) = 0;

};