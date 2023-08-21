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
#include <wrl.h>

#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "d3dx12.h"

const int MAX_RENDERING_TARGET = D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT;	//�����_�����O�^�[�Q�b�g�̍ő吔�B

using namespace Microsoft::WRL;



const UINT FRAME_BUFFER_W = 1280;				//�t���[���o�b�t�@�̕��B
const UINT FRAME_BUFFER_H = 720;				//�t���[���o�b�t�@�̍����B
static const int MAX_BONE = 512;	//!<�{�[���̍ő吔�B