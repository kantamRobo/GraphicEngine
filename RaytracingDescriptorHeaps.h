#pragma once
class GPUBuffer;
class DescriptorHeap;
namespace raytracing {

	class DescriptorHeaps {
	public:
		//各種リソースをディスクリプタヒープに
		//登録していく。
		
		void InitDescriptorHeaps(
			World& world,
			GPUBuffer& outputBuffer,
			ConstantBuffer& rayGeneCB
		);

		//シェーダーリソース、アンオーダードアクセスシェーダーリソース
		//ディスクリプタが登録されているディスクリプタヒープを取得


		const DescriptorHeap& GetSrvUavCbvDescriptorHeap()const
		{
			return m_srvUavCbvHeap;
		}
		
		/// <summary>
		/// サンプラのディスクリプタが登録されているディスクリプタヒープを取得。
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