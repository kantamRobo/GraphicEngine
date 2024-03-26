#pragma once
class GPUBuffer;
class DescriptorHeap;
namespace raytracing {

	class DescriptorHeaps {
	public:
		//�e�탊�\�[�X���f�B�X�N���v�^�q�[�v��
		//�o�^���Ă����B
		
		void InitDescriptorHeaps(
			World& world,
			GPUBuffer& outputBuffer,
			ConstantBuffer& rayGeneCB
		);

		//�V�F�[�_�[���\�[�X�A�A���I�[�_�[�h�A�N�Z�X�V�F�[�_�[���\�[�X
		//�f�B�X�N���v�^���o�^����Ă���f�B�X�N���v�^�q�[�v���擾


		const DescriptorHeap& GetSrvUavCbvDescriptorHeap()const
		{
			return m_srvUavCbvHeap;
		}
		
		/// <summary>
		/// �T���v���̃f�B�X�N���v�^���o�^����Ă���f�B�X�N���v�^�q�[�v���擾�B
		/// </summary>
		/// <returns></returns>
		/// 
		const DescriptorHeap& GetSamplerDescriptorHeap() const
		{
			return m_samplerDescriptorHeap;
		}
		

	private:
		DescriptorHeap m_srvUavCbvHeap;
		DescriptorHeap m_samplerDescriptorHeap;

	};


}