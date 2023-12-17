#pragma once
#include <d3d12.h>
namespace raytracing {
	using ID3D12StateObjectPtr = CComPtr<ID3D12StateObject>;
	using ID3D12RootSignaturePtr = CComPtr<ID3D12RootSignature>;
	class DescriptorHeaps;
	/// <summary>
	/// レイトレ用のパイプラインステートオブジェクト。
	/// </summary>
	class PSO {
	private:
		/// <summary>
		/// ルートシグネチャ定義。
		/// </summary>
		struct RootSignatureDesc
		{
			D3D12_ROOT_SIGNATURE_DESC desc = {};
			std::vector<D3D12_DESCRIPTOR_RANGE> range;
			std::vector<D3D12_ROOT_PARAMETER> rootParams;
		};
	public:
		/// <summary>
		/// パイプラインステートオブジェクトを初期化。
		/// </summary>
		void InitRTPSO(DescriptorHeaps& descriptorHeaps);
		void QueryInterface(CComPtr< ID3D12StateObjectProperties >& props) const
		{
			m_pipelineState->QueryInterface(&props);
		}
		/// <summary>
		/// グローバルルートシグネチャを取得。
		/// </summary>
		/// <returns></returns>
		ID3D12RootSignaturePtr GetGlobalRootSignature()
		{
			return m_emptyRootSignature;
		}
		ID3D12StateObjectPtr Get()
		{
			return m_pipelineState;
		}
	private:
		RootSignatureDesc CreateRayGenRootSignatureesc();
		RootSignatureDesc CreatePBRMatterialHitRootSignatureDesc();
		
	private:
		const DescriptorHeap* m_srvUavCbvHeap = nullptr;		//SRV_UAV_CBV用のディスクリプタヒープ。
		ID3D12StateObjectPtr m_pipelineState;					//パイプラインステート
		ID3D12RootSignaturePtr m_emptyRootSignature;
		
	};


	/// </summary>
	struct ExportAssociationSubobject
	{
		void InitEx_A_Sub_O(const WCHAR* exportNames[], uint32_t exportCount, const D3D12_STATE_SUBOBJECT* pSubobjectToAssociate)
		{
			association.NumExports = exportCount;
			association.pExports = exportNames;
			association.pSubobjectToAssociate = pSubobjectToAssociate;

			subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			subobject.pDesc = &association;
		}
		D3D12_STATE_SUBOBJECT subobject = {};
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
	};
}