#pragma once

	class I11ShaderResource
	{

	public:
		virtual ~I11ShaderResource()
		{
		}
		/// <summary>
		/// SRVÇ…ìoò^ÅB
		/// </summary>
		/// <param name="descriptorHandle"></param>
		virtual void RegistShaderResourceView( int bufferNo) = 0;
	};


