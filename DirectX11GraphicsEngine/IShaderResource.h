#pragma once

	class IShaderResource
	{

	public:
		virtual ~IShaderResource()
		{
		}
		/// <summary>
		/// SRV�ɓo�^�B
		/// </summary>
		/// <param name="descriptorHandle"></param>
		virtual void RegistShaderResourceView( int bufferNo) = 0;
	};


