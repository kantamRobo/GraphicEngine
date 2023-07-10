#pragma once
#include "stdafx.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "PipelineState.h"
class ConstantBuffer;
class Texture;
class DescriptorHeap;
enum { MAX_DESCRIPTOR_HEAP = 4 };	//�f�B�X�N���v�^�q�[�v�̍ő吔�B
enum { MAX_CONSTANT_BUFFER = 8 };	//�萔�o�b�t�@�̍ő吔�B����Ȃ��Ȃ����瑝�₵�ĂˁB
enum { MAX_SHADER_RESOURCE = 16 };	//�V�F�[�_�[���\�[�X�̍ő吔�B����Ȃ��Ȃ����瑝�₵�ĂˁB

class RenderContext
{
public:

	/// <summary>
	/// ���\�[�X���R�s�[�B
	/// </summary>
	/// <param name="pDst">�R�s�[��̃��\�[�X</param>
	/// <param name="pSrc">�R�s�[���̃��\�[�X</param>
	void CopyResource(ID3D12Resource* pDst, ID3D12Resource* pSrc)
	{
		m_commandList->CopyResource(pDst, pSrc);
	}
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

		
		m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps->GetAddressOf());

	}
	inline void SetDescriptorHeap(DescriptorHeap& descHeap)
	{
		m_descriptorHeaps[0] = descHeap.get();
		m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps->GetAddressOf());

		//�f�B�X�N���v�^�e�[�u���ɓo�^����B
		if (descHeap.IsRegistConstantBuffer()) {
			SetGraphicsRootDescriptorTable(0, descHeap.GetConstantBufferGpuDescriptorStartHandle());
		}
		if (descHeap.IsRegistShaderResource()) {
			SetGraphicsRootDescriptorTable(1, descHeap.GetShaderResourceGpuDescriptorStartHandle());
		}
		if (descHeap.IsRegistUavResource()) {
			SetGraphicsRootDescriptorTable(2, descHeap.GetUavResourceGpuDescriptorStartHandle());
		}
	}

		void SetDescriptorHeap(ComPtr<DescriptorHeap> descHeap);
		void SetComputeDescriptorHeap(ComPtr<DescriptorHeap> descHeap);

		void SetRenderTargetAndViewport(std::shared_ptr<RenderTarget> renderTarget);

		void SetDescriptorHeaps(int numDescriptorHeap, const ComPtr<DescriptorHeap> descHeaps[])
		{
			for (int i = 0; i < numDescriptorHeap; i++)
			{
				m_descriptorHeaps[i] = descHeaps[i]->get();

			}
			m_commandList->SetDescriptorHeaps(numDescriptorHeap
				, m_descriptorHeaps->GetAddressOf());
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
		/// <param name="renderTarget"></param>
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
		{
			m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		}



		/// <summary>
		/// �����_�����O�^�[�Q�b�g���X���b�g0�ɐݒ肷��B
		/// </summary>
		/// <remarks>
		/// �{�֐��̓r���[�|�[�g�̐ݒ���s���܂���B
		/// ���[�U�[���œK�؂ȃr���[�|�[�g���w�肷��K�v������܂��B
		/// </remarks>
		/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
		void SetRenderTarget(std::shared_ptr<RenderTarget> Rendertarget)
		{
			std::array<RenderTarget*, 1>rtArray[] = { Rendertarget.get() };
			SetRenderTargets(1, rtArray->data());
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
		void SetRenderTargetAndViewport(RenderTarget & renderTarget);
		/// <summary>
		/// �������̃����_�����O�^�[�Q�b�g�ƃr���[�|�[�g�𓯎��ɐݒ肷��B
		/// </summary>
		/// /// <remarks>
		/// ���̊֐��𗘗p����ƃ����_�����O�^�[�Q�b�g�Ɠ������ƍ����̃r���[�|�[�g���ݒ肳��܂��B
		/// </remarks>
		/// <param name="numRT">�ݒ肷�郌���_�����O�^�[�Q�b�g�̖���</param>
		/// <param name="renderTargets">�����_�����O�^�[�Q�b�g�̔z��B</param>
		void SetRenderTargetsAndViewport(UINT numRT, RenderTarget * renderTargets[]);

		/// <summary>
	/// �������̃����_�����O�^�[�Q�b�g���N���A�B
	/// </summary>
	/// <remarks>
	/// �N���A�J���[�̓����_�����O�^�[�Q�b�g�̏��������Ɏw�肵���J���[�ł��B
	/// </remarks>
	/// <param name="numRt">�����_�����O�^�[�Q�b�g�̐�</param>
	/// <param name="renderTargets">�����_�����O�^�[�Q�b�g�̐�</param>
		void ClearRenderTargetViews(
			int numRt,
			RenderTarget* renderTargets[]
		);
		void SetPipelineState(PSO& pso);
		void SetComputeDescriptorHeap(std::shared_ptr<DescriptorHeap> descHeap);
		void WaitUntilFinishDrawingToRenderTarget(std::shared_ptr<RenderTarget> renderTarget);
		void WaitUntilToPossibleSetRenderTarget(std::shared_ptr<RenderTarget> renderTarget);
		void WaitUntilToPossibleSetRenderTargets(int numRt, std::shared_ptr<RenderTarget> renderTargets[]);
		void WaitUntilToPossibleSetRenderTargets(int numRt, RenderTarget* renderTargets[]);
		
		void WaitUntilFinishDrawingToRenderTargets(int numRt, std::shared_ptr<RenderTarget> renderTargets[]);
		/// <summary>
	/// �������̃����_�����O�^�[�Q�b�g��ݒ肷��B
	/// </summary>
	/// <remarks>
	/// MRT�𗘗p���������_�����O���s�������ꍇ�ɗ��p���Ă��������B
	/// </remarks>
	/// <param name="numRT">�����_�����O�^�[�Q�b�g�̐�</param>
	/// <param name="renderTarget">�����_�����O�^�[�Q�b�g�̔z��B</param>
		void SetRenderTargets(UINT numRT, RenderTarget * renderTargets[]);


		void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);
		void WaitUntilToPossibleSetRenderTarget(ComPtr<RenderTarget> renderTarget);
		void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);

		
		void ResourceBarrier(D3D12_RESOURCE_BARRIER & barrier)
		{
			m_commandList->ResourceBarrier(1, &barrier);
		}
		/// <summary>
		/// ���\�[�X�X�e�[�g��J�ڂ���B
		/// </summary>
		/// <param name="resrouce"></param>
		/// <param name="beforeState"></param>
		/// <param name="afterState"></param>
		void TransitionResourceState(ID3D12Resource * resrouce, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
		{
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resrouce, beforeState, afterState);
			m_commandList->ResourceBarrier(1, &barrier);
		}
		/// <summary>
		/// �R�}���h���X�g�����
		/// </summary>
		void Close()
		{
			m_commandList->Close();
		}

		/// <summary>
		/// �C���f�b�N�X���v���~�e�B�u��`��B
		/// </summary>
		/// <param name="indexCount">�C���f�b�N�X�̐��B</param>
		void DrawIndexed(UINT indexCount)
		{
			m_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		}
		/// <summary>
		/// �C���X�^���V���O�`��
		/// </summary>
		/// <param name="indexCount">�C���f�b�N�X��</param>
		/// <param name="numInstance">�C���X�^���X��</param>
		void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
		{
			m_commandList->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
		}
		/// <summary>
		/// �R���s���[�g�V�F�[�_�[���f�B�X�p�b�`�B
		/// </summary>
		/// <param name="ThreadGroupCountX"></param>
		/// <param name="ThreadGroupCountY"></param>
		/// <param name="ThreadGroupCountZ"></param>
		void Dispatch(
			UINT ThreadGroupCountX,
			UINT ThreadGroupCountY,
			UINT ThreadGroupCountZ)
		{
			m_commandList->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
		}




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
	/// �����_�����O�^�[�Q�b�g�̃N���A�B
	/// </summary>
	/// <param name="rtvHandle">CPU�̃����_�����O�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�n���h��</param>
	/// <param name="clearColor">�N���A�J���[</param>
		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
		{
			m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}

		/// <summary>
	/// �f�v�X�X�e���V���r���[���N���A
	/// </summary>
	/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
	/// <param name="clearValue">�N���A�l</param>
		void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
		{
			m_commandList->ClearDepthStencilView(
				dsvHandle,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				clearValue,
				0,
				0,
				nullptr);
		}

		/// <summary>
	/// �R�}���h���X�g�����
	/// </summary>
		void Close()
		{
			m_commandList->Close();
		}

		/// <summary>
/// �R�}���h���X�g�����Z�b�g�B
/// </summary>
/// <param name="commandAllocator"></param>
/// <param name="pipelineState"></param>
		void Reset(ID3D12CommandAllocator* commandAllocator, ID3D12PipelineState* pipelineState)
		{
			m_commandList->Reset(commandAllocator, pipelineState);

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

		/// <summary>
	/// �C���f�b�N�X���v���~�e�B�u��`��B
	/// </summary>
	/// <param name="indexCount">�C���f�b�N�X�̐��B</param>
		void DrawIndexed(UINT indexCount)
		{
			m_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		}

		/// <summary>
	/// �C���X�^���V���O�`��
	/// </summary>
	/// <param name="indexCount">�C���f�b�N�X��</param>
	/// <param name="numInstance">�C���X�^���X��</param>
		void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
		{
			m_commandList->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
		}

		/// <summary>
	/// �R���s���[�g�V�F�[�_�[���f�B�X�p�b�`�B
	/// </summary>
	/// <param name="ThreadGroupCountX"></param>
	/// <param name="ThreadGroupCountY"></param>
	/// <param name="ThreadGroupCountZ"></param>
		void Dispatch(
			UINT ThreadGroupCountX,
			UINT ThreadGroupCountY,
			UINT ThreadGroupCountZ)
		{
			m_commandList->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
		}

		/// <summary>
	/// GPU�Ń��C�g���[�V���O�A�N�Z�����[�V�����\���̃r���h���s���܂��B
	/// </summary>
		void BuildRaytracingAccelerationStructure(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc)
		{
			m_commandList->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
		}
		/// <summary>
		/// ���C���f�B�X�p�b�`
		/// </summary>
		/// <param name="rayDesc"></param>
		void DispatchRays(D3D12_DISPATCH_RAYS_DESC& rayDesc)
		{
			m_commandList->DispatchRays(&rayDesc);
		}
		/// <summary>
		/// ���\�[�X���R�s�[�B
		/// </summary>
		/// <param name="pDst">�R�s�[��̃��\�[�X</param>
		/// <param name="pSrc">�R�s�[���̃��\�[�X</param>
		void CopyResource(ID3D12Resource* pDst, ID3D12Resource* pSrc)
		{
			m_commandList->CopyResource(pDst, pSrc);
		}
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

		private:
		ComPtr<ID3D12GraphicsCommandList4> m_commandList;
		ComPtr<ID3D12DescriptorHeap> m_descriptorHeaps[MAX_DESCRIPTOR_HEAP];
		std::shared_ptr<ConstantBuffer>m_constantBuffers[MAX_CONSTANT_BUFFER];
		std::shared_ptr<Texture> m_shaderResources[MAX_SHADER_RESOURCE];
		std::vector < ComPtr<ID3D12Resource>> m_scratchResourceList;
		D3D12_VIEWPORT m_currentViewport;
	
};
