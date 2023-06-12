#include "TLASBuffer.h"
#include "stdafx.h"

namespace raytracing {


	extern ComPtr<ID3D12Resource> CreateBuffer
	(
		ID3D12Device5* pDevice,
		uint64_t size,
		D3D12_RESOURCE_FLAGS flags,
		D3D12_RESOURCE_STATES initState,
		const D3D12_HEAP_PROPERTIES& heapProps
	);

	extern const D3D12_HEAP_PROPERTIES kUploadHeapProps;

	extern const D3D12_HEAP_PROPERTIES kDefaultHeapProps;


	void TLASBuffer::InitTLASBuffer(std::shared_ptr<RenderContext> rc, const std::vector<InstancePtr>& instances, const std::vector<AccelerationStructureBuffers>& bottomLevelASBuffers)
	{
		uint64_t tlasSize;

		auto d3dDevice = g_GraphicEngine->GetD3DDevice();

		int numInstance = static_cast<int>(instances.size());

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
		inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
		inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
		inputs.NumDescs = numInstance;
		inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info;
		d3dDevice->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);


		//çXêV?
		//êVãK?

		m_topLevelASBuffers.pScratch = CreateBuffer(d3dDevice, info.ScratchDataSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, kDefaultHeapProps);
		m_topLevelASBuffers.pResult = CreateBuffer(d3dDevice, info.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, kDefaultHeapProps);
			m_topLevelASBuffers.pInstanceDesc = CreateBuffer(
				d3dDevice,
				sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * numInstance,
				D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ,
				kUploadHeapProps
			);
		tlasSize = info.ResultDataMaxSizeInBytes;
	
	
	   //Map the 
	}

}