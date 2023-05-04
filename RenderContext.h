#pragma once
#include "stdafx.h"
#include "PipelineState.h"
#include "RootSignature.h"

class ConstantBuffer;
class Texture;
class DescriptorHeap;
class RenderTarget;
class RenderContext
{
public:
	void InitRenderingContext(ComPtr<ID3D12GraphicsCommandList4> commandList)
	{
		m_commandList = commandList;
	}
	/// <summary>
	/// ���_�o�b�t�@��ݒ�B
	/// </summary>
	/// <param name="vb">���_�o�b�t�@�B</param>
	void SetVertexBuffer(VertexBuffer& vb)
	{
		m_commandList->IASetVertexBuffers(0, 1, &vb.GetView());
	}
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@��ݒ�B
	/// </summary>
	/// <param name="ib"></param>
	void SetIndexBuffer(IndexBuffer& ib)
	{
		m_commandList->IASetIndexBuffer(&ib.GetView());
	}
	/// <summary>
	/// �v���~�e�B�u�̃g�|���W�[��ݒ�B
	/// </summary>
	/// <remarks>
	/// ID3D12GraphicsCommandList::��IASetPrimitiveTopology�̃��b�p�[�֐��B
	/// �ڍׂ�Microsoft�̃w���v���Q�ƁB
	/// </remarks>
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		m_commandList->IASetPrimitiveTopology(topology);
	}
	/// <summary>
	/// �R�}���h���X�g��ݒ�B
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g�B</param>
	void SetCommandList(ID3D12GraphicsCommandList4* commandList)
	{
		m_commandList = commandList;
	}
	/// <summary>
	/// �r���[�|�[�g�ƃV�U�����O��`���Z�b�g�Őݒ�
	/// </summary>
	/// <param name="viewport">�r���[�|�[�g</param>
	void SetViewportAndScissor(D3D12_VIEWPORT& viewport)
	{
		//�V�U�����O��`���ݒ肷��B
		D3D12_RECT scissorRect;
		scissorRect.bottom = static_cast<LONG>(viewport.Height);
		scissorRect.top = 0;
		scissorRect.left = 0;
		scissorRect.right = static_cast<LONG>(viewport.Width);
		SetScissorRect(scissorRect);

		m_commandList->RSSetViewports(1, &viewport);
		m_currentViewport = viewport;
	}
	/// <summary>
	/// �r���[�|�[�g���擾�B
	/// </summary>
	/// <returns></returns>
	D3D12_VIEWPORT GetViewport() const
	{
		return m_currentViewport;
	}
	/// <summary>
	/// �V�U�����O��`��ݒ�
	/// </summary>
	/// <param name="rect"></param>
	void SetScissorRect(D3D12_RECT& rect)
	{
		m_commandList->RSSetScissorRects(1, &rect);
	}

	/// <summary>
	/// ���[�g�V�O�l�`����ݒ�B
	/// </summary>
	void SetRootSignature(ID3D12RootSignature* rootSignature)
	{
		m_commandList->SetGraphicsRootSignature(rootSignature);
	}
	void SetRootSignature(std::shared_ptr<RootSignature> rootSignature)
	{
		m_commandList->SetGraphicsRootSignature(rootSignature->Get().Get());
	}
	void SetComputeRootSignature(ID3D12RootSignature* rootSignature)
	{
		m_commandList->SetComputeRootSignature(rootSignature);
	}
	void SetComputeRootSignature(std::shared_ptr<RootSignature> rootSignature)
	{
		m_commandList->SetComputeRootSignature(rootSignature->Get().Get());
	}
	/// <summary>
	/// �p�C�v���C���X�e�[�g��ݒ�B
	/// </summary>
	void SetPipelineState(ID3D12PipelineState* pipelineState)
	{
		m_commandList->SetPipelineState(pipelineState);
	}
	void SetPipelineState(std::shared_ptr<PipelineState> pipelineState)
	{
		m_commandList->SetPipelineState(pipelineState->Get().Get());
	}
	
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v��ݒ�B
	/// </summary>
	void SetDescriptorHeap(ID3D12DescriptorHeap* descHeap)
	{
		m_descriptorHeaps[0] = descHeap;
		m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps.data()->GetAddressOf());
	}

	void SetDescriptorHeap(ComPtr<DescriptorHeap> descHeap);
	void SetComputeDescriptorHeap(ComPtr<DescriptorHeap> descHeap);

	void SetDescriptorHeaps(int numDescriptorHeap, const ComPtr<DescriptorHeap> descHeaps[])
	{
		for (int i = 0; i < numDescriptorHeap; i++)
		{
			m_descriptorHeaps[i] = descHeaps[i];

		}
		m_commandList->SetDescriptorHeaps(numDescriptorHeap
			, m_descriptorHeaps.data()->GetAddressOf());
	}

	void SetConstantBuffer(int registerNo, std::shared_ptr<ConstantBuffer> cb)
	{
		if (registerNo < MAX_CONSTANT_BUFFER) {
			m_constantBuffers[registerNo] = cb;
		}
		else {
			//�͈͊O�A�N�Z�X�B
			std::abort();
		}
	}

	void SetShaderResource(int registerNo, std::shared_ptr<Texture> texture)
	{

	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�ƃr���[�|�[�g�𓯎��ɐݒ肷��B
	/// </summary>
	/// <remarks>
	/// ���̊֐��𗘗p����ƃ����_�����O�^�[�Q�b�g�Ɠ������ƍ����̃r���[�|�[�g���ݒ肳��܂��B
	/// </remarks>
	/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
	void SetRenderTargetAndViewport(RenderTarget& renderTarget);
	/// <summary>
	/// �������̃����_�����O�^�[�Q�b�g�ƃr���[�|�[�g�𓯎��ɐݒ肷��B
	/// </summary>
	/// /// <remarks>
	/// ���̊֐��𗘗p����ƃ����_�����O�^�[�Q�b�g�Ɠ������ƍ����̃r���[�|�[�g���ݒ肳��܂��B
	/// </remarks>
	/// <param name="numRT">�ݒ肷�郌���_�����O�^�[�Q�b�g�̖���</param>
	/// <param name="renderTargets">�����_�����O�^�[�Q�b�g�̔z��B</param>
	void SetRenderTargetsAndViewport(UINT numRT, RenderTarget* renderTargets[]);


	/// <summary>
/// �������̃����_�����O�^�[�Q�b�g��ݒ肷��B
/// </summary>
/// <remarks>
/// MRT�𗘗p���������_�����O���s�������ꍇ�ɗ��p���Ă��������B
/// </remarks>
/// <param name="numRT">�����_�����O�^�[�Q�b�g�̐�</param>
/// <param name="renderTarget">�����_�����O�^�[�Q�b�g�̔z��B</param>
	void SetRenderTargets(UINT numRT, RenderTarget* renderTargets[]);


	void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);
	void WaitUntilToPossibleSetRenderTarget(ComPtr<RenderTarget> renderTarget);
	void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);
private:

	/// <summary>
	/// �f�B�X�N���v�^�e�[�u����ݒ�B
	/// </summary>
	/// <param name="RootParameterIndex"></param>
	/// <param name="BaseDescriptor"></param>
	void SetGraphicsRootDescriptorTable(
		UINT RootParameterIndex,
		D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
	{
		m_commandList->SetGraphicsRootDescriptorTable(
			RootParameterIndex,
			BaseDescriptor
		);
	}
	/// <summary>
	/// �f�B�X�N���v�^�e�[�u����ݒ�B
	/// </summary>
	/// <param name="RootParameterIndex"></param>
	/// <param name="BaseDescriptor"></param>
	void SetComputeRootDescriptorTable(
		UINT RootParameterIndex,
		D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
	{
		m_commandList->SetComputeRootDescriptorTable(
			RootParameterIndex,
			BaseDescriptor
		);
	}

	

	enum { MAX_DESCRIPTOR_HEAP = 4 };
	enum { MAX_CONSTANT_BUFFER = 8 };
	enum { MAX_SHADER_RESOURCE = 16 };
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	std::array<ComPtr<ID3D12DescriptorHeap>, MAX_DESCRIPTOR_HEAP> m_descriptorHeaps;
	std::shared_ptr<ConstantBuffer>m_constantBuffers[MAX_CONSTANT_BUFFER];
	std::shared_ptr<Texture> m_shaderResources[MAX_SHADER_RESOURCE];

};

