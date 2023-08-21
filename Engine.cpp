
#include "Shader.h"
#include "stdafx.h"

#include "Engine.h"
#include "GraphicsEngine.h"

Engine::~Engine()
{
}

void Engine::BeginFrame()
{
	g_graphicsEngine->BeginRender();
/*
	for (auto& pad : m_pad)
	{

	}
	*/


}

void Engine::EndFrame()
{
	g_graphicsEngine->EndRender();

}

void Engine::InitEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	
	g_graphicsEngine->InitGraphicsEngine(hwnd, frameBufferWidth, frameBufferHeight);

}
