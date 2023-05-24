#pragma once

	class IShaderResource
	{

	public:
		virtual ~IShaderResource()
		{
		}
		/// <summary>
		/// SRVÇ…ìoò^ÅB
		/// </summary>
		/// <param name="descriptorHandle"></param>
		virtual void RegistShaderResourceView( int bufferNo) = 0;
	};


