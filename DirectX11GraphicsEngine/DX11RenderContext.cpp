#include "DX11RenderContext.h"
#include "Vertex.h"
//�Z�b�^

//���̓A�Z���u��
void DX11RenderContext::SetVertexBuffer(ID3D11Buffer* vertexbuffers)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_rendercontext->IASetVertexBuffers(0, 1, &vertexbuffers, &stride, &offset);//�b��I�ȏ��u

}

void DX11RenderContext::SetIndexBuffer(ID3D11Buffer* indexbuffer)
{
	m_rendercontext->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);//�b��I�ȏ��u
}

void DX11RenderContext::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology)
{
	m_rendercontext->IASetPrimitiveTopology(topology);
}

void DX11RenderContext::SetInputLayout(ID3D11InputLayout* layout)
{

	m_rendercontext->IASetInputLayout(layout);


}

//ATTENTION:�V�F�[�_�[�Ɋւ��ẮA�������Ɋւ��Ă�Shader�N���X���쐬���A�����ŃV�F�[�_�[�̍쐬���s���B�����ł̓O���t�B�b�N�X�p�C�v���C���𓮂����R���e�L�X�g�̈ꕔ
//�Ƃ��āA�V�F�[�_�[�̃Z�b�g�֐���������Ɏ������Ă���B
void DX11RenderContext::SetVertexShader(ID3D11VertexShader* vertexShader, UINT numclassInstances)
{
	m_rendercontext->VSSetShader(vertexShader, nullptr, 0);


}

void DX11RenderContext::SetVertexShader_SingleConstantBuffer(ID3D11Buffer* constantbuffer)
{
	m_rendercontext->VSSetConstantBuffers(0, 1, &constantbuffer);
}


void DX11RenderContext::SetComputeShader(ID3D11ComputeShader* computeShader, UINT numclassInstances)
{

	m_rendercontext->CSSetShader(computeShader, nullptr, numclassInstances);
}

void DX11RenderContext::SetHullShader(ID3D11HullShader* hullShader, UINT numclassInstances)
{
	m_rendercontext->HSSetShader(hullShader, nullptr, numclassInstances);
}

void DX11RenderContext::SetDomainShader(ID3D11DomainShader* DomainShader, UINT numclassInstances)
{
	m_rendercontext->DSSetShader(DomainShader, nullptr, numclassInstances);
}


//���X�^���C�U�X�e�[�W

void DX11RenderContext::SetRasterizerState(ID3D11RasterizerState* rasterizerstate)
{
	m_rendercontext->RSSetState(rasterizerstate);
}


void DX11RenderContext::ClearRenderTargetView(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	m_rendercontext->ClearRenderTargetView(
		rtv,
		midnightBlue
	);

	m_rendercontext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, // �[�x�ƃX�e���V���̗������N���A
		1.0f,                                    // �[�x�l���ő��
		0);
}

void DX11RenderContext::SetViewPorts(UINT numviewports, const D3D11_VIEWPORT* viewports)
{
	m_rendercontext->RSSetViewports(numviewports, viewports);



}

void  DX11RenderContext::SetSingleViewPort(const D3D11_VIEWPORT* viewport)
{

	m_rendercontext->RSSetViewports(1, viewport);

	//�G���[���������?(�����̃r���[�|�[�g�������Ă邼�Ƃ�)
}


void DX11RenderContext::SetScssciorRects(UINT numRects, D3D11_RECT* rects)
{
	m_rendercontext->RSSetScissorRects(numRects, rects);
}

//�s�N�Z���V�F�[�_�[�X�e�[�W

void DX11RenderContext::SetPixelShader(ID3D11PixelShader* pixelShader, UINT numclassInstances)
{
	m_rendercontext->PSSetShader(pixelShader, nullptr, 0);

}

//����
void DX11RenderContext::DrawIndexedInstanced(UINT indexcountperinstance, UINT instanceCount)
{
	m_rendercontext->DrawIndexedInstanced(indexcountperinstance, instanceCount, 0, 0, 0);
}
//Indirect���ĂȂ񂼂�
//https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC165%E5%9B%9E
//Indirect����
void DX11RenderContext::DrawIndexedInstancedIndirect(ID3D11Buffer* GPUPrimitiveBuffer, UINT AlingedByteOffset) {
	m_rendercontext->DrawIndexedInstancedIndirect(GPUPrimitiveBuffer, AlingedByteOffset);

}
void DX11RenderContext::DrawDispatchIndirect(ID3D11Buffer* GPUPrimitivebuffer, UINT Offset)
{
	m_rendercontext->DispatchIndirect(GPUPrimitivebuffer, Offset);

}




void DX11RenderContext::DrawIndexed(UINT indexCount)
{
	m_rendercontext->DrawIndexed(indexCount, 0, 0);

}



void DX11RenderContext::SetRenderTargets(UINT numviews, ID3D11RenderTargetView* rendertargetviews, ID3D11DepthStencilView* depthstencilviews)
{
	m_rendercontext->OMSetRenderTargets(numviews, &rendertargetviews, depthstencilviews);
}

void DX11RenderContext::SetBlendState(ID3D11BlendState* blenderstate)
{
	m_rendercontext->OMSetBlendState(blenderstate, nullptr, 0xffffffff);
}



void DX11RenderContext::SetSingleRenderTarget(ID3D11RenderTargetView* rendertargetviews, ID3D11DepthStencilView* depthstencilviews)
{
	m_rendercontext->OMSetRenderTargets(1, &rendertargetviews, depthstencilviews);
	//�G���[���������?(�����̃����_�[�^�[�Q�b�g�������Ă邼�Ƃ�)
}
//��ʃN���A
void DX11RenderContext::ClearRenderTargetView(ID3D11RenderTargetView* RenderTargetView, float nextcolor[4])
{
	m_rendercontext->ClearRenderTargetView(RenderTargetView, nextcolor);
}

//�X�g���[���o�̓}�[�W���[(�O���t�B�b�N�p�C�v���C������R���s���[�g�p�C�v���C���ɓ��͂���)
//https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/stream-output-counters
//��{�I�ɒ��_���o�b�t�@�[�ɏ������ދ@�\�Ɏg��
void DX11RenderContext::SetStates(UINT numbuffers, ID3D11Buffer* vertexbuffer, UINT offset)
{
	m_rendercontext->SOSetTargets(numbuffers, &vertexbuffer, &offset);
}

void DX11RenderContext::SetState(ID3D11Buffer* vertexbufferforCompute)
{
	m_rendercontext->SOSetTargets(1, &vertexbufferforCompute, 0);

	//�G���[���������?(�����̃o�b�t�@�������Ă邼�Ƃ�)
}




//�R���s���[�g�p�C�v���C��:������̓O���t�B�b�N�X�p�C�v���C���Ƃ͂Ȃ���͂��邪�A����̒��ɂ͑g�ݍ��܂�Ă��Ȃ��p�C�v���C�����w���B
void DX11RenderContext::Dispatch(UINT numthreadX, UINT numthreadY, UINT numthreadZ)
{

	m_rendercontext->Dispatch(numthreadX, numthreadY, numthreadZ);
}


//�Q�b�^