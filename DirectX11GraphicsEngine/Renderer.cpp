#include "Vertex.h"
#include "SceneConstant.h"
#include "Renderer.h"


std::vector<UINT> g_IndexList{
		0,  1,  2,     3,  2,  1,
		4,  5,  6,     7,  6,  5,
		8,  9, 10,    11, 10,  9,
	   12, 13, 14,    15, 14, 13,
	   16, 17, 18,    19, 18, 17,
	   20, 21, 22,    23, 22, 21,
};
//const Microsoft::glTF::Document& doc, const std::string& attrName
void Renderer::Init(HWND hwnd)
{

	std::vector<Vertex> g_VertexList{
	{ { -0.5f,  0.5f, -0.5f },  },
	{ {  0.5f,  0.5f, -0.5f },  },
	{ { -0.5f, -0.5f, -0.5f },},
	{ {  0.5f, -0.5f, -0.5f }, },

	{ { -0.5f,  0.5f,  0.5f },  },
	{ { -0.5f, -0.5f,  0.5f }, },
	{ {  0.5f,  0.5f,  0.5f },  },
	{ {  0.5f, -0.5f,  0.5f },  },

	{ { -0.5f,  0.5f,  0.5f },  },
	{ { -0.5f,  0.5f, -0.5f },  },
	{ { -0.5f, -0.5f,  0.5f }, },
	{ { -0.5f, -0.5f, -0.5f }, },

	{ {  0.5f,  0.5f,  0.5f },  },
	{ {  0.5f, -0.5f,  0.5f },  },
	{ {  0.5f,  0.5f, -0.5f }, },
	{ {  0.5f, -0.5f, -0.5f } },

	{ { -0.5f,  0.5f,  0.5f },  },
	{ {  0.5f,  0.5f,  0.5f },  },
	{ { -0.5f,  0.5f, -0.5f },  },
	{ {  0.5f,  0.5f, -0.5f },  },

	{ { -0.5f, -0.5f,  0.5f },  },
	{ { -0.5f, -0.5f, -0.5f },  },
	{ {  0.5f, -0.5f,  0.5f },  },
	{ {  0.5f, -0.5f, -0.5f }, },
	};


	
	graphicEngine.CreateDevice();
	graphicEngine.CreateSwapChain(hwnd);
	graphicEngine.CreateRTV();


	model = std::make_shared<DX11Model>(graphicEngine.m_device.Get());

	model->m_mesh->m_vertexbuffer->InitVertexBuffer(graphicEngine.m_device.Get(), g_VertexList);
	model->m_mesh->m_indexbuffer->InitIndexbuffer(graphicEngine.m_device.Get(),g_IndexList, g_IndexList.size()*sizeof(UINT));
	
	context = std::make_shared<DX11RenderContext>(graphicEngine.m_deviceContext);
	rasterizerState = std::make_shared<DX11RasterizerState>();
	rasterizerState->InitRasterizerState(&graphicEngine, graphicEngine.m_deviceContext.Get(),&graphicEngine.m_viewport);
	temp_shader = std::make_shared<DX11Shader>();
	temp_shader->InitVertexShader(graphicEngine.m_device.Get(), L"VertexShader.hlsl");
	temp_shader->InitPixelShader(graphicEngine.m_device.Get(), L"PixelShader.hlsl");
	


	std::vector<D3D11_INPUT_ELEMENT_DESC> templayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		 { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	 };
	temp_shader->InitLayout(graphicEngine.m_device.Get(), templayout);
	//model.Init(graphicEngine.m_device.Get(), doc, attrName);


	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(2.0f, 2.0f, -2.0f, 0.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);

	float    fov = DirectX::XMConvertToRadians(45.0f);
	float    aspect = graphicEngine.m_viewport.Width / graphicEngine.m_viewport.Height;
	float    nearZ = 0.1f;
	float    farZ = 100.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);


	m_constbuffer.InitConstantBuffer(&graphicEngine, sizeof(SceneConstant), &temp_const);


	

	

}

void Renderer::Tick()
{
	
	context->ClearRenderTargetView(graphicEngine.m_backBufferView.Get(),graphicEngine.m_DepthstencilView.Get());
	context->SetSingleRenderTarget(graphicEngine.m_backBufferView.Get(), graphicEngine.m_DepthstencilView.Get());

	context->SetVertexBuffer(model->m_mesh->m_vertexbuffer->m_vertexbuffer.Get());
	context->SetIndexBuffer(model->m_mesh->m_indexbuffer->m_IndexBuffer.Get());
	context->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	context->SetRasterizerState(rasterizerState->m_rasterizerstate.Get());
	//シェーダーは暫定的なもの。
	context->SetInputLayout(temp_shader->m_InputLayout.Get());
	context->SetVertexShader(temp_shader->m_VS.Get(), 1);
	// Rotate the cube 1 degree per frame.

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(2.0f, 2.0f, -2.0f, 0.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);

	float    fov = DirectX::XMConvertToRadians(45.0f);
	float    aspect = graphicEngine.m_viewport.Width / graphicEngine.m_viewport.Height;
	float    nearZ = 0.1f;
	float    farZ = 100.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	temp_const.xmproj = XMMatrixTranspose(projMatrix);
	temp_const.xmview = XMMatrixTranspose(viewMatrix);
	temp_const.xmworld = XMMatrixTranspose(worldMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = graphicEngine.m_deviceContext->Map(
		m_constbuffer.m_constantBuffer.Get(), // マップするリソース
		0,                                    // サブリソースのインデックス
		D3D11_MAP_WRITE_DISCARD,              // マップのタイプ
		0,                                    // マップオプション
		&mappedResource);                     // マップされたリソース情報を受け取る構造体
	if (SUCCEEDED(hr)) {
		// 定数バッファにデータをコピー
		memcpy(mappedResource.pData, &temp_const, sizeof(temp_const));
		// リソースのマップを解除
		graphicEngine.m_deviceContext->Unmap(m_constbuffer.m_constantBuffer.Get(), 0);
	}


	

	
	
	context->SetVertexShader_SingleConstantBuffer(m_constbuffer.m_constantBuffer.Get());
	
	
	context->SetPixelShader(temp_shader->m_PS.Get(), 1);
	context->SetSingleShaderResource();(しおり)
	context->SetPSSingleSampler();///new（しおり)
	context->SetSingleViewPort(&graphicEngine.m_viewport);
	
	context->DrawIndexed(g_IndexList.size());

	graphicEngine.m_swapChain->Present(1, 0);
}
