#pragma once
#include "DirectX11GraphicEngine.h"
#include "DX11RenderContext.h"
#include "DX11Model.h"
#include <memory>
#include <GLTFSDK/GLTFResourceReader.h>
#include "DX11RasterizerState.h"
class Renderer
{
public:

	DirectX11GraphicEngine graphicEngine;
	std::shared_ptr<DX11RenderContext> context;
	DX11Model model;
	std::shared_ptr<DX11RasterizerState> rasterizerState;
	

	void Init(HWND hwnd, const Microsoft::glTF::Document& doc, const std::string& attrName);

	void Tick();


};

