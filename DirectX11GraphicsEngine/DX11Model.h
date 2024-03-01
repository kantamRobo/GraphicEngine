#pragma once
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/Document.h>
#include <Model.h>
class DX11Model
{
	const Microsoft::glTF::Document& acc = {};
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader;

	using namespace Microsoft::glTF;
	std::vector< std::shared_ptr<SMesh> > m_meshs;
	
	bool Init(const Microsoft::glTF::Document& doc, const std::string& attrName);
	bool LoadMesh(const Microsoft::glTF::Mesh& mesh, const int primitiveIdx);
	
	
};

