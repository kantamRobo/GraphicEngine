#pragma once


	class TextureDX11 :public DX11::IShaderResource
	{
		TextureDX11(const wchar_t* filePath)
		{

		}

		
		
		~TextureDX11();
		/// <summary>
			/// ファイルからテクスチャをロードするコンストラクタ
			/// </summary>
			/// <param name="filePath">ロードするテクスチャのファイルパス。</param>
		explicit TextureDX11(const wchar_t* filePath);

		/// <summary>
		/// メモリからテクスチャを初期化する。
		/// </summary>
		/// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス</param>
		/// <param name="size">テクスチャのサイズ。</param>
		void InitFromMemory(const char* memory, unsigned int size);
		void InitFromD3DResource(ComPtr<ID3D11Texture2D> texture);
		/// <summary>
		/// D3Dリソースからテクスチャを初期化する。
		/// </summary>
		/// <param name="resrouce">D3Dリソース。</param>
		/// <summary>
		/// SRVに登録。
		/// </summary>
		/// <param name="descriptorHandle"></param>
		void RegistShaderResourceView(int bufferNo);

		/// <summary>
		/// テクスチャが有効か判定。
		/// </summary>
		/// <returns>trueが返ってきたら有効。</returns>
		bool IsValid() const
		{
			return m_texture != nullptr;
		}

		bool IsValid()const
		{
			return m_texture != nullptr;
		}

	private:
		/// <summary>
		/// DDSファイルからテクスチャをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void LoadTextureFromDDSFile(ID3D11Device* device, const wchar_t* filePath);
		/// <summary>
		/// メモリからテクスチャをロード。
		/// </summary>
		/// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス。</param>
		/// <param name="size">テクスチャのサイズ。</param>
		/// <param name="ge12">Dx12版のグラフィックスエンジン</param>
		/// <param name="device">D3Dデバイス</param>
		void LoadTextureFromMemory(const char* memory, unsigned int size);

		ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

		ComPtr<ID3D11Texture2D> m_texture;


	};

