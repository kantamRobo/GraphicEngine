#include "GraphicsEngine.h"
#include  "Camera.h"
#include "Raytracing.h"
Camera* g_camera3D;
GraphicsEngine::~GraphicsEngine()
{

	if (m_commandQueue)
	{
		m_commandQueue->Release();
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
	}

	if (m_rtvHeap)
	{
		m_rtvHeap->Release();
	}

	if (m_commandAllocator)
	{
		m_commandAllocator->Release();
	}

	if (m_commandList)
	{
		m_commandList->Release();
	}

	if (m_pipelineState)
	{
		m_pipelineState->Release();
	}
	for (auto& rt : m_renderTargets)
	{
		if (rt)
		{
			rt->Release();
		}
	}
	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
	}
	if (m_fence)
	{
		m_fence->Release();
	}

	if (m_d3dDevice)
	{
		m_d3dDevice->Release();
	}

	if (g_camera3D)
	{
		delete g_camera3D;
	}
	CloseHandle(m_fenceEvent);

}

void GraphicsEngine::WaitDraw()
{
	//�`��I���҂�
	const UINT64 fence = m_fenceValue;
	m_commandQueue->Signal(m_fence.Get(), fence);
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < fence)
	{
		m_fence->SetEventOnCompletion(fence, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
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


bool GraphicsEngine::InitGraphicsEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	m_graphicsEngine = this;

	frameBufferWidth = frameBufferWidth;
	frameBufferHeight = frameBufferHeight;
	//�f�o�C�X�ɃA�N�Z�X���邽�߂̃C���^�[�t�F�[�X���쐬�B
	auto dxgiFactory = CreateDXGIFactory();

	if (!CreateD3DDevice(dxgiFactory))
	{
		//D3D�f�o�C�X�̍쐬�Ɏ��s����
		MessageBox(hwnd, TEXT("D3D�f�o�C�X�̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);

		return false;
	}


	//�R�}���h�A���P�[�^�̍쐬�B
	m_d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_commandAllocator));

	if (!m_commandAllocator) {
		MessageBox(hwnd, TEXT("�R�}���h�A���P�[�^�̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);
		return false;
	}



	//�R�}���h�L���[�̍쐬
	if (!CreateCommandQueue())
	{
		//�R�}���h�L���[�쐬�Ɏ��s����
		MessageBox(hwnd, TEXT("�R�}���h�L���[�̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);
		return false;
	}

	//�X���b�v�`�F�C�����쐬
	if (!CreateSwapChain(hwnd, frameBufferWidth, frameBufferHeight,
		dxgiFactory.Get()))
	{
		//�X���b�v�`�F�C���̍쐬�Ɏ��s�B
		MessageBox(hwnd, TEXT("�X���b�v�`�F�C���̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);
		return false;
	}

	//�t���[���o�b�t�@�p�̃f�B�X�N���v�^�q�[�v���쐬����B
	if (!CreateDescriptorHeapForFrameBuffer()) {
		MessageBox(hwnd, TEXT("�t���[���o�b�t�@�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);
		return false;
	}

	//�t���[���o�b�t�@�p��RTV���쐬����B
	if (!CreateRTVForFrameBuffer()) {
		MessageBox(hwnd, TEXT("�t���[���o�b�t�@�p��RTV�̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);
		return false;

	}

	//�t���[���o�b�t�@�p��DSV���쐬����B
	if (!CreateDSVForFrameBuffer(frameBufferWidth, frameBufferHeight)) {
		MessageBox(hwnd, TEXT("�t���[���o�b�t�@�p��DSV�̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);
		return false;
	}

	//�����_�����O�R���e�L�X�g�̍쐬
	m_renderContext->InitRenderingContext(m_commandList);

	//�r���[�|�[�g���������B
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<FLOAT>(frameBufferWidth);
	m_viewport.Height = static_cast<FLOAT>(frameBufferHeight);
	m_viewport.MinDepth = D3D12_MIN_DEPTH;
	m_viewport.MaxDepth = D3D12_MAX_DEPTH;

	//�V�U�����O��`���������B
	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = frameBufferWidth;
	m_scissorRect.bottom = frameBufferHeight;

	//CBR_SVR�̃f�B�X�N���v�^�̃T�C�Y���擾�B
	m_cbrSrvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//Sampler�̃f�B�X�N���v�^�̃T�C�Y���擾�B
	m_samplerDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	//���������I������̂�DXGI��j���B
	dxgiFactory->Release();

	//�k���e�N�X�`����������
	m_nullTextureMaps->Init();

	m_camera3D.SetPosition({ 0.0f, 50.0f, 200.0f });
	m_camera3D.SetTarget({ 0.0f, 50.0f, 0.0f });

	g_camera3D = &m_camera3D;
	//DirectXTK�p�̃O���t�B�b�N�������Ǘ��N���X�̃C���X�^���X���쐬����B
	m_directXTKGfxMemory = std::make_unique<DirectX::GraphicsMemory>(m_d3dDevice);



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
	if (adapterVender[GPU_VenderNvidia] != nullptr)
	{
		useAdapter = adapterVender[GPU_VenderNvidia];

	}
	else if (adapterVender[GPU_VenderAMD] != nullptr)
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


bool GraphicsEngine::CreateCommandList()
{
	//�R�}���h���X�g�̍쐬�B
	m_d3dDevice->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)
	);
	if (!m_commandList) {
		return false;
	}
	//�R�}���h���X�g�͊J����Ă����Ԃō쐬�����̂ŁA�����������B
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

	//IDXGISwapChain3�̃C���^�[�t�F�[�X���擾�B
	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	return true;
}

bool GraphicsEngine::CreateDescriptorHeapForFrameBuffer()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	desc.NumDescriptors = FRAME_BUFFER_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	auto hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));

	if (FAILED(hr)) {
		//RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
		return false;
	}
	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
	if (FAILED(hr)) {
		//DSV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
		return false;
	}
	//�f�B�X�N���v�^�̃T�C�Y���擾�B
	m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	return true;

}




/// <summary>
	/// �t���[���o�b�t�@�p�̃����_�����O�^�[�Q�b�g�r���[���쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
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

	//�f�B�X�N���v�^���쐬
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
		//�t�F���X�̍쐬�Ɏ��s�����B
		return false;
	}
	m_fenceValue = 1;
	//�������s���Ƃ��̃C�x���g�n���h�����쐬����B
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr) {
		return false;
	}
	return true;
}

void GraphicsEngine::BeginRender()
{
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	//�J�������X�V����B
	m_camera3D.Update();

	//�R�}���h�A���P�[�^�[�����Z�b�g
	m_commandAllocator->Reset();

	//�����_�����O�R���e�L�X�g�����Z�b�g
	m_renderContext->Reset(m_commandAllocator.Get(), m_pipelineState.Get());
	//�r���[�|�[�g��ݒ�B
	m_renderContext->SetViewportAndScissor(m_viewport);

	m_currentFrameBufferRTVHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

	m_currentFrameBufferRTVHandle.ptr += m_frameIndex * m_rtvDescriptorSize;

	//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�A�h���X���擾�B
	m_currentFrameBufferDSVHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//�o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ�\�ɂȂ�܂ő҂B
	m_renderContext->WaitUntilToPossibleSetRenderTarget(m_renderTargets[m_frameIndex].Get());

	//�����_�����O�^�[�Q�b�g��ݒ�B
	m_renderContext->SetRenderTarget(m_currentFrameBufferRTVHandle, m_currentFrameBufferDSVHandle);

	const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_renderContext->ClearRenderTargetView(m_currentFrameBufferRTVHandle, clearColor);
	m_renderContext->ClearDepthStencilView(m_currentFrameBufferDSVHandle, 1.0f);




}

void GraphicsEngine::EndRender()
{
	// �����_�����O�^�[�Q�b�g�ւ̕`�����݊����҂�
	m_renderContext->WaitUntilFinishDrawingToRenderTarget(m_renderTargets[m_frameIndex].Get());


	m_directXTKGfxMemory->Commit(m_commandQueue.Get());

	//�����_�����O�R���e�L�X�g�����B
	m_renderContext->Close();

	//�R�}���h�����s�B
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get()};
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
#if defined( SAMPE_16_04 ) || defined(APPEND_04)
	// Present the frame.
	m_swapChain->Present(0, 0);
#else
	// Present the frame.
	m_swapChain->Present(1, 0);
#endif
	m_directXTKGfxMemory->GarbageCollect();
	//�`�抮���҂��B
	WaitDraw();
}
