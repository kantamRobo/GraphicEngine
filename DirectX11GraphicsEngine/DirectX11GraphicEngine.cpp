#include "DirectX11GraphicEngine.h"
#include <GLFW/glfw3.h>
#include <iostream>
HRESULT DirectX11GraphicEngine::CreateDevice()
{

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

    // �X���b�v�`�F�C���̍쐬
    if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, &m_swapChain)))
    {
        return false;
    }
    

    

    return S_OK;
}

HRESULT DirectX11GraphicEngine::CreateRTV(UINT width,UINT height)
{

    // �X���b�v�`�F�C������o�b�N�o�b�t�@���\�[�X�擾
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
    {
        return false;
    }
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    //�[�x�l��24bit��float�^���X�e���V���l��8�r�b�g��uint���m�ۂ��Ă���
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.SampleDesc.Count = 1;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
     auto hr = this->m_device->CreateTexture2D(&desc, nullptr, this->m_depthmapbuffer.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("�[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s");
    }
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = desc.Format;
    dsvDesc.Texture2D.MipSlice = 0;
    this->m_device->CreateDepthStencilView(this->m_depthmapbuffer.Get(), &dsvDesc, this->m_DepthstencilView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("�[�x�X�e���V���r���[�쐬�Ɏ��s");
    }

    // Scene::onInit�֐��̈ꕔ
//�[�x�e�X�g�ƃX�e���V���e�X�g��ʁX�ō���Ă��܂����A������񓯎��ɍs�����Ƃ��ł��܂��B
    D3D11_DEPTH_STENCIL_DESC ds = {};
    //�[�x�e�X�g���s���[�x�X�e���V���X�e�[�g�̍쐬
    ds.DepthEnable = true;
    ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    ds.StencilEnable = false;
    auto dshr = this->m_device->CreateDepthStencilState(&ds, this->m_depthstencilstate.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("�[�x�e�X�g�p�̃X�e�[�g�쐬�Ɏ��s");
    }
    
    //�X�e���V���e�X�g���s���[�x�X�e���V���X�e�[�g�̍쐬
    ds.DepthEnable = false;
    ds.StencilEnable = true;
    ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    ds.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;

    ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    ds.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;

     this->m_device->CreateDepthStencilState(&ds, this->m_depthstencilstate.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("�X�e���V���e�X�g�p�̃X�e�[�g�쐬�Ɏ��s");
    }
    
    // �o�b�N�o�b�t�@���\�[�X�p��RTV���쐬
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    if (FAILED(m_device->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &m_backBufferView)))
    {
        return false;
    }
    m_device->CreateDepthStencilView(m_depthstencilbuffer.Get(), &dsvDesc, m_DepthstencilView.GetAddressOf());
    //=====================================================
    // �f�o�C�X�R���e�L�X�g�ɕ`��Ɋւ���ݒ���s���Ă���
    //=====================================================

    // �o�b�N�o�b�t�@��RT�Ƃ��ăZ�b�g
    m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

    // �r���[�|�[�g�̐ݒ�

    m_deviceContext->RSSetViewports(1, &m_viewport);
    return S_OK;
}
