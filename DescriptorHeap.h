#pragma once
#include "IUnorderAccessResrouce.h"
#include <vector>
#include "IUnorderAccessResrouce.h"


class DescriptorHeap
{
public:

	//�R���X�g���N�^
	DescriptorHeap();
	//�f�X�g���N�^
	~DescriptorHeap();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Get(); const

		/// <summary>
		/// �V�F�[�_�[���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^�B
		/// </summary>
		/// <param name="registerNo">
		/// ���W�X�^�ԍ��B-1���w�肳�ꂽ��A���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p�����B
		/// </param>
		/// <param name="sr">�V�F�[�_�[���\�[�X</param>
		void RegistShaderResource(int registerNo, std::shared_ptr<IShaderResource> sr)
	{
		RegistResource(
			registerNo,
			&sr,
			&m_shaderResources.front(),
			m_numShaderResource,
			MAX_SHADER_RESOURCE,
			L"DescriptorHeap::RegistShaderResource() ���W�X�^�ԍ����͈͊O�ł��B"
		);
	}
	
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�ւ̓o�^���m��B
	/// </summary>
	void Commit();
	/// <summary>
	/// �T���v���X�e�[�g�p�̃f�B�X�N���v�^�q�[�v�ւ̓o�^�B
	/// </summary>
	void CommitSamplerHeap();


	/// <summary>
	/// �A���I�[�_�[�A�N�Z�X���\�[�X��o�^�B
	/// </summary>
	/// <param name="registerNo">
	/// ���W�X�^�ԍ��B-1���w�肳�ꂽ��A���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p�����B
	/// </param>
	/// <param name="sr">�A���I�[�_�[���\�[�X</param>
	void RegistUnorderAccessResource(int registerNo, IUnorderAccessResrouce& sr)
	{
		RegistResource(
			registerNo,
			&sr,
			&m_uavResources.front(),
			m_numUavResource,
			MAX_SHADER_RESOURCE,
			L"DescriptorHeap::RegistUnorderAccessResource() ���W�X�^�ԍ����͈͊O�ł��B"
		);
	}
	/// <summary>
	/// �萔�o�b�t�@���f�B�X�N���v�^�q�[�v�ɓo�^�B
	/// </summary>
	/// <param name="registerNo">
	/// ���W�X�^�ԍ��B-1���w�肳�ꂽ��A���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p�����B
	/// </param>
	/// <param name="cb">�萔�o�b�t�@</param>
	void RegistConstantBuffer(int registerNo, ConstantBuffer& cb)
	{
		RegistResource(
			registerNo,
			&cb,
			&m_constantBuffers.front(),
			m_numConstantBuffer,
			MAX_CONSTANT_BUFFER,
			L"DescriptorHeap::RegistConstantBuffer() ���W�X�^�ԍ����͈͊O�ł��B"
		);
	}
	/// <summary>
	/// �T���v����`���f�B�X�N���v�^�q�[�v�ɒǉ��B
	/// </summary>
	/// <param name="registerNo">
	/// ���W�X�^�ԍ��B-1���w�肳�ꂽ��A���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p�����B
	/// </param>
	/// <param name="desc">
	/// �T���v����`
	/// </param>
	void RegistSamplerDesc(int registerNo, const D3D12_SAMPLER_DESC& desc)
	{
		RegistResource(
			registerNo,
			desc,
			m_samplerDescs,
			m_numSamplerDesc,
			MAX_SAMPLER_STATE,
			L"DescriptorHeap::RegistSamplerDesc() ���W�X�^�ԍ����͈͊O�ł��B"
		);
	}

	ID3D12DescriptorHeap* get()
	{
		return m_descriptorHeap->Get();
	}

	/// <summary>
	/// �V�F�[�_�[���\�[�X����ł��o�^����Ă��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsRegistShaderResource() const
	{
		return m_numShaderResource != 0;
	}
	public:
	/// <summary>
	/// �萔�o�b�t�@����ł��o�^����Ă��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsRegistConstantBuffer() const
	{
		return m_numConstantBuffer != 0;
	}
	/// <summary>
	/// UAV���\�[�X����ł��o�^����Ă��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsRegistUavResource() const
	{
		return m_numUavResource != 0;
	}

	inline D3D12_GPU_DESCRIPTOR_HANDLE GetConstantBufferGpuDescriptorStartHandle() const
	{
		auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
		return m_cbGpuDescriptorStart[backBufferIndex];
	}
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceGpuDescriptorStartHandle() const
	{
		auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
		return m_srGpuDescriptorStart[backBufferIndex];
	}
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetUavResourceGpuDescriptorStartHandle() const
	{
		auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
		return m_uavGpuDescriptorStart[backBufferIndex];
	}
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerResourceGpuDescriptorStartHandle() const
	{
		auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
		return m_samplerGpuDescriptorStart[backBufferIndex];
	}

	

	

	private:
		/// <summary>
	/// ���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^�B
	/// </summary>
	/// <param name="registerNo">�o�^�ԍ�</param>
	/// <param name="res">�o�^���郊�\�[�X</param>
	/// <param name="resTbl">���\�[�X�e�[�u���B���̃e�[�u���Ƀ��\�[�X���ǉ�����܂��B</param>
	/// <param name="numRes">�o�^����Ă��郊�\�[�X�̐��B�{�֐����Ăяo���ƁA���̐����P�C���N�������g����܂��B</param>
		template<class T>
		void RegistResource(
			int registerNo,
			T& res,
			std::shared_ptr<T> resTbl[],
			int& numRes,
			const int MAX_RESOURCE,
			const wchar_t* errorMessage
		)
		{
			if (registerNo == -1) {
				//-1���w�肳��Ă�����A���ݓo�^����Ă��閖���̃��\�[�X�̎��ɓo�^�����B
				registerNo = numRes;
			}
			if (registerNo < MAX_RESOURCE) {
				resTbl[registerNo] = res;
				if (numRes < registerNo + 1) {
					numRes = registerNo + 1;
				}
			}
			else {
				MessageBox(nullptr, errorMessage, L"�G���[", MB_OK);
				std::abort();
			}
		}
		enum {
			MAX_SHADER_RESOURCE = 1024 * 10,	//�V�F�[�_�[���\�[�X�̍ő吔�B
			MAX_CONSTANT_BUFFER = 1024 * 10,	//�萔�o�b�t�@�̍ő吔�B
			MAX_SAMPLER_STATE = 16,	//�T���v���X�e�[�g�̍ő吔�B
		};



		int m_numShaderResource = 0;
		int m_numConstantBuffer = 0;
		int m_numUavResource = 0;
		int m_numSamplerDesc = 0;
		int m_numDescriptorHeap = 0;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap[2] = { nullptr };
		std::vector<std::shared_ptr<IUnorderAccessResrouce>> m_UAVResources;
		std::vector < std::shared_ptr<IUnorderAccessResrouce>> m_uavResources;
		std::vector<std::shared_ptr<ConstantBuffer>> m_constantBuffers;
		D3D12_SAMPLER_DESC m_samplerDescs[MAX_SAMPLER_STATE];						//�T���v���X�e�[�g�B
		D3D12_GPU_DESCRIPTOR_HANDLE m_cbGpuDescriptorStart[2];						//�萔�o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
		D3D12_GPU_DESCRIPTOR_HANDLE m_srGpuDescriptorStart[2];						//�V�F�[�_�[���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
		D3D12_GPU_DESCRIPTOR_HANDLE m_uavGpuDescriptorStart[2];						//UAV���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
		D3D12_GPU_DESCRIPTOR_HANDLE m_samplerGpuDescriptorStart[2];					//Sampler�̂ŃX�N���v�^�q�[�v�̊J�n�n���h���B

};

