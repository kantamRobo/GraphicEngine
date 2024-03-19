#pragma once
#include <d3d11.h>
#include "DirectX11GraphicEngine.h"
#include "DX11RenderContext.h"
#include "DX11Model.h"
#include <memory>
#include <GLTFSDK/GLTFResourceReader.h>
#include <DirectXMath.h>
#include "DX11RasterizerState.h"
#include "DX11Shader.h"
#include "DX11ConstantBuffer.h"
class Renderer{
public:

	DirectX11GraphicEngine graphicEngine;
	SceneConstant temp_const;
	std::shared_ptr<DX11RenderContext> context;
	DX11Model model;
	DX11ConstantBuffer m_constbuffer;
	std::shared_ptr<DX11RasterizerState> rasterizerState;
	std::shared_ptr<DX11Shader> temp_shader;

	void Init(HWND hwnd, const Microsoft::glTF::Document& doc, const std::string& attrName);

	void Tick();


};

