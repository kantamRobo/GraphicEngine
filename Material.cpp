
#include <string>
#include <memory>
#include "Engine.h"
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/Document.h>
#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/GLBResourceReader.h>
#include "Material.h"
using namespace std;
void Material::InitTexture(const Microsoft::glTF::Document& doc,const TkmFile::SMaterial& tkmMat)
{
	for (auto& m : doc.materials.Elements())
	{
	auto textureId = m.metallicRoughness.baseColorTexture.textureId;
	if (textureId.empty())
	{
		textureId = m.normalTexture.textureId;
	}
	//法線マップ。
	{
		if (tkmMat.normalMap != nullptr)
		{
			auto filePath = tkmMat.normalMapFilePath.c_str();

			// モデルデータの読み込み
			auto modelFilePath = experimental::filesystem::path(filePath);
			if (modelFilePath.is_relative())
			{
				auto current = experimental::filesystem::current_path();
				current /= modelFilePath;
				current.swap(modelFilePath);
			}
			auto reader = std::make_unique<Microsoft::glTF::GLTFResourceReader>(modelFilePath.parent_path());
			auto glbStream = reader->GetInputStream(modelFilePath.filename().u8string());
			auto glbResourceReader = std::make_shared<Microsoft::glTF::GLBResourceReader>(std::move(reader), std::move(glbStream));
			auto document = Microsoft::glTF::Deserialize(glbResourceReader->GetJson());
			auto& texture = doc.textures.Get(textureId);
			auto& image = doc.images.Get(texture.imageId);
			auto imageBufferView = doc.bufferViews.Get(image.bufferViewId);
			auto imageData = reader->ReadBinaryData<char>(doc, imageBufferView);

			auto imageData = reader->ReadBinaryData<char>(document, imageBufferView);
			map = tkmMat.normalMap.get();//しおり
			mapSize = tkmMat.normalMapSize;

			
			auto normalMap = g_engine->GetTextureFromBank(filePath);

			if (normalMap == nullptr)
			{
				normalMap = new Texture();
				normalMap->InitFromMemory(map, mapSize);
				g_engine->RegistTextureToBank(filePath, normalMap);
			}
			m_normalMap = normalMap;
		}

	}

}
