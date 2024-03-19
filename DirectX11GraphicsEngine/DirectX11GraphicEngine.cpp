#include "DirectX11GraphicEngine.h"
#include <GLFW/glfw3.h>
#include <iostream>
HRESULT DirectX11GraphicEngine::CreateDevice()
{

    //=====================================================
//デバイス生成(主にリソース作成時に使用するオブジェクト)
//=====================================================
    UINT creationFlags = 0;
#ifdef _DEBUG
    // DEBUGビルド時はDirect3Dのデバッグを有効にする
    // (すごく重いが細かいエラーがわかる)
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
     // スワップチェイン作成(フロントバッファに表示可能なバックバッファを持つもの)
     //=====================================================		// スワップチェーンの設定データ
    scDesc.BufferDesc.Width = width;						// 画面の幅
    scDesc.BufferDesc.Height = height;						// 画面の高さ
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バッファの形式
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.SampleDesc.Count = 1;							// MSAAは使用しない
    scDesc.SampleDesc.Quality = 0;							// MSAAは使用しない
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バッファの使用方法
    scDesc.BufferCount = 2;									// バッファの数
    scDesc.OutputWindow = hwnd;
    scDesc.Windowed = TRUE;									// ウィンドウモード
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // スワップチェインの作成
    if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, &m_swapChain)))
    {
        return false;
    }
    

    

    return S_OK;
}

HRESULT DirectX11GraphicEngine::CreateRTV(UINT width,UINT height)
{

    // スワップチェインからバックバッファリソース取得
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
    {
        return false;
    }
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    //深度値に24bitのfloat型をステンシル値に8ビットのuintを確保している
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.SampleDesc.Count = 1;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
     auto hr = this->m_device->CreateTexture2D(&desc, nullptr, this->m_depthmapbuffer.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("深度ステンシルバッファの作成に失敗");
    }
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = desc.Format;
    dsvDesc.Texture2D.MipSlice = 0;
    this->m_device->CreateDepthStencilView(this->m_depthmapbuffer.Get(), &dsvDesc, this->m_DepthstencilView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("深度ステンシルビュー作成に失敗");
    }

    // Scene::onInit関数の一部
//深度テストとステンシルテストを別々で作っていますが、もちろん同時に行うこともできます。
    D3D11_DEPTH_STENCIL_DESC ds = {};
    //深度テストを行う深度ステンシルステートの作成
    ds.DepthEnable = true;
    ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    ds.StencilEnable = false;
    auto dshr = this->m_device->CreateDepthStencilState(&ds, this->m_depthstencilstate.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("深度テスト用のステート作成に失敗");
    }
    
    //ステンシルテストを行う深度ステンシルステートの作成
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
        throw std::runtime_error("ステンシルテスト用のステート作成に失敗");
    }
    
    // バックバッファリソース用のRTVを作成
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    if (FAILED(m_device->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &m_backBufferView)))
    {
        return false;
    }
    m_device->CreateDepthStencilView(m_depthstencilbuffer.Get(), &dsvDesc, m_DepthstencilView.GetAddressOf());
    //=====================================================
    // デバイスコンテキストに描画に関する設定を行っておく
    //=====================================================

    // バックバッファをRTとしてセット
    m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

    // ビューポートの設定

    m_deviceContext->RSSetViewports(1, &m_viewport);
    return S_OK;
}
