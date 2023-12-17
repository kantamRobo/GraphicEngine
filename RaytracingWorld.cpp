#include "stdafx.h"
#include "MeshParts.h"
#include "BLASBuffer.h"
#include "TLASBuffer.h"
#include "Model.h"
#include "RaytracingWorld.h"

namespace raytracing {


	void raytracing::World::RegistGeometry(Model& model)
	{
		model.QueryMeshAndDescriptorHeap([&](const std::shared_ptr<SMesh> mesh, const std::shared_ptr<DescriptorHeap> ds) {

			for (int i = 0; i < mesh->m_materials.size(); i++)
			{
				const auto& vertexBufferView = mesh->m_vertexBuffer.GetView();
				const auto& indexBufferView = mesh->m_indexBufferArray[i]->GetView();
				D3D12_RAYTRACING_GEOMETRY_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
				desc.Triangles.Transform3x4 = 0;
				desc.Triangles.VertexBuffer.StartAddress = vertexBufferView.BufferLocation;
				desc.Triangles.VertexCount = vertexBuferView.Size();
				desc.Triangles.IndexBuffer = indexBufferView.BufferLocation;
				desc.Triangles.IndexCount = mesh.m_indexBufferArray[i]->GetCount();
				desc.Triangles.IndexFormat = indexBufferView.Format;

				InstancePtr instance = std::make_unique<Instance>();
				instance->geometoryDesc = desc;
				instance->m_material = mesh.m_materials[i];
				instance->m_vertexBufferRWSB.InitRWStructuredBuffer(mesh.m_vertexBuffer, false);
				instance->m_indexBufferRWSB.InitRWStructuredBuffer(*mesh.m_indexBufferArray[i], false);

				m_instances.emplace_back(std::move(instance));
			}
			}

	}

	void raytracing::World::CommitRegistGeometry(RenderContext& rc)
	{
		//BLASÇç\ízÅB
		m_blasBuffer.InitBLASBuffer(rc, m_instances);
		//TLASÇç\íz
		m_topLevelASBuffers.InitTLASBuffer(rc, m_instances, m_blasBuffer.Get());

	}
}