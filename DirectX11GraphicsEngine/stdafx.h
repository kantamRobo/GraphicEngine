#pragma once

#include <d3d11.h>
#include <wrl.h>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;