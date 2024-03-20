#include "DX11Shader.h"
//https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/how-to--compile-a-shader
void DX11Shader::InitVertexShader(ID3D11Device* device,const std::wstring& shaderfile)
{
	Microsoft::WRL::ComPtr<ID3DBlob> errorcode = nullptr;
	UINT compileFlag = 0;
	compileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};
	auto result = D3DCompileFromFile(shaderfile.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"vs_main", "vs_5_0", 
		compileFlag, 0,m_compiledVS.GetAddressOf(), errorcode.GetAddressOf());
	if (FAILED(result))
	{
		OutputDebugStringA((char*)errorcode->GetBufferPointer());
		errorcode->Release();
	}
	device->CreateVertexShader(m_compiledVS->GetBufferPointer(), m_compiledVS->GetBufferSize(), nullptr, m_VS.GetAddressOf());
}

void DX11Shader::InitPixelShader(ID3D11Device* device,const std::wstring& shaderfile)
{
	Microsoft::WRL::ComPtr<ID3DBlob> errorcode = nullptr;
	UINT compileFlag = 0;
	compileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};
	auto result = D3DCompileFromFile(shaderfile.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_5_0", 
		compileFlag, 0, m_compiledPS.GetAddressOf(), errorcode.GetAddressOf());

	if (FAILED(result))
	{
		OutputDebugStringA((char*)errorcode->GetBufferPointer());
		errorcode->Release();
	}

	device->CreatePixelShader(m_compiledPS->GetBufferPointer(), m_compiledPS->GetBufferSize(), nullptr,m_PS.GetAddressOf());
}

void DX11Shader::InitLayout(ID3D11Device* device,const std::vector <D3D11_INPUT_ELEMENT_DESC>& layout)
{

	device->CreateInputLayout(layout.data(), 1, m_compiledVS->GetBufferPointer(), m_compiledVS->GetBufferSize(), m_InputLayout.GetAddressOf());
}
