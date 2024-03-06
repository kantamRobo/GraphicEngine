#pragma once
#include <d3d11.h>
#include <wrl.h>
class DX11StructuredBuffer
{

public:
	~DX11StructuredBuffer();

	//�\�����o�b�t�@��������
	void InitStructuredBuffer(ID3D11Device* device, int sizeOfElement, int numElement, void* initDatap);


	//SRV�ɓo�^


	/// <summary>
	/// SRV�ɓo�^�B
	/// </summary>
	/// <param name="descriptorHandle"></param>
	//TODO
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetD3DResources()
	{
		return m_buffersOnGPU[0];
	};
	/// <summary>
	/// �\�����o�b�t�@�̓��e���X�V�B
	/// </summary>
	/// <param name="data"></param>
	void Update(void* data);
	/// <summary>
	/// ����������Ă��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffersOnGPU[2] = { nullptr };
	void* m_buffersOnCPU[2] = { nullptr };
	int m_numElement = 0;
	int m_sizeOfElement = 0;
	bool m_isInited = false;
	void InitStructuredBuffer(ID3D11Device* device, int sizeOfElement, int numElement, void* initData);
	void RegistShaderResourceView(ID3D11Device* device, int bufferNo);
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView> m_SRVbuffer;
};

