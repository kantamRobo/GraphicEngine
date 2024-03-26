#include "DirectX11GraphicEngine.h"
#include <GLFW/glfw3.h>
#include <iostream>

//guwaa
HRESULT DirectX11GraphicEngine::CreateDevice()
{
  
    m_viewport.Width = width;
    m_viewport.Height = height;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;  // ��[���E�B���h�E�̏�[�ɍ��킹�܂�
    m_viewport.MinDepth = 0.0f;  // �W���I�Ȑ[�x�͈͂̍ŏ��l
    m_viewport.MaxDepth = 0.5f;  // �W���I�Ȑ[�x�͈͂̍ő�l
    //=====================================================
//�f�o�C�X����(��Ƀ��\�[�X�쐬���Ɏg�p����I�u�W�F�N�g)
//=====================================================
    UINT creationFlags = 0;
#ifdef _DEBUG
    // DEBUG�r���h����Direct3D�̃f�o�b�O��L���ɂ���
    // (�������d�����ׂ����G���[���킩��)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
        D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
        D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
        D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
        D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
        D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
        D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
    };
    D3D_FEATURE_LEVEL futureLevel = {};
    if (FAILED(D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        &m_device,
        &futureLevel,
        &m_deviceContext)))
    {
        return E_FAIL;
    }


    return S_OK;
}

HRESULT DirectX11GraphicEngine::CreateSwapChain(HWND hwnd)
{
    //=====================================================
     // �X���b�v�`�F�C���쐬(�t�����g�o�b�t�@�ɕ\���\�ȃo�b�N�o�b�t�@��������)
     //=====================================================		// �X���b�v�`�F�[���̐ݒ�f�[�^
    scDesc.BufferDesc.Width = width;						// ��ʂ̕�
    scDesc.BufferDesc.Height = height;						// ��ʂ̍���
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�t�@�̌`��
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.SampleDesc.Count = 1;							// MSAA�͎g�p���Ȃ�
    scDesc.SampleDesc.Quality = 0;							// MSAA�͎g�p���Ȃ�
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�t�@�̎g�p���@
    scDesc.BufferCount = 2;									// �o�b�t�@�̐�
    scDesc.OutputWindow = hwnd;
    scDesc.Windowed = TRUE;									// �E�B���h�E���[�h
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


    //�t�@�N�g���[�̍쐬
    // 
    // https://learn.microsoft.com/ja-jp/windows/win32/api/dxgi/nf-dxgi-createdxgifactory
    // 
    // �X���b�v�`�F�C���̍쐬

    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory));
    if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, m_swapChain.GetAddressOf())))
    {
        return E_FAIL;
    }
    

    

    return S_OK;
}
//https://github.com/RikutoNarita/20221003_ss/blob/2cc0fc45f3f09473abfc0ae8a4b4c88afe4a98b5/Library/GraphicsSystem/DirectX/Gfx_D3DManager.cpp#L143

//��������BDirectX11�̗���x���[�܂�ɂ�ĕς��Ă����B
HRESULT DirectX11GraphicEngine::CreateRTV()
{
//�����_�[�^�[�Q�b�g�r���[����


    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &m_BackBuffer);
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    auto hr =m_device->CreateRenderTargetView(m_BackBuffer.Get(),&rtvDesc , m_backBufferView.GetAddressOf());
    
    if (FAILED(hr))
    {
        return hr;
    }
    //Z�o�b�t�@�p�e�N�X�`������

    D3D11_TEXTURE2D_DESC texturedesc = {};

    texturedesc.Width = width;
    texturedesc.Height = height;
    texturedesc.MipLevels = 1;
    texturedesc.ArraySize = 1;
    texturedesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texturedesc.SampleDesc.Count = 1;
    texturedesc.SampleDesc.Quality = 0;
    texturedesc.Usage = D3D11_USAGE_DEFAULT;
    texturedesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_device->CreateTexture2D(&texturedesc, nullptr, &m_backbufferTexture);
    if (FAILED(hr)) { return hr; }

    //Z�o�b�t�@�^�[�Q�b�g�r���[����

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));
    dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = m_device->CreateDepthStencilView(m_backbufferTexture.Get(), &dsvd, &m_DepthstencilView);
    if (FAILED(hr)) { return hr; }
    //�o�b�N�o�b�t�@��RT�Ƃ��ăZ�b�g
    m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), m_DepthstencilView.Get());


    //! �r���[�|�[�g�̐ݒ�
   
    m_deviceContext->RSSetViewports(1, &m_viewport);

    return hr;

}
