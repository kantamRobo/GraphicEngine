#pragma once
#include <vector>
#include "RayTracingInstance.h"
namespace raytracing {
	/// <summary>
	/// BLASBuffer
	/// </summary>
	/// <remark>
	/// BLAS( Bottom level acceleration structures )�Ƃ�
	/// ���C�g�����[���h�ɓo�^����Ă���W�I���g���̃f�[�^�\���ł��B
	/// BLAS��3D���f���̃|���S����񂪓o�^����邱�ƂɂȂ�܂��B
	/// </remark
	class BLASBuffer
	{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="instance"></param>

		void InitBLASBuffer(std::shared_ptr<RenderContext> rc, const std::vector<InstancePtr>& instances);


		/// <summary>
			/// BLASBuffer�̃��X�g���擾�B
			/// </summary>
			/// <returns></returns>
		const std::vector< AccelerationStructureBuffers>& Get() const
		{
			return m_bottomLevelASBuffers;
		}
	private:

		std::vector< AccelerationStructureBuffers> m_bottomLevelASBuffers;	//BLASBuffer
	};

}