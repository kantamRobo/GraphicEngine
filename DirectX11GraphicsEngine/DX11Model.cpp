#include "DirectX11IndexBuffer.h"
#include <GLTFSDK/GLTFResourceReader.h>
#include "Vertex.h"

#include "DX11Model.h"

bool DX11Model::Init(const Microsoft::glTF::Document& doc,const std::string& attrName)
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


			// ���_�ʒu���A�N�Z�b�T�̎擾
			auto& idPos = meshPrimitives.GetAttributeAccessorId(ACCESSOR_POSITION);
			auto& accPos = doc.accessors.Get(idPos);
			// �@�����A�N�Z�b�T�̎擾
			auto& idNrm = meshPrimitives.GetAttributeAccessorId(ACCESSOR_NORMAL);
			auto& accNrm = doc.accessors.Get(idNrm);
			// �e�N�X�`�����W���A�N�Z�b�T�̎擾
			auto& idUV = meshPrimitives.GetAttributeAccessorId(ACCESSOR_TEXCOORD_0);
			auto& accUV = doc.accessors.Get(idUV);
			// ���_�C���f�b�N�X�p�A�N�Z�b�T�̎擾
			auto& idIndex = meshPrimitives.indicesAccessorId;
			auto& accIndex = doc.accessors.Get(idIndex);

			// �A�N�Z�b�T����f�[�^����擾
			auto vertPos = reader->ReadBinaryData<float>(doc, accPos);
			auto vertNrm = reader->ReadBinaryData<float>(doc, accNrm);
			auto vertUV = reader->ReadBinaryData<float>(doc, accUV);
			int numVertex = accPos.count;

			for (uint32_t i = 0; i < numVertex; ++i)
			{
				// ���_�f�[�^�̍\�z
				int vid0 = 3 * i, vid1 = 3 * i + 1, vid2 = 3 * i + 2;
				int tid0 = 2 * i, tid1 = 2 * i + 1;
				vertices.emplace_back(
					Vertex{
					  DirectX::XMFLOAT3(vertPos[vid0], vertPos[vid1],vertPos[vid2])
					  // DirectX::XMFLOAT3(vertNrm[vid0], vertNrm[vid1],vertNrm[vid2]),
					   //DirectX::XMFLOAT2(vertUV[tid0],vertUV[tid1])
					}
				);
			}

			//todo indices��gltf�p��
	 // �C���f�b�N�X�f�[�^
			indices = reader->ReadBinaryData<uint32_t>(doc, accIndex);

		}
	}






	//1. ���_�o�b�t�@���쐬�B

	int vertexStride = sizeof(Vertex);
	auto mesh = new SMesh;//������B����Mesh��gltf�p�ɉ��C����B
	mesh->skinFlags.reserve(tkmMesh.materials.size());
	mesh->m_vertexBuffer.InitVertexBuffer(vertexStride * vertices.size(), vertexStride);
	mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

	auto SetSkinFlag = [&](int index) {
		if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
			//�X�L��������B
			mesh->skinFlags.push_back(1);
		}
		else {
			//�X�L���Ȃ��B
			mesh->skinFlags.push_back(0);
		}
		};
	//2. �C���f�b�N�X�o�b�t�@���쐬�B
	if (!tkmMesh.indexBuffer16Array.empty()) {
		//�C���f�b�N�X�̃T�C�Y��2byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer16Array) {
			auto ib = new DirectX11IndexBuffer;
			auto hr =ib->InitIndexbuffer(static_cast<int>(tkIb.indices.size()) * 2);
			ib->Copy((uint16_t*)&tkIb.indices.at(0));

			//�X�L�������邩�ǂ�����ݒ肷��B
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	else {
		//�C���f�b�N�X�̃T�C�Y��4byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer32Array) {
			auto ib = new DirectX11IndexBuffer;
			ib->InitIndexbuffer(static_cast<int>(tkIb.indices.size()) * 4);
			ib->Copy((uint32_t*)&tkIb.indices.at(0));

			//�X�L�������邩�ǂ�����ݒ肷��B
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}

	/*
	//3. �}�e���A�����쐬�B
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
		//�쐬�����}�e���A�������J�E���g����B
		materialNum++;
		mesh->m_materials.push_back(mat);
	}

	*/
	m_meshs[meshNo] = mesh;
}

