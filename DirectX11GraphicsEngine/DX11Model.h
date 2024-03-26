#pragma once
#define NOMINMAX
#include "streamreader.h"
#include <Model.h>
#include "DX11Mesh.h"

#include <memory>
class DX11Model
{
public:
	

	


	DX11Model(ID3D11Device* device, const std::string& attrName);

	bool Init(ID3D11Device* device, const Microsoft::glTF::Document& doc, const std::string& attrName, Microsoft::glTF::GLTFResourceReader* reader);

	
	
	//, const Microsoft::glTF::Document& doc, const std::string& attrName
	
	
	//, const Microsoft::glTF::Document& doc, const std::string& attrName
		
	std::shared_ptr<DX11Mesh> m_mesh;
	//bool LoadMesh(const Microsoft::glTF::Mesh& mesh, const int primitiveIdx);
private:
	
	


	

	
	
	
};

