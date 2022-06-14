#pragma once

#include "include.h"
#include "DisplayWin32.h"
#include "GameComponent.h"
#include "TriangleComponent.h"

class Game {
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	DisplayWin32 DW;
	D3D11_VIEWPORT viewport; // ������� ��������
	int numVP; // ���������� ���������
	ID3D11DeviceContext* context; // ���������, ���������� �������� �� ��������� ��������� ����������, ����� ��� ����� ��� �������
	// ��� ������ ��������� ����������� ����� ������ ��������� ����������, ������� ������������� ����������.
	IDXGISwapChain* swapChain; // �������� (������� ��������)
	ID3D11RenderTargetView* rtv; // ������� ������ ����������
	// ��������� ���������� � ������������ ������ ��������� ����� ��� ��������, ����� ����� ������� Direct3D ������������ ������ ��������� ���� ����������
	ID3D11Debug* debug; // ��������� ������� ��������� ����������� ������� � ��������� ��������� ���������
	InputDevice inputDevice;
	std::vector <GameComponent*> Components; // ������ ��������� (�������)
	TriangleComponent TC;

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
	void Update();
	void Draw();
	void ErrorsOutput(int ErrorCode);

	void CreateGrid();
	void CreateTriangle();
	void CreatePyramid();
	void CreateSphere();
	void CreateCapsule();

public:
	Game();
	void Run();
};

