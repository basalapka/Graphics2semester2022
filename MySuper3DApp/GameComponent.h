#pragma once

#ifndef LIBRARIES
#define LIBRARIES

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinUser.h>
#include <Wincodec.h>
#include <wrl.h>
#include <vector>
#include <chrono>
#include <iostream>

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#endif

#include "DisplayWin32.h"
//#include "Camera.h"

#define NOTHING_TO_DRAW -1
#define SUCCESS 0
#define ERROR_VERTEX_BC 1
#define MISSING_VERTEX_SHADER 2
#define ERROR_PIXEL_BC 3
#define MISSING_PIXEL_SHADER 4
#define ERROR_CREATING_VERTEX_SHADER 5
#define ERROR_CREATING_PIXEL_SHADER 6
#define ERROR_CREATING_COM_OBJ 7
#define ERROR_CREATING_LAYOUT 8
#define ERROR_CREATING_POSBUF 9
#define ERROR_CREATING_COLBUF 10
#define ERROR_CREATING_TEXBUF 11
#define ERROR_CREATING_NORMBUF 12
#define ERROR_CREATING_INDBUF 13
#define ERROR_CREATING_BLENDSTATE 14
#define ERROR_CREATING_CONSTBUF 15
#define ERROR_CREATING_LIGHTTBUF 16
#define ERROR_CREATING_RASTSTATE 17
#define ERROR_CREATING_DECODER 18
#define ERROR_GET_FRAME 19
#define ERROR_CREATING_CONVERTER 20
#define ERROR_INITIALIZE_CONVERTER 21
#define ERROR_COPY_PIXELS 22
#define ERROR_CREATING_TEXTURE 23
#define ERROR_CREATING_SHADER_RV 24
#define ERROR_RESOURCES_NOT_PREPARED 25
#define ERROR_TEXTURE_FILE_MISSING 26
#define ERROR_LOAD_TEXTURE_ORDER 27
#define ERROR_CREATING_SAMPLER_STATE 28

using namespace DirectX;

class GameComponent {
public:
	virtual int PrepareResourses(Microsoft::WRL::ComPtr<ID3D11Device> device) { return 0; };
	virtual int LoadTextureFromFile(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context, bool generateMips, bool useSrgb, UINT frameIndex) { return 0; };
	virtual void DestroyResourses() {};
	//virtual void Update(ID3D11DeviceContext* context, Camera* camera) {};
	virtual void Draw(ID3D11DeviceContext* context) {};
};

