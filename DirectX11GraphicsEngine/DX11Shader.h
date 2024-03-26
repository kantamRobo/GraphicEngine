#pragma once
#include <d3dcompiler.h>
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <string>
class DX11Shader
{
public:
 void InitVertexShader(ID3D11Device* device, const std::wstring& shaderfile);
 void InitPixelShader(ID3D11Device* device,const std::wstring& shaderfile);
 void InitLayout(ID3D11Device* device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout);
 
 Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_VS = nullptr;	// 頂点シェーダー
 Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS = nullptr;
 Microsoft::WRL::ComPtr<ID3DBlob> m_compiledVS;
 Microsoft::WRL::ComPtr<ID3DBlob> m_compiledPS;
 Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_InputLayout = nullptr;// 入力レイアウト
 // １頂点の詳細な情報
 std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
 };


