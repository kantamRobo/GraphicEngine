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


	
	graphicEngine.CreateDevice();
	graphicEngine.CreateSwapChain(hwnd);
	graphicEngine.CreateRTV();


	model = std::make_shared<DX11Model>(graphicEngine.m_device.Get(),"alicia-solid.vrm");

		
	context = std::make_shared<DX11RenderContext>(graphicEngine.m_deviceContext);
	rasterizerState = std::make_shared<DX11RasterizerState>();
	rasterizerState->InitRasterizerState(&graphicEngine, graphicEngine.m_deviceContext.Get(),&graphicEngine.m_viewport);
	temp_shader = std::make_shared<DX11Shader>();
	temp_shader->InitVertexShader(graphicEngine.m_device.Get(), L"VertexShader.hlsl");
	temp_shader->InitPixelShader(graphicEngine.m_device.Get(), L"PixelShader.hlsl");
	


	std::vector<D3D11_INPUT_ELEMENT_DESC> templayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		
	 };
	//�o�O�����炱��(new)(������)
	temp_shader->InitLayout(graphicEngine.m_device.Get(), templayout);
	


	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(2.0f, 2.0f, -2.0f, 0.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtRH(eye, focus, up);

	float    fov = DirectX::XMConvertToRadians(45.0f);
	float    aspect = graphicEngine.m_viewport.Width / graphicEngine.m_viewport.Height;
	float    nearZ = 0.1f;
	float    farZ = 100.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovRH(fov, aspect, nearZ, farZ);


	m_constbuffer.InitConstantBuffer(&graphicEngine, sizeof(SceneConstant), &temp_const);


	

	

}

void Renderer::Tick()
{
	
	context->ClearRenderTargetView(graphicEngine.m_backBufferView.Get(),graphicEngine.m_DepthstencilView.Get());
	context->SetSingleRenderTarget(graphicEngine.m_backBufferView.Get(), graphicEngine.m_DepthstencilView.Get());//������

	context->SetVertexBuffer(model->m_mesh->m_vertexbuffer->m_vertexbuffer.Get());
	context->SetIndexBuffer(model->m_mesh->m_indexbuffer->m_IndexBuffer.Get());
	context->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	context->SetRasterizerState(rasterizerState->m_rasterizerstate.Get());
	//�V�F�[�_�[�͎b��I�Ȃ��́B
	context->SetInputLayout(temp_shader->m_InputLayout.Get());
	context->SetVertexShader(temp_shader->m_VS.Get(), 1);
	// Rotate the cube 1 degree per frame.
	context->SetPixelShader(temp_shader->m_PS.Get(), 1);
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(2.0f, 2.0f, -2.0f, 0.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtRH(eye, focus, up);

	float    fov = DirectX::XMConvertToRadians(45.0f);
	float    aspect = graphicEngine.m_viewport.Width / graphicEngine.m_viewport.Height;
	float    nearZ = 0.1f;
	float    farZ = 100.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovRH(fov, aspect, nearZ, farZ);
	temp_const.xmproj = XMMatrixTranspose(projMatrix);
	temp_const.xmview = XMMatrixTranspose(viewMatrix);
	temp_const.xmworld = XMMatrixTranspose(worldMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = graphicEngine.m_deviceContext->Map(
		m_constbuffer.m_constantBuffer.Get(), // �}�b�v���郊�\�[�X
		0,                                    // �T�u���\�[�X�̃C���f�b�N�X
		D3D11_MAP_WRITE_DISCARD,              // �}�b�v�̃^�C�v
		0,                                    // �}�b�v�I�v�V����
		&mappedResource);     
	// �}�b�v���ꂽ���\�[�X�����󂯎��\����

	//TODO:���̓��I���\�[�X�̃}�b�s���O�E�A���}�b�s���O���֐���(������)
	if (SUCCEEDED(hr)) {
		// �萔�o�b�t�@�Ƀf�[�^���R�s�[
		memcpy(mappedResource.pData, &temp_const, sizeof(temp_const));
		// ���\�[�X�̃}�b�v������
		graphicEngine.m_deviceContext->Unmap(m_constbuffer.m_constantBuffer.Get(), 0);
	}


	

	
	
	context->SetVertexShader_SingleConstantBuffer(m_constbuffer.m_constantBuffer.Get());
	
	
	context->SetPixelShader(temp_shader->m_PS.Get(), 1);
	//context->SetSingleShaderResource();(������)
	//context->SetPSSingleSampler();///new�i������)
	context->SetSingleViewPort(&graphicEngine.m_viewport);
	
	context->DrawIndexed(model->m_mesh->indices.size());

	graphicEngine.m_swapChain->Present(1, 0);
}
