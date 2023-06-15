#pragma once
#include "MeshParts.h"
#include "Skeleton.h"
#include "Vector.h"
#include "Matrix.h"
class IShaderResource;

//モデルの上方向
enum EnModelUpAxis {
	enModelUpAxisY,		//モデルの上方向がY軸。
	enModelUpAxisZ,		//モデルの上方向がZ軸。
};

//モデルの初期化データ
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;
	const char* m_vsEntryPointFunc = "VSMain";
	const char* m_vsSkinEntrypointFunc = "VSMain";
	const char* m_psEntryPointFunc = "PSMain";
	const char* m_fxFilePath = nullptr;
	void* m_expandConstantBuffer = nullptr;
	int m_expandConstantBufferSize = 0;

	std::array<DXGI_FORMAT, MAX_RENDERING_TARGET> m_colorBufferFormat = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
	};	//レンダリングするカラーバッファのフォーマット。

};

class Model
{
public:
	/// <summary>
	/// tkmファイルから初期化。
	/// </summary>
	/// <param name="initData">初期化データ</param>
	void InitModel(const ModelInitData& initData);

	/// <summary>
	/// ワールド行列を計算して、メンバ変数のm_worldMatrixをこぅしんする。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void Draw(RenderContext& renderContext);

	/// <summary>
	/// 描画(カメラ指定版)
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(RenderContext& renderContext, Camera& camera);
	/// <summary>
	/// 描画(カメラ行列指定版)
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	/// <param name="viewMatrix">ビュー行列</param>
	/// <param name="projMatrix">プロジェクション行列</param>
	void Draw(RenderContext& renderContext, const Matrix& viewMatrix, const Matrix& projMatrix);
	void DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance);
	/// <summary>
	/// インスタンシング描画
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	/// <param name="numInstance">インスタンスの数</param>
	void DrawInstancing(RenderContext& renderContext, int numInstance);
	/// <summary>
	/// ワールド行列を取得。
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}
	/// <summary>
	/// メッシュに対して問い合わせを行う。
	/// </summary>
	/// <param name="queryFunc">問い合わせ関数</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
	{
		m_meshParts.QueryMeshs(queryFunc);
	}
	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		m_meshParts.QueryMeshAndDescriptorHeap(queryFunc);
	}
	/// <summary>
	/// アルベドマップを変更。
	/// </summary>
	/// <remarks>
	/// この関数を呼び出すとディスクリプタヒープの再構築が行われるため、処理負荷がかかります。
	/// 毎フレーム呼び出す必要がない場合は呼び出さないようにしてください。
	/// </remarks>
	/// <param name="materialName">変更しいたマテリアルの名前</param>
	/// <param name="albedoMap">アルベドマップ</param>
	void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);
	/// <summary>
	/// TKMファイルを取得。
	/// </summary>
	/// <returns></returns>
	const TkmFile& GetTkmFile() const
	{
		return *m_tkmFile;
	}
	/// <summary>
	/// 初期化されているか判定。
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}

	/// <summary>
	/// ワールド行列を計算する。
	/// </summary>
	/// <remark>
	/// この関数はUpdateWorldMatrix関数の中から使われています。
	/// Modelクラスの使用に沿ったワールド行列の計算を行いたい場合、
	/// 本関数を利用すると計算することができます。
	/// </remark>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率。</param>
	/// <returns></returns>
	/// 
	Matrix CalcWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mWorld;
		Matrix mBias;
		if (m_modelUpAxis == enModelUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
		}
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);

		mWorld = mBias * mScale * mRot * mTrans;
		return mWorld;


	}
	private:

private:
	bool m_isInited = false;						// 初期化されている？
	Matrix m_world;									// ワールド行列。
	TkmFile* m_tkmFile;								// tkmファイル。
	Skeleton m_skeleton;							// スケルトン。
	MeshParts m_meshParts;							// メッシュパーツ。
	EnModelUpAxis m_modelUpAxis = enModelUpAxisY;



};
