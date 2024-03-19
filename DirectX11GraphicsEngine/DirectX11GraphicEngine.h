#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_4.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

class DirectX11GraphicEngine
{

public:
    Microsoft::WRL::ComPtr<ID3D11Device> m_device = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
    Microsoft::WRL::ComPtr<IDXGIFactory2> m_dxgiFactory = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBufferView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthstencilView;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_depthstencilbuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthstencilstate;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthmapbuffer;
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    D3D11_VIEWPORT m_viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
    UINT width = {};
    UINT height = {};
    HRESULT CreateDevice();
    HRESULT CreateSwapChain(HWND hwnd);
    HRESULT CreateRTV(UINT width, UINT height);
   
  
};

