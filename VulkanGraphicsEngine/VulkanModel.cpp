#include "VulkanModel.h"




for (const auto& mesh : doc.meshes.Elements())
{
    for (const auto& meshPrimitive : mesh.primitives)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // 頂点位置情報アクセッサの取得
        auto& idPos = meshPrimitive.GetAttributeAccessorId(ACCESSOR_POSITION);
        auto& accPos = doc.accessors.Get(idPos);
        // 法線情報アクセッサの取得
        auto& idNrm = meshPrimitive.GetAttributeAccessorId(ACCESSOR_NORMAL);
        auto& accNrm = doc.accessors.Get(idNrm);
        // テクスチャ座標情報アクセッサの取得
        auto& idUV = meshPrimitive.GetAttributeAccessorId(ACCESSOR_TEXCOORD_0);
        auto& accUV = doc.accessors.Get(idUV);
        // 頂点インデックス用アクセッサの取得
        auto& idIndex = meshPrimitive.indicesAccessorId;
        auto& accIndex = doc.accessors.Get(idIndex);

        // アクセッサからデータ列を取得
        auto vertPos = reader->ReadBinaryData<float>(doc, accPos);
        auto vertNrm = reader->ReadBinaryData<float>(doc, accNrm);
        auto vertUV = reader->ReadBinaryData<float>(doc, accUV);

        auto vertexCount = accPos.count;
        for (uint32_t i = 0; i < vertexCount; ++i)
        {
            // 頂点データの構築
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
        // インデックスデータ
        indices = reader->ReadBinaryData<uint32_t>(doc, accIndex);