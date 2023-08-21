#include "stdafx.h"
#include "Matrix.h"
#include "Vector.h"
#include "MeshParts.h"
#include "Model.h"
#include "RaytracingWorld.h"


void raytracing::World::RegistGeometry(std::shared_ptr<Model> model)
{
	model->QueryMeshAndDescriptorHeap([&](const std::shared_ptr<SMesh> mesh, const std::shared_ptr<DescriptorHeap> ds) {

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
			instance->m_vertexBufferRWSB.Init(mesh.m_vertexBuffer, false);
			instance->m_indexBufferRWSB.Init(*mesh.m_indexBufferArray[i], false);

			m_instances.emplace_back(std::move(instance));
		}
		}

}

void raytracing::World::CommitRegistGeometry(std::shared_ptr<RenderContext> rc)
{
	//BLAS���\�z�B
	m_blasBuffer.Init(rc, m_instances);
	//TLAS���\�z
	m_topLevelASBuffers.Init(rc,m_instances,m_blasBuffer.Get())

}
