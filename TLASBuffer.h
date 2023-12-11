#pragma once
#include <memory>
#include "Raytracing.h"
namespace raytracing
{
	class TLASBuffer :public IShaderResource
	{
	public:

		void InitTLASBuffer(std::shared_ptr<RenderContext> rc,
			const std::vector<InstancePtr>& instances,
			const std::vector< AccelerationStructureBuffers>& bottomLevelASBuffers);

		//SRV�ɓo�^

		void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)override;

		//VRAM��̉��z�A�h���X���擾
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress()const
		{
			return m_topLevelASBuffers.pResult->GetGPUVirtualAddress();
		}

		AccelerationStructureBuffers m_topLevelASBuffers;
	};
	
}

