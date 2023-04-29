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

		/// <summary>
	/// ���_�o�b�t�@�̃X�g���C�h(�P�ʁF�o�C�g)���擾�B
	/// </summary>
	/// <remarks>
	/// �X�g���C�h�Ƃ����̂́A�P�v�f�̃T�C�Y�̂��ƁB
	/// �����ł́A��̒��_�̃T�C�Y���擾���邱�ƂɂȂ�B
	/// </remarks>
	/// <returns></returns>
		UINT GetStrideInBytes() const
		{
			return m_vertexBufferView.StrideInBytes;
		}

		const D3D12_VERTEX_BUFFER_VIEW& GetView()
		{

			return m_vertexBufferView;
		};

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;


};

