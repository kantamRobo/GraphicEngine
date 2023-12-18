#include "VulkanModel.h"




for (const auto& mesh : doc.meshes.Elements())
{
    for (const auto& meshPrimitive : mesh.primitives)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // ���_�ʒu���A�N�Z�b�T�̎擾
        auto& idPos = meshPrimitive.GetAttributeAccessorId(ACCESSOR_POSITION);
        auto& accPos = doc.accessors.Get(idPos);
        // �@�����A�N�Z�b�T�̎擾
        auto& idNrm = meshPrimitive.GetAttributeAccessorId(ACCESSOR_NORMAL);
        auto& accNrm = doc.accessors.Get(idNrm);
        // �e�N�X�`�����W���A�N�Z�b�T�̎擾
        auto& idUV = meshPrimitive.GetAttributeAccessorId(ACCESSOR_TEXCOORD_0);
        auto& accUV = doc.accessors.Get(idUV);
        // ���_�C���f�b�N�X�p�A�N�Z�b�T�̎擾
        auto& idIndex = meshPrimitive.indicesAccessorId;
        auto& accIndex = doc.accessors.Get(idIndex);

        // �A�N�Z�b�T����f�[�^����擾
        auto vertPos = reader->ReadBinaryData<float>(doc, accPos);
        auto vertNrm = reader->ReadBinaryData<float>(doc, accNrm);
        auto vertUV = reader->ReadBinaryData<float>(doc, accUV);

        auto vertexCount = accPos.count;
        for (uint32_t i = 0; i < vertexCount; ++i)
        {
            // ���_�f�[�^�̍\�z
            int vid0 = 3 * i, vid1 = 3 * i + 1, vid2 = 3 * i + 2;
            int tid0 = 2 * i, tid1 = 2 * i + 1;
            vertices.emplace_back(
                Vertex{
                  vec3(vertPos[vid0], vertPos[vid1],vertPos[vid2]),
                  vec3(vertNrm[vid0], vertNrm[vid1],vertNrm[vid2]),
                  vec2(vertUV[tid0],vertUV[tid1])
                }
            );
        }
        // �C���f�b�N�X�f�[�^
        indices = reader->ReadBinaryData<uint32_t>(doc, accIndex);