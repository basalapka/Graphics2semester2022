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

#include "SimpleMath.h"

#endif
#include "DisplayWin32.h"
#include "GameComponent.h"
#include "TriangleComponent.h"
#include "LineComponent.h"

#define ERROR_DEV_SWAPCHAIN 101
#define ERROR_SWAPCHAIN_BUF 102
#define ERROR_RENDER_TARGER 103
#define ERROR_DEPTH_BUF 104
#define ERROR_DEPTH_STENSIL 105

class Game {
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	DisplayWin32 Display;
	D3D11_VIEWPORT* viewport; // ������� ��������
	int numVP; // ���������� ���������
	std::vector <GameComponent*> Components; // ������ ��������� (�������)
	ID3D11DeviceContext* context; // ���������, ���������� �������� �� ��������� ��������� ����������, ����� ��� ����� ��� �������
	// ��� ������ ��������� ����������� ����� ������ ��������� ����������, ������� ������������� ����������.
	IDXGISwapChain* swapChain; // �������� (������� ��������)
	ID3D11RenderTargetView** rtv; // ������� ������ ����������
	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* depthView;
	ID3DUserDefinedAnnotation* annotation; // �������� �������������� �������� � �������� � ������ ���� ����������
	// ��������� ���������� � ������������ ������ ��������� ����� ��� ��������, ����� ����� ������� Direct3D ������������ ������ ��������� ���� ����������
	ID3D11Debug* debug; // ��������� ������� ��������� ����������� ������� � ��������� ��������� ���������
	InputDevice inputDevice;
	std::vector <Camera*> camera;

	std::chrono::time_point<std::chrono::steady_clock> prevTime;
	float deltaTime;
	float totalTime = 0;
	unsigned int frameCount = 0;

	void Initialize();
	int PrepareResources();
	void DestroyResources();
	void PrepareFrame();
	void PrepareFrameViewport(int nVP);
	void EndFrame();
	void Update(int nVP);
	void Draw();
	void ErrorsOutput(int ErrorCode);

	void CreateGrid();
	void CreateCube();
	void CreatePyramid();
	void CreateSphere();
	void CreateCapsule();

public:
	Game();
	void Run();
};
