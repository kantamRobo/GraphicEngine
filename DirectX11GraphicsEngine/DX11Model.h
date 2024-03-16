#pragma once
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/Document.h>
#include <Model.h>
#include "DX11Mesh.h"
class DX11Model
{
public:
	DX11Model() {};
	

	DX11Model(ID3D11Device* device, const Microsoft::glTF::Document& doc, const std::string& attrName);

	bool Init(ID3D11Device* device, const Microsoft::glTF::Document& doc, const std::string& attrName);
    	
	
	//bool LoadMesh(const Microsoft::glTF::Mesh& mesh, const int primitiveIdx);
private:
	const Microsoft::glTF::Document& acc = {};
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader;


	std::shared_ptr<DX11Mesh> m_mesh;

	
};

