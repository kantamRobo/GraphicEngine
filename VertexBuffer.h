#pragma once

class VertexBuffer
{
public:
		~VertexBuffer();

	//初期化
		//strideとは一個の頂点のサイズを指す
		void InitVertexBuffer(int size, int stride);

		void Copy(void* srcVertices);

		UINT GetSizeInBytes()const
		{
			return m_vertexBufferView.SizeInBytes;
		}

		/// <summary>
	/// 頂点バッファのストライド(単位：バイト)を取得。
	/// </summary>
	/// <remarks>
	/// ストライドというのは、１要素のサイズのこと。
	/// ここでは、一つの頂点のサイズを取得することになる。
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

