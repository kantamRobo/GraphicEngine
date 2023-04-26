#pragma once

class VertexBuffer
{
public:
		~VertexBuffer();

	//������
		//stride�Ƃ͈�̒��_�̃T�C�Y���w��
		void InitVertexBuffer(int size, int stride);

		void Copy(void* srcVertices);

		UINT GetSizeInBytes()const
		{
			return m_vertexBufferView.SizeInBytes;
		}

		const D3D12_VERTEX_BUFFER_VIEW& GetView()
		{

			return m_vertexBufferView;
		};

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;


};

