#include "GraphicsEngine.h"

GraphicsEngine::~GraphicsEngine()
{
}

ComPtr<IDXGIFactory4> GraphicsEngine::CreateDXGIFactory()
{
	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	//デバッグコントローラーがあれば、デバッグレイヤーがあるDXGIを作成する。
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();

		// Enable additional debug layers.
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		debugController->Release();
	}
#endif
	ComPtr<IDXGIFactory4> factory;
	CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
	return factory;
}

bool GraphicsEngine::CreateD3DDevice(ComPtr<IDXGIFactory4> dxgiFactory)
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	D3D_FEATURE_LEVEL_12_1,	//Direct3D 12.1の機能を使う。
		D3D_FEATURE_LEVEL_12_0	//Direct3D 12.0の機能を使う。
	};
	ComPtr<IDXGIAdapter> adapterTmp = nullptr;
	ComPtr<IDXGIAdapter> adapterVender[Num_GPUVender] = { nullptr };	//各ベンダーのアダプター。
	ComPtr<IDXGIAdapter> adapterMaxVideoMemory = nullptr;
	ComPtr<IDXGIAdapter> useAdapter = nullptr;
	SIZE_T videoMemorySize = 0;
	for (int i = 0; dxgiFactory->EnumAdapters(i, adapterTmp.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC desc;
		adapterTmp->GetDesc(&desc);

		if (desc.DedicatedVideoMemory > videoMemorySize)
		{
			//こちらのビデオメモリの方が多いので、こちらを使う。
			if (adapterMaxVideoMemory != nullptr) {
				adapterMaxVideoMemory->Release();
			}
			adapterMaxVideoMemory = adapterTmp;
			adapterMaxVideoMemory->AddRef();
			videoMemorySize = desc.DedicatedVideoMemory;


		}
		if (wcsstr(desc.Description, L"NVIDIA") != nullptr) {
			//NVIDIA製
			if (adapterVender[GPU_VenderNvidia]) {
				adapterVender[GPU_VenderNvidia]->Release();
			}
			adapterVender[GPU_VenderNvidia] = adapterTmp;
			adapterVender[GPU_VenderNvidia]->AddRef();
		}
		else if (wcsstr(desc.Description, L"AMD") != nullptr) {
			//AMD製
			if (adapterVender[GPU_VenderAMD]) {
				adapterVender[GPU_VenderAMD]->Release();
			}
			adapterVender[GPU_VenderAMD] = adapterTmp;
			adapterVender[GPU_VenderAMD]->AddRef();
		}
		else if (wcsstr(desc.Description, L"Intel") != nullptr) {
			//Intel製
			if (adapterVender[GPU_VenderIntel]) {
				adapterVender[GPU_VenderIntel]->Release();
			}
			adapterVender[GPU_VenderIntel] = adapterTmp;
			adapterVender[GPU_VenderIntel]->AddRef();
		}
		adapterTmp->Release();
			}
	//使用するアダプターを決める
	if(adapterVender[GPU_VenderNvidia]!= nullptr)
	{
		useAdapter = adapterVender[GPU_VenderNvidia];

	}else if(adapterVender[GPU_VenderAMD] != nullptr)
	{
		useAdapter = adapterVender[GPU_VenderNvidia];
	}
	else if (adapterVender[GPU_VenderAMD] != nullptr) {
		//次はAMDが優先
		useAdapter = adapterVender[GPU_VenderAMD];
	}
	else
	{
		useAdapter = adapterMaxVideoMemory;
	}
	for (auto featureLevel : featureLevels)
	{
		auto hr = D3D12CreateDevice(
			useAdapter.Get(),
			featureLevel,
			IID_PPV_ARGS(m_d3dDevice.GetAddressOf())
		);
		if (SUCCEEDED(hr))
		{
			//D3Dデバイスの作成に成功した
			break;
		}
	}

	for (auto& adapter : adapterVender)
	{
		if (adapter)
		{
			adapter->Release();
		}
	}

	if (adapterMaxVideoMemory) {
		adapterMaxVideoMemory->Release();
	}
	return m_d3dDevice != nullptr;
}

bool GraphicsEngine::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	auto hr = m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr)) {
		//コマンドキューの作成に失敗した。
		return false;
	}
	return true;
}


bool GraphicsEngine::CreateCommandList()
{
	//コマンドリストの作成。
	m_d3dDevice->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)
	);
	if (!m_commandList) {
		return false;
	}
	//コマンドリストは開かれている状態で作成されるので、いったん閉じる。
	m_commandList->Close();
	return true;
}

bool GraphicsEngine::CreateSwapChain(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight, IDXGIFactory4* dxgiFactory)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.BufferCount = FRAME_BUFFER_COUNT;
	swapChainDesc.Width = frameBufferWidth;
	swapChainDesc.Height = frameBufferHeight;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapchain;
	dxgiFactory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		swapchain.GetAddressOf()
	);

	swapchain->QueryInterface(IID_PPV_ARGS(m_swapChain.GetAddressOf()));
	swapchain->Release();

	//IDXGISwapChain3のインターフェースを取得。
	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	return true;
}

bool GraphicsEngine::InitGraphicsEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	m_graphicsEngine = std::make_shared<GraphicsEngine>();

	frameBufferWidth = frameBufferWidth;
	frameBufferHeight = frameBufferHeight;

	auto dxgiFactory = CreateDXGIFactory();


	//コマンドアロケータの作成。
	m_d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_commandAllocator));

	if (!m_commandAllocator) {
		MessageBox(hwnd, TEXT("コマンドアロケータの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
}

void GraphicsEngine::BeginRender()
{
}

void GraphicsEngine::EndRender()
{
}


bool GraphicsEngine::CreateDescriptorHeapForFrameBuffer()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	desc.NumDescriptors = FRAME_BUFFER_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	auto hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
	
	if (FAILED(hr)) {
		//RTV用のディスクリプタヒープの作成に失敗した。
		return false;
	}
	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
	if (FAILED(hr)) {
		//DSV用のディスクリプタヒープの作成に失敗した。
		return false;
	}
	//ディスクリプタのサイズを取得。
	m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	return true;

}
/// <summary>
	/// フレームバッファ用のレンダリングターゲットビューを作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
bool GraphicsEngine::CreateRTVForFrameBuffer()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();


	for (UINT n = 0; n > FRAME_BUFFER_COUNT; n++)
	{
		m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));

		m_d3dDevice->CreateRenderTargetView(
			m_renderTargets[n].Get(), nullptr, rtvHandle
		);
		rtvHandle.ptr += m_rtvDescriptorSize;
	}
	return true;
}


bool GraphicsEngine::CreateDSVForFrameBuffer(UINT frameBufferWidth, UINT frameBufferHeight)
{
	D3D12_CLEAR_VALUE dsvClearValue;
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	dsvClearValue.DepthStencil.Depth = 1.0f;
	dsvClearValue.DepthStencil.Stencil = 0;

	CD3DX12_RESOURCE_DESC desc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		frameBufferWidth,
		frameBufferHeight,
		1,
		1,
		DXGI_FORMAT_D32_FLOAT,
		1,
		0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto hr = m_d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsvClearValue,
		IID_PPV_ARGS(m_depthStencilBuffer.GetAddressOf())
	);
	if (FAILED(hr))
	{
		return false;
	}

	//ディスクリプタを作成
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

	m_d3dDevice->CreateDepthStencilView(
		m_depthStencilBuffer.Get(), nullptr, dsvHandle
	);

	return true;

}


bool GraphicsEngine::CreateSynchronizationWithGPUObject()
{
	m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));

	if (!m_fence) {
		//フェンスの作成に失敗した。
		return false;
	}
	m_fenceValue = 1;
	//同期を行うときのイベントハンドラを作成する。
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr) {
		return false;
	}
	return true;
}