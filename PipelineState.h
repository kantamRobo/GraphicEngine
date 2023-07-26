#pragma once

class PipelineState
{
public:
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

	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetP()
	{
		return m_pipelineState;
	}

private:

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState = nullptr;

	
	
};

