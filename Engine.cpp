#include "Engine.h"
#include "stdafx.h"
#include "TResourceBank.h"

Engine::~Engine()
{
	if (m_graphicsEngine)
	{
		m_graphicsEngine.release();
	}
}

void Engine::BeginFrame()
{
	m_graphicsEngine->BeginRender();

}

void Engine::EndFrame()
{
}

void Engine::InitEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	//グラフィックエンジンの初期化
	m_graphicsEngine = std::make_unique<GraphicsEngine>();
	m_graphicsEngine->InitGraphicsEngine(hwnd, frameBufferWidth, frameBufferHeight);

}
