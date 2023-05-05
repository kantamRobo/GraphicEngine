#include "GraphicsEngine.h"

GraphicsEngine::~GraphicsEngine()
{
}

ComPtr<IDXGIFactory4> GraphicsEngine::CreateDXGIFactory()
{
	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	//�f�o�b�O�R���g���[���[������΁A�f�o�b�O���C���[������DXGI���쐬����B
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
	D3D_FEATURE_LEVEL_12_1,	//Direct3D 12.1�̋@�\���g���B
		D3D_FEATURE_LEVEL_12_0	//Direct3D 12.0�̋@�\���g���B
	};
	ComPtr<IDXGIAdapter> adapterTmp = nullptr;
	ComPtr<IDXGIAdapter> adapterVender[Num_GPUVender] = { nullptr };	//�e�x���_�[�̃A�_�v�^�[�B
	ComPtr<IDXGIAdapter> adapterMaxVideoMemory = nullptr;
	ComPtr<IDXGIAdapter> useAdapter = nullptr;
	SIZE_T videoMemorySize = 0;
	for (int i = 0; dxgiFactory->EnumAdapters(i, adapterTmp.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC desc;
		adapterTmp->GetDesc(&desc);

		if (desc.DedicatedVideoMemory > videoMemorySize)
		{
			//������̃r�f�I�������̕��������̂ŁA��������g���B
			if (adapterMaxVideoMemory != nullptr) {
				adapterMaxVideoMemory->Release();
			}
			adapterMaxVideoMemory = adapterTmp;
			adapterMaxVideoMemory->AddRef();
			videoMemorySize = desc.DedicatedVideoMemory;


		}
		if (wcsstr(desc.Description, L"NVIDIA") != nullptr) {
			//NVIDIA��
			if (adapterVender[GPU_VenderNvidia]) {
				adapterVender[GPU_VenderNvidia]->Release();
			}
			adapterVender[GPU_VenderNvidia] = adapterTmp;
			adapterVender[GPU_VenderNvidia]->AddRef();
		}
		else if (wcsstr(desc.Description, L"AMD") != nullptr) {
			//AMD��
			if (adapterVender[GPU_VenderAMD]) {
				adapterVender[GPU_VenderAMD]->Release();
			}
			adapterVender[GPU_VenderAMD] = adapterTmp;
			adapterVender[GPU_VenderAMD]->AddRef();
		}
		else if (wcsstr(desc.Description, L"Intel") != nullptr) {
			//Intel��
			if (adapterVender[GPU_VenderIntel]) {
				adapterVender[GPU_VenderIntel]->Release();
			}
			adapterVender[GPU_VenderIntel] = adapterTmp;
			adapterVender[GPU_VenderIntel]->AddRef();
		}
		adapterTmp->Release();
			}
	//�g�p����A�_�v�^�[�����߂�
	if(adapterVender[GPU_VenderNvidia]!= nullptr)
	{
		useAdapter = adapterVender[GPU_VenderNvidia];

	}else if(adapterVender[GPU_VenderAMD] != nullptr)
	{
		useAdapter = adapterVender[GPU_VenderNvidia];
	}
	else if (adapterVender[GPU_VenderAMD] != nullptr) {
		//����AMD���D��
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
			//D3D�f�o�C�X�̍쐬�ɐ�������
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
		//�R�}���h�L���[�̍쐬�Ɏ��s�����B
		return false;
	}
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

	//IDXGISwapChain3�̃C���^�[�t�F�[�X���擾�B
	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	return true;
}

bool GraphicsEngine::InitGraphicsEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	m_graphicsEngine = std::make_shared<GraphicsEngine>();

	frameBufferWidth = frameBufferWidth;
	frameBufferHeight = frameBufferHeight;

	auto dxgiFactory = CreateDXGIFactory();
}

void GraphicsEngine::BeginRender()
{
}

void GraphicsEngine::EndRender()
{
}

bool GraphicsEngine::CreateDescriptorHeapForFrameBuffer()
{
	return false;


void GraphicsEngine::ChangeRenderTargetToFrameBuffer(std::shared_ptr<RenderContext> rc)
{
}
