#define NOMINMAX


/*
* MIT licence
山田 英伸 2019-09-14
https://github.com/techlabxe/vulkan_book_1/tree/master/04_DrawModel
vulkan_book_1/04_DrawModel/ModelApp.cppより引用

Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation
files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include "DirectX11IndexBuffer.h"

#include "Vertex.h"

#include "DX11Model.h"


DX11Model::DX11Model(ID3D11Device* device, const std::string& attrName)
{
	// モデルデータの読み込み
	auto modelFilePath = std::filesystem::path(attrName);
	if (modelFilePath.is_relative())
	{
		auto current = std::filesystem::current_path();
		current /= modelFilePath;
		current.swap(modelFilePath);
	}
	m_mesh = std::make_shared<DX11Mesh>();
	
	
	auto reader = std::make_unique<StreamReader>(modelFilePath);
	auto glbStream = reader->GetInputStream(modelFilePath.filename().string());
	auto glbResourceReader = std::make_shared<Microsoft::glTF::GLBResourceReader>(std::move(reader), std::move(glbStream));
	auto document = Microsoft::glTF::Deserialize(glbResourceReader->GetJson());
	
	Init(device,document,attrName);

}

bool DX11Model::Init(ID3D11Device* device, const Microsoft::glTF::Document& doc, 
	const std::string& attrName)
{



	const Microsoft::glTF::Document& acc = {};
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader;

	using namespace Microsoft::glTF;
	
	
	m_mesh = std::make_shared<DX11Mesh>();
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
				m_mesh->vertices.emplace_back(
					Vertex{
					  DirectX::XMFLOAT3(vertPos[vid0], vertPos[vid1],vertPos[vid2])
					  // DirectX::XMFLOAT3(vertNrm[vid0], vertNrm[vid1],vertNrm[vid2]),
					   //DirectX::XMFLOAT2(vertUV[tid0],vertUV[tid1])
					}
				);
			}

			//todo indicesをgltf用に
	 // インデックスデータ
			m_mesh->indices = reader->ReadBinaryData<uint32_t>(doc, accIndex);

		}
	}



	


	//頂点・インデックスバッファを作成。

	
	//mesh->skinFlags.reserve(tkmMesh.materials.size());
	m_mesh->m_vertexbuffer->InitVertexBuffer(device, m_mesh->vertices);
	m_mesh->m_indexbuffer->InitIndexbuffer(device,m_mesh->indices, m_mesh->indices.size() * sizeof(uint32_t));
	/*
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
		*/
	/*
	//2. インデックスバッファを作成。
	if (!tkmMesh.indexBuffer16Array.empty()) {
		//インデックスのサイズが2byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer16Array) {
			auto ib = new DirectX11IndexBuffer;
			auto hr =ib->InitIndexbuffer(static_cast<int>(tkIb.indices.size()) * 2);
			

			//スキンがあるかどうかを設定する。
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	else {
		//インデックスのサイズが4byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer32Array) {
			auto ib = new DirectX11IndexBuffer;
			ib->InitIndexbuffer(static_cast<int>(tkIb.indices.size()) * 4);
			ib->Copy((uint32_t*)&tkIb.indices.at(0));

			//スキンがあるかどうかを設定する。
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	*/
	/*
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

	*/
	return true;
}

