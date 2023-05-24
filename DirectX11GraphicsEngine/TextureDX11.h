#pragma once


	class TextureDX11 :public DX11::IShaderResource
	{
		TextureDX11(const wchar_t* filePath)
		{

		}

		
		
		~TextureDX11();
		/// <summary>
			/// �t�@�C������e�N�X�`�������[�h����R���X�g���N�^
			/// </summary>
			/// <param name="filePath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
		explicit TextureDX11(const wchar_t* filePath);

		/// <summary>
		/// ����������e�N�X�`��������������B
		/// </summary>
		/// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X</param>
		/// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
		void InitFromMemory(const char* memory, unsigned int size);
		void InitFromD3DResource(ComPtr<ID3D11Texture2D> texture);
		/// <summary>
		/// D3D���\�[�X����e�N�X�`��������������B
		/// </summary>
		/// <param name="resrouce">D3D���\�[�X�B</param>
		/// <summary>
		/// SRV�ɓo�^�B
		/// </summary>
		/// <param name="descriptorHandle"></param>
		void RegistShaderResourceView(int bufferNo);

		/// <summary>
		/// �e�N�X�`�����L��������B
		/// </summary>
		/// <returns>true���Ԃ��Ă�����L���B</returns>
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
		/// DDS�t�@�C������e�N�X�`�������[�h�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void LoadTextureFromDDSFile(ID3D11Device* device, const wchar_t* filePath);
		/// <summary>
		/// ����������e�N�X�`�������[�h�B
		/// </summary>
		/// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X�B</param>
		/// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
		/// <param name="ge12">Dx12�ł̃O���t�B�b�N�X�G���W��</param>
		/// <param name="device">D3D�f�o�C�X</param>
		void LoadTextureFromMemory(const char* memory, unsigned int size);

		ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

		ComPtr<ID3D11Texture2D> m_texture;


	};

