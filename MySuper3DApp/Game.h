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
	
	// ��� ������ ��������� ����������� ����� ������ ��������� ����������, ������� ������������� ����������.
	IDXGISwapChain* swapChain; // �������� (������� ��������)
	ID3D11RenderTargetView* rtv; // ������� ������ ����������
	// ��������� ���������� � ������������ ������ ��������� ����� ��� ��������, ����� ����� ������� Direct3D ������������ ������ ��������� ���� ����������
	ID3D11Debug* debug; // ��������� ������� ��������� ����������� ������� � ��������� ��������� ���������
	
	
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

	void Draw();
	void ErrorsOutput(int ErrorCode);
	
	float* BGcolor;

public:
	InputDevice inputDevice;
	std::vector <GameComponent*> Components; // ������ ��������� (�������)
	ID3D11DeviceContext* context; // ���������, ���������� �������� �� ��������� ��������� ����������, ����� ��� ����� ��� �������
	bool IsKeyDown(Keys key) { inputDevice.IsKeyDown(key); };
	virtual void Update();
	Game();
	void Run();
	void SetBackgroundColor(float* color) { BGcolor = color; };
};

