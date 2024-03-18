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
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    UINT width = {};
    UINT height = {};
    HRESULT CreateDevice();
    HRESULT CreateSwapChain(HWND hwnd);
    HRESULT CreateRTV();
  
};

