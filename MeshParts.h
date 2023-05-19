#include "StructuredBuffer.h"
#include <string>
#include "Matrix.h"
class RenderContext;
class Skeleton;
class Material;
class IShaderResource;

const int MAX_MODEL_EXPAND_SRV = 6;	//拡張SRVの最大数。
const int  MAX_RENDERING_TARGET = 8;
struct SMesh {
	VertexBuffer m_vertexbuffer;
	std::vector< std::shared_ptr<IndexBuffer> >		m_indexBufferArray;
	std::vector<std::shared_ptr<Material>> m_materials;
    std::vector<int>				skinFlags;
};

class MeshParts {
public:
	~MeshParts();

	void InitFromAssimpFile(

		const  std::string& fxFilePath,
		const char* vsEntryPointFunc,
		const char* vsSkinEntryPointFunc,
		const char* psEntryPointFunc,
		void* expandData,
		int expandDataSize,
		const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView,
		const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat,
		D3D12_FILTER samplerFilter

	);

	void Draw(std::shared_ptr<RenderContext>rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj);


	void DrawInstancing(std::shared_ptr<RenderContext>rc,int numInstance, const Matrix& mView, const Matrix& mProj);


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

	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		for (int i = 0; i < m_meshs.size(); i++) {
			queryFunc(*m_meshs[i], m_descriptorHeap);
		}
	}

	/// <summary>
	/// ディスクリプタヒープを作成。
	/// </summary>
	void CreateDescriptorHeaps();


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
	void CreateMeshFromTkmMesh(
		const TkmFile::SMesh& mesh,
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
	void DrawCommon(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj);

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
		Matrix mWorld;		//ワールド行列。
		Matrix mView;		//ビュー行列。
		Matrix mProj;		//プロジェクション行列。
	};
	ConstantBuffer m_commonConstantBuffer;					//メッシュ共通の定数バッファ。
	ConstantBuffer m_expandConstantBuffer;

	std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV> m_expandShaderResourceView = { nullptr };	//ユーザー拡張シェーダーリソースビュー。
	StructuredBuffer m_boneMatricesStructureBuffer;	//ボーン行列の構造化バッファ。
	std::vector< std::shared_ptr<SMesh> > m_meshes;
	
	DescriptorHeap m_descriptorHeap;					//ディスクリプタヒープ。
	std::shared_ptr<Skeleton> m_skeleton = nullptr;						//スケルトン。
	void* m_expandData = nullptr;//メッシュ。

};