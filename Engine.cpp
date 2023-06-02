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
/*
	for (auto& pad : m_pad)
	{

	}
	*/


}

void Engine::EndFrame()
{
	m_graphicsEngine->EndRender();

}

void Engine::InitEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	//グラフィックエンジンの初期化
	m_graphicsEngine = std::make_unique<GraphicsEngine>();
	m_graphicsEngine->InitGraphicsEngine(hwnd, frameBufferWidth, frameBufferHeight);

}
