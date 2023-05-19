#pragma once
#include "IUnorderAccessResrouce.h"
#include <vector>
#include "IUnorderAccessResrouce.h"


class DescriptorHeap
{
public:

	//コンストラクタ
	DescriptorHeap();
	//デストラクタ
	~DescriptorHeap();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Get(); const

		/// <summary>
		/// シェーダーリソースをディスクリプタヒープに登録。
		/// </summary>
		/// <param name="registerNo">
		/// レジスタ番号。-1が指定されたら、現在登録されているリソース数の次のレジスタが使用される。
		/// </param>
		/// <param name="sr">シェーダーリソース</param>
		void RegistShaderResource(int registerNo, std::shared_ptr<IShaderResource> sr)
	{
		RegistResource(
			registerNo,
			&sr,
			&m_shaderResources.front(),
			m_numShaderResource,
			MAX_SHADER_RESOURCE,
			L"DescriptorHeap::RegistShaderResource() レジスタ番号が範囲外です。"
		);
	}
	
	/// <summary>
	/// ディスクリプタヒープへの登録を確定。
	/// </summary>
	void Commit();
	/// <summary>
	/// サンプラステート用のディスクリプタヒープへの登録。
	/// </summary>
	void CommitSamplerHeap();


	/// <summary>
	/// アンオーダーアクセスリソースを登録。
	/// </summary>
	/// <param name="registerNo">
	/// レジスタ番号。-1が指定されたら、現在登録されているリソース数の次のレジスタが使用される。
	/// </param>
	/// <param name="sr">アンオーダーリソース</param>
	void RegistUnorderAccessResource(int registerNo, IUnorderAccessResrouce& sr)
	{
		RegistResource(
			registerNo,
			&sr,
			&m_uavResources.front(),
			m_numUavResource,
			MAX_SHADER_RESOURCE,
			L"DescriptorHeap::RegistUnorderAccessResource() レジスタ番号が範囲外です。"
		);
	}
	/// <summary>
	/// 定数バッファをディスクリプタヒープに登録。
	/// </summary>
	/// <param name="registerNo">
	/// レジスタ番号。-1が指定されたら、現在登録されているリソース数の次のレジスタが使用される。
	/// </param>
	/// <param name="cb">定数バッファ</param>
	void RegistConstantBuffer(int registerNo, ConstantBuffer& cb)
	{
		RegistResource(
			registerNo,
			&cb,
			&m_constantBuffers.front(),
			m_numConstantBuffer,
			MAX_CONSTANT_BUFFER,
			L"DescriptorHeap::RegistConstantBuffer() レジスタ番号が範囲外です。"
		);
	}
	/// <summary>
	/// サンプラ定義をディスクリプタヒープに追加。
	/// </summary>
	/// <param name="registerNo">
	/// レジスタ番号。-1が指定されたら、現在登録されているリソース数の次のレジスタが使用される。
	/// </param>
	/// <param name="desc">
	/// サンプラ定義
	/// </param>
	void RegistSamplerDesc(int registerNo, const D3D12_SAMPLER_DESC& desc)
	{
		RegistResource(
			registerNo,
			desc,
			m_samplerDescs,
			m_numSamplerDesc,
			MAX_SAMPLER_STATE,
			L"DescriptorHeap::RegistSamplerDesc() レジスタ番号が範囲外です。"
		);
	}

	ID3D12DescriptorHeap* get()
	{
		return m_descriptorHeap->Get();
	}

	/// <summary>
	/// シェーダーリソースが一つでも登録されているか判定。
	/// </summary>
	/// <returns></returns>
	bool IsRegistShaderResource() const
	{
		return m_numShaderResource != 0;
	}
	public:
	/// <summary>
	/// 定数バッファが一つでも登録されているか判定。
	/// </summary>
	/// <returns></returns>
	bool IsRegistConstantBuffer() const
	{
		return m_numConstantBuffer != 0;
	}
	/// <summary>
	/// UAVリソースが一つでも登録されているか判定。
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
	/// リソースをディスクリプタヒープに登録。
	/// </summary>
	/// <param name="registerNo">登録番号</param>
	/// <param name="res">登録するリソース</param>
	/// <param name="resTbl">リソーステーブル。このテーブルにリソースが追加されます。</param>
	/// <param name="numRes">登録されているリソースの数。本関数を呼び出すと、この数が１インクリメントされます。</param>
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
				//-1が指定されていたら、現在登録されている末尾のリソースの次に登録される。
				registerNo = numRes;
			}
			if (registerNo < MAX_RESOURCE) {
				resTbl[registerNo] = res;
				if (numRes < registerNo + 1) {
					numRes = registerNo + 1;
				}
			}
			else {
				MessageBox(nullptr, errorMessage, L"エラー", MB_OK);
				std::abort();
			}
		}
		enum {
			MAX_SHADER_RESOURCE = 1024 * 10,	//シェーダーリソースの最大数。
			MAX_CONSTANT_BUFFER = 1024 * 10,	//定数バッファの最大数。
			MAX_SAMPLER_STATE = 16,	//サンプラステートの最大数。
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
		D3D12_SAMPLER_DESC m_samplerDescs[MAX_SAMPLER_STATE];						//サンプラステート。
		D3D12_GPU_DESCRIPTOR_HANDLE m_cbGpuDescriptorStart[2];						//定数バッファのディスクリプタヒープの開始ハンドル。
		D3D12_GPU_DESCRIPTOR_HANDLE m_srGpuDescriptorStart[2];						//シェーダーリソースのディスクリプタヒープの開始ハンドル。
		D3D12_GPU_DESCRIPTOR_HANDLE m_uavGpuDescriptorStart[2];						//UAVリソースのディスクリプタヒープの開始ハンドル。
		D3D12_GPU_DESCRIPTOR_HANDLE m_samplerGpuDescriptorStart[2];					//Samplerのでスクリプタヒープの開始ハンドル。

};

