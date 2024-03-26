#include "DirectX11GraphicEngine.h"
#include <GLFW/glfw3.h>
#include <iostream>

//guwaa
HRESULT DirectX11GraphicEngine::CreateDevice()
{
  
    m_viewport.Width = width;
    m_viewport.Height = height;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;  // 上端をウィンドウの上端に合わせます
    m_viewport.MinDepth = 0.0f;  // 標準的な深度範囲の最小値
    m_viewport.MaxDepth = 0.5f;  // 標準的な深度範囲の最大値
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


    //ファクトリーの作成
    // 
    // https://learn.microsoft.com/ja-jp/windows/win32/api/dxgi/nf-dxgi-createdxgifactory
    // 
    // スワップチェインの作成

    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory));
    if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, m_swapChain.GetAddressOf())))
    {
        return E_FAIL;
    }
    

    

    return S_OK;
}
//https://github.com/RikutoNarita/20221003_ss/blob/2cc0fc45f3f09473abfc0ae8a4b4c88afe4a98b5/Library/GraphicsSystem/DirectX/Gfx_D3DManager.cpp#L143

//たたき台。DirectX11の理解度が深まるにつれて変えていく。
HRESULT DirectX11GraphicEngine::CreateRTV()
{
//レンダーターゲットビュー生成


    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &m_BackBuffer);
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    auto hr =m_device->CreateRenderTargetView(m_BackBuffer.Get(),&rtvDesc , m_backBufferView.GetAddressOf());
    
    if (FAILED(hr))
    {
        return hr;
    }
    //Zバッファ用テクスチャ生成

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

    //Zバッファターゲットビュー生成

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));
    dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = m_device->CreateDepthStencilView(m_backbufferTexture.Get(), &dsvd, &m_DepthstencilView);
    if (FAILED(hr)) { return hr; }
    //バックバッファをRTとしてセット
    m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), m_DepthstencilView.Get());


    //! ビューポートの設定
   
    m_deviceContext->RSSetViewports(1, &m_viewport);

    return hr;

}
