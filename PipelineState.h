#pragma once
#include "stdafx.h"
class PipelineState
{
	~PipelineState()
	{
		if (m_pipelineState)
		{
			m_pipelineState->Release();
		}


	}

	//èâä˙âª

	void InitGraphicPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);

	void InitComputePipelineState(D3D12_COMPUTE_PIPELINE_STATE_DESC desc);

	ComPtr<ID3D12PipelineState> Get()
	{
		return m_pipelineState;
	}

private:

	ComPtr<ID3D12PipelineState> m_pipelineState = nullptr;

	
	
};

