#include "Material.h"


enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};

void Material::InitTexture(const aiMaterial* aiMat)
{
	const auto& nullTextureMaps = g_graphicsEngine->GetNullTextureMaps();

	const char* filePath = nullptr;
	char* map = nullptr;
	unsigned int mapSize;

	//アルベドマップ
	{
		
		if (aiMat != nullptr)
		{
			
			aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, filePath);
			aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, map);
			
		aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, mapSize);

		}
		else
		{
			filePath = nullTextureMaps.GetAlbedoMapFilePath();
			map = nullTextureMaps.GetAlbedoMap().get();
			mapSize = nullTextureMaps.GetAlbedoMapSize();
		}

		auto albedoMap = g_engine->GetTextureFromBank(filePath);

		if (albedoMap == nullptr)
		{
			albedoMap = std::make_shared<Texture>();
			albedoMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, albedoMap);
		}
		m_albedoMap = albedoMap;
	}

	//法線マップ
	{
		if (aiMat != nullptr)
		{
			
			aiMat->Get(AI_MATKEY_TEXTURE_NORMALS, filePath);
			aiMat->Get(AI_MATKEY_TEXTURE_NORMALS, map);

			aiMat->Get(AI_MATKEY_TEXTURE_NORMALS, mapSize);
		}
		auto normalMap = g_engine->GetTextureFromBank(filePath);
		if (normalMap == nullptr)
		{
			normalMap = new Texture();
			normalMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, normalMap);
		}
		m_normalMap = normalMap;

	}



	//反射マップ
	{
		if (aiMat != nullptr)
		{

			aiMat->Get(AI_MATKEY_TEXTURE_REFLECTION, filePath);
			aiMat->Get(AI_MATKEY_TEXTURE_REFLECTION, map);

			aiMat->Get(AI_MATKEY_TEXTURE_REFLECTION, mapSize);
		}
		auto specularmap = g_engine->GetTextureFromBank(filePath);
		if (normalMap == nullptr)
		{
			normalMap = new Texture();
			normalMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, normalMap);
		}
	 m_specularMap = specularmap;

	}

	
}
