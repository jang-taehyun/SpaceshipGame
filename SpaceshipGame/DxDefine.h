#pragma once


// DirectX 11 library 링크 //
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")


#define DIRECTINPUT_VERSION 0x0800			// direct input의 version 설정
											// direct input의 version을 설정하지 않으면 기본값인 8로 설정됨


// DirectX 11 header 포함 //
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <dinput.h>
#include <dsound.h>

#include <DirectXTex.h>