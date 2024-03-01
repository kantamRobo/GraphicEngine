




#include "Vector.h"

#include "Material.h"
#include "RenderContext.h"
#include "IndexBuffer.h"

#include "DescriptorHeap.h"
#include <memory>
#include "MeshParts.h"
#include "Vertex.h"
MeshParts::~MeshParts()
{
	for (auto& mesh : m_meshs)
	{
		//インデックスバッファを削除。
		for (auto& ib : mesh->m_indexBufferArray) {
			mesh.reset();
		}
		//マテリアルを削除
		for (auto& mat : mesh->m_materials)
		{
			mat.reset();
		}
	//メッシュを削除
		mesh.reset();
	}


}

void MeshParts::InitFromGltfFile(const aiScene* scene,const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, void* expandData, int expandDataSize, const std::array<std::shared_ptr<IShaderResource>, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter)
{
	//assimpから頂点をロードしたいが、
	// 処理が複雑なので調査してからメッシュメモリ確保の
	//実装を書く

	int meshNo = 0;
	int materialNo = 0;

	CreateMeshFromAssimpMesh(meshNo,
		materialNo,
		fxFilePath,
		vsEntryPointFunc,
		vsSkinEntryPointFunc,
		vsSkinEntryPointFunc,
		psEntryPointFunc,
		,
		samplerFilter
	);

	//共通定数バッファの作成。
	m_commonConstantBuffer.InitConstBuffer(sizeof(SConstantBuffer), nullptr);

	//ユーザー拡張用の定数バッファを作成。
	if (expandData) {
		m_expandConstantBuffer.InitConstBuffer(expandDataSize, nullptr);
		m_expandData = expandData;
	}

	for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
		m_expandShaderResourceView[i] = expandShaderResourceView[i];
	}

	//ディスクリプタヒープを作成。
	CreateDescriptorHeaps();
}

void MeshParts::BindSkeleton(std::shared_ptr<Skeleton> skeleton)
{
	m_skeleton = skeleton;
	m_boneMatricesStructureBuffer->InitStructuredBuffer(
		sizeof(EngineMath::Matrix),
		m_skeleton->GetNumBones(),
		m_skeleton->GetBoneMatricesTopAddress()
		);


}

void MeshParts::DrawCommon(std::shared_ptr<RenderContext> rc, const EngineMath::Matrix& mWorld, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj)
{
	//メッシュごとにドロー
	//プリミティブのトポロジーはトライアングルリストのみ。
	rc->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//定数バッファを更新する。
	SConstantBuffer cb;
	cb.mWorld = mWorld;
	cb.mView = mView;
	cb.mProj = mProj;
	m_commonConstantBuffer.CopyToVRAM(cb);

	if (m_expandData) {
		m_expandConstantBuffer.CopyToVRAM(m_expandData);
	}
	if (m_boneMatricesStructureBuffer->IsInited()) {
		//ボーン行列を更新する。
		m_boneMatricesStructureBuffer->Update(m_skeleton->GetBoneMatricesTopAddress());
	}
}


void MeshParts::DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj)
{
	//定数バッファの設定、更新など描画の共通処理を実行する。
	DrawCommon(rc, g_matIdentity, mView, mProj);

	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		//1. 頂点バッファを設定。
		rc->SetVertexBuffer(mesh->m_vertexBuffer);
		//マテリアルごとにドロー。
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			//このマテリアルが貼られているメッシュの描画開始。
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);
			//2. ディスクリプタヒープを設定。
			rc->SetDescriptorHeap(m_descriptorHeap.get());
			//3. インデックスバッファを設定。
			auto& ib = mesh->m_indexBufferArray[matNo];
			rc->SetIndexBuffer(*ib);

			//4. ドローコールを実行。
			rc->DrawIndexedInstanced(ib->GetCount(), numInstance);
			descriptorHeapNo++;
		}
	}

}


void MeshParts::Draw(std::shared_ptr<RenderContext> rc, 
	const EngineMath::Matrix& mWorld, 
	const EngineMath::Matrix& mView,
	const EngineMath::Matrix& mProj)
{
	//定数バッファの設定、更新など描画の共通処理を実行する。
	DrawCommon(rc, mWorld, mView, mProj);

	int descriptorHeapNo = 0;

	for (auto& mesh : m_meshs)
	{
		rc->SetVertexBuffer(mesh->m_vertexBuffer);

		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo)
		{
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);

			rc->SetDescriptorHeap(m_descriptorHeap.get());

		}
	}

}

void MeshParts::DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj)
{
	//定数バッファの設定、更新など描画の共通処理を実行する。
	DrawCommon(rc, g_matIdentity, mView, mProj);

	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		//1. 頂点バッファを設定。
		rc->SetVertexBuffer(mesh->m_vertexBuffer);
		//マテリアルごとにドロー。
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			//このマテリアルが貼られているメッシュの描画開始。
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);
			//2. ディスクリプタヒープを設定。
			rc->SetDescriptorHeap(m_descriptorHeap.get());
			//3. インデックスバッファを設定。
			auto& ib = mesh->m_indexBufferArray[matNo];
			rc->SetIndexBuffer(*ib);

			//4. ドローコールを実行。
			rc->DrawIndexedInstanced(ib->GetCount(), numInstance);
			descriptorHeapNo++;
		}
	}
}


void MeshParts::CreateDescriptorHeaps()
{

	//ディスクリプタヒープを構築していく。
	int srvNo = 0;
	int cbNo = 0;
	for (auto& mesh : m_meshs) {
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {

			//ディスクリプタヒープにディスクリプタを登録していく。
			m_descriptorHeap->RegistShaderResource(srvNo, mesh->m_materials[matNo]->GetAlbedoMap().get());			//アルベドマップ。
			m_descriptorHeap->RegistShaderResource(srvNo + 1, mesh->m_materials[matNo]->GetNormalMap().get());		//法線マップ。
			m_descriptorHeap->RegistShaderResource(srvNo + 2, mesh->m_materials[matNo]->GetSpecularMap().get());		//スペキュラマップ。
			m_descriptorHeap->RegistShaderResource(srvNo + 3, m_boneMatricesStructureBuffer);					//ボーンのストラクチャードバッファ。
			for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
				if (m_expandShaderResourceView[i]) {
					m_descriptorHeap->RegistShaderResource(srvNo + EXPAND_SRV_REG__START_NO + i, m_expandShaderResourceView[i]);
				}
			}
			srvNo += NUM_SRV_ONE_MATERIAL;
			m_descriptorHeap->RegistConstantBuffer(cbNo, m_commonConstantBuffer);
			if (m_expandConstantBuffer.IsValid()) {
				m_descriptorHeap->RegistConstantBuffer(cbNo + 1, m_expandConstantBuffer);
			}
			cbNo += NUM_CBV_ONE_MATERIAL;
		}
	}
	m_descriptorHeap->Commit();
}

void MeshParts::CreateMeshFromAssimpMesh(const Microsoft::glTF::Document& doc,int meshNo, int& materialNum, const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter)
{





	const Microsoft::glTF::Document& acc = {}; 
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader;

	using namespace Microsoft::glTF;
	std::vector<Vertex> vertices;
	std::vector <uint32_t> indices;
	for (const auto& mesh : acc.meshes.Elements())
	{
		for (const auto& meshPrimitives : mesh.primitives)
		{
			

			// 頂点位置情報アクセッサの取得
			auto& idPos = meshPrimitives.GetAttributeAccessorId(ACCESSOR_POSITION);
			auto& accPos = doc.accessors.Get(idPos);
			// 法線情報アクセッサの取得
			auto& idNrm = meshPrimitives.GetAttributeAccessorId(ACCESSOR_NORMAL);
			auto& accNrm = doc.accessors.Get(idNrm);
			// テクスチャ座標情報アクセッサの取得
			auto& idUV = meshPrimitives.GetAttributeAccessorId(ACCESSOR_TEXCOORD_0);
			auto& accUV = doc.accessors.Get(idUV);
			// 頂点インデックス用アクセッサの取得
			auto& idIndex = meshPrimitives.indicesAccessorId;
			auto& accIndex = doc.accessors.Get(idIndex);

			// アクセッサからデータ列を取得
			auto vertPos = reader->ReadBinaryData<float>(doc, accPos);
			auto vertNrm = reader->ReadBinaryData<float>(doc, accNrm);
			auto vertUV = reader->ReadBinaryData<float>(doc, accUV);
			int numVertex = accPos.count;

			for (uint32_t i = 0; i < numVertex; ++i)
			{
				// 頂点データの構築
				int vid0 = 3 * i, vid1 = 3 * i + 1, vid2 = 3 * i + 2;
				int tid0 = 2 * i, tid1 = 2 * i + 1;
				vertices.emplace_back(
					Vertex{
					  XMFLOAT3(vertPos[vid0], vertPos[vid1],vertPos[vid2]),
					  XMFLOAT3(vertNrm[vid0], vertNrm[vid1],vertNrm[vid2]),
					  XMFLOAT2(vertUV[tid0],vertUV[tid1])
					}
				);
			}

			//todo indicesをgltf用に
	 // インデックスデータ
			indices = reader->ReadBinaryData<uint32_t>(doc, accIndex);

		}
	}
	





	//1. 頂点バッファを作成。
	
	int vertexStride = sizeof(Vertex);
	auto mesh = new SMesh;//しおり。このMeshもgltf用に改修する。
	mesh->skinFlags.reserve(tkmMesh.materials.size());
	mesh->m_vertexBuffer.InitVertexBuffer(vertexStride *vertices.size(), vertexStride);
	mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

	auto SetSkinFlag = [&](int index) {
		if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
			//スキンがある。
			mesh->skinFlags.push_back(1);
		}
		else {
			//スキンなし。
			mesh->skinFlags.push_back(0);
		}
	};
	//2. インデックスバッファを作成。
	if (!tkmMesh.indexBuffer16Array.empty()) {
		//インデックスのサイズが2byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer16Array) {
			auto ib = new IndexBuffer;
			ib->InitIndexBuffer(static_cast<int>(tkIb.indices.size()) * 2, 2);
			ib->Copy((uint16_t*)&tkIb.indices.at(0));

			//スキンがあるかどうかを設定する。
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	else {
		//インデックスのサイズが4byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer32Array) {
			auto ib = new IndexBuffer;
			ib->InitIndexBuffer(static_cast<int>(tkIb.indices.size()) * 4, 4);
			ib->Copy((uint32_t*)&tkIb.indices.at(0));

			//スキンがあるかどうかを設定する。
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	//3. マテリアルを作成。
	mesh->m_materials.reserve(tkmMesh.materials.size());
	for (auto& tkmMat : tkmMesh.materials) {
		auto mat = new Material;
		mat->InitFromTkmMaterila(
			tkmMat,
			fxFilePath,
			vsEntryPointFunc,
			vsSkinEntryPointFunc,
			psEntryPointFunc,
			colorBufferFormat,
			NUM_SRV_ONE_MATERIAL,
			NUM_CBV_ONE_MATERIAL,
			NUM_CBV_ONE_MATERIAL * materialNum,
			NUM_SRV_ONE_MATERIAL * materialNum,
			samplerFilter
		);
		//作成したマテリアル数をカウントする。
		materialNum++;
		mesh->m_materials.push_back(mat);
	}

	m_meshs[meshNo] = mesh;
}


