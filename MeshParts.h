/// <summary>
/// メッシュパーツクラス。
/// </summary>

#pragma once
class ConstantBuffer;
class RenderContext;
class Skeleton;
class SMaterial;
class IndexBuffer;
class VertexBuffer;
class IShaderResource;

class DescriptorHeap;
#include "StructuredBuffer.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "Skeleton.h"
#include "Matrix.h"


const int MAX_MODEL_EXPAND_SRV = 6;	//拡張SRVの最大数。

/// <summary>
/// メッシュ
/// </summary>
struct SMesh {
	VertexBuffer m_vertexBuffer;						//頂点バッファ。
	std::vector< std::shared_ptr<IndexBuffer> >		m_indexBufferArray;	//インデックスバッファ。
	std::vector < std::shared_ptr< SMaterial> > m_materials;			//マテリアル。
	std::vector<int>				skinFlags;				//スキンを持っているかどうかのフラグ。
};

/// <summary>
/// メッシュパーツ。
/// </summary>
class MeshParts {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~MeshParts();
	void InitFromGltfFile(const aiScene* scene, const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, void* expandData, int expandDataSize, const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter);
	
	
	void DrawCommon(std::shared_ptr<RenderContext> rc, const EngineMath::Matrix& mWorld, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
	/// <param name="mWorld">ワールド行列</param>
	/// <param name="mView">ビュー行列</param>
	/// <param name="mProj">プロジェクション行列</param>
	/// <param name="light">ライト</param>
	
	void Draw(std::shared_ptr<RenderContext> rc, const EngineMath::Matrix& mWorld, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);

	
	void DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);
	/// <summary>
	/// スケルトンを関連付ける。
	/// </summary>
	/// <param name="skeleton">スケルトン</param>
	void BindSkeleton(std::shared_ptr<Skeleton> skeleton);
	/// <summary>
	/// メッシュに対して問い合わせを行う。
	/// </summary>
	/// <param name="queryFunc">クエリ関数</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
	{
		for (const auto& mesh : m_meshs) {
			queryFunc(*mesh);
		}
	}
	
	/// <summary>
	/// ディスクリプタヒープを作成。
	/// </summary>
	void CreateDescriptorHeaps();
	void CreateMeshFromAssimpMesh(const Microsoft::glTF::Document& doc, int meshNo, int& materialNum, const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter);
private:
	/// <summary>
	/// tkmメッシュからメッシュを作成。
	/// </summary>
	/// <param name="mesh">メッシュ</param>
	/// <param name="meshNo">メッシュ番号</param>
	/// <param name="fxFilePath">fxファイルのファイルパス</param>
	/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="vsSkinEntryPointFunc">スキンありマテリアル用の頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
	/// <param name="colorBufferFormat">このモデルをレンダリングするカラーバッファのフォーマット</param>
	/// <param name="samplerFilter">サンプラフィルタ</param>
	void CreateMeshFromAssimpMesh(
		
		int meshNo,
		int& materialNum,
		const char* fxFilePath,
		const char* vsEntryPointFunc,
		const char* vsSkinEntryPointFunc,
		const char* psEntryPointFunc,
		const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat,
		D3D12_FILTER samplerFilter
	);
	/// <summary>
	/// 描画処理の共通処理
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
	/// <param name="mWorld">ワールド行列</param>
	/// <param name="mView">ビュー行列</param>
	/// <param name="mProj">プロジェクション行列</param>
	void DrawCommon(RenderContext& rc, const EngineMath::Matrix& mWorld, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);
	
private:
	//拡張SRVが設定されるレジスタの開始番号。
	const int EXPAND_SRV_REG__START_NO = 10;
	//１つのマテリアルで使用されるSRVの数。
	const int NUM_SRV_ONE_MATERIAL = EXPAND_SRV_REG__START_NO + MAX_MODEL_EXPAND_SRV;
	//１つのマテリアルで使用されるCBVの数。
	const int NUM_CBV_ONE_MATERIAL = 2;
	/// <summary>
	/// 定数バッファ。
	/// </summary>
	/// <remarks>
	/// この構造体を変更したら、SimpleModel.fxのCBも変更するように。
	/// </remarks>
	struct SConstantBuffer {
		EngineMath::Matrix mWorld;		//ワールド行列。
		EngineMath::Matrix mView;		//ビュー行列。
		EngineMath::Matrix mProj;		//プロジェクション行列。
	};
	ConstantBuffer m_commonConstantBuffer;					//メッシュ共通の定数バッファ。
	ConstantBuffer m_expandConstantBuffer;					//ユーザー拡張用の定数バッファ
	std::array<std::shared_ptr<IShaderResource>, MAX_MODEL_EXPAND_SRV> m_expandShaderResourceView = { nullptr };	//ユーザー拡張シェーダーリソースビュー。
	std::shared_ptr<StructuredBuffer> m_boneMatricesStructureBuffer;	//ボーン行列の構造化バッファ。
	std::vector< std::shared_ptr<SMesh> > m_meshs;						//メッシュ。
	//std::vector< DescriptorHeap > m_descriptorHeap;	//ディスクリプタヒープ。
	std::shared_ptr<DescriptorHeap> m_descriptorHeap;					//ディスクリプタヒープ。
	std::shared_ptr<Skeleton> m_skeleton = nullptr;						//スケルトン。
	void* m_expandData = nullptr;						//ユーザー拡張データ。
};