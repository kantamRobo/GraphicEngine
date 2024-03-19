#include "DX11Shader.h"

void DX11Shader::InitVertexShader(ID3D11Device* device,const std::wstring& shaderfile)
{
	auto result = D3DCompileFromFile(shaderfile.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_compiledVS, nullptr);
	
}

void DX11Shader::InitPixelShader(ID3D11Device* device,const std::wstring& shaderfile)
{

	auto result = D3DCompileFromFile(shaderfile.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_compiledPS, nullptr);

	device->CreatePixelShader(m_compiledPS->GetBufferPointer(), m_compiledPS->GetBufferSize(), nullptr,m_PS.GetAddressOf());
}

void DX11Shader::InitLayout(ID3D11Device* device,const std::vector <D3D11_INPUT_ELEMENT_DESC>& layout)
{

	device->CreateInputLayout(layout.data(), 1, m_compiledVS->GetBufferPointer(), m_compiledVS->GetBufferSize(), m_InputLayout.GetAddressOf());
}
