#pragma once
#pragma comment( lib, "xinput.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <map>
#include <functional>
#include <algorithm>
#include <DirectXMath.h>
#include <Xinput.h>
#include <array>
#include <dxcapi.h>
#include <atlbase.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <dxgiformat.h>
#include <wrl/client.h>
#include "d3dx12.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "GraphicsEngine.h"
#include <atlbase.h>
#include "Raytracing.h"
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

