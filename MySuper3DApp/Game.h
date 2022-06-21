#pragma once

#include "include.h"
#include "DisplayWin32.h"
#include "GameComponent.h"
#include "TriangleComponent.h"

class Game {
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	DisplayWin32 DW;
	D3D11_VIEWPORT viewport; 
	IDXGISwapChain* swapChain; // свапчейн (цепочка подкачки)
	ID3D11RenderTargetView* rtv; // целевой объект рендеринга
	ID3D11Debug* debug;	
	TriangleComponent TC;
	std::chrono::time_point<std::chrono::steady_clock> prevTime;
	float deltaTime;
	float totalTime = 0;
	unsigned int frameCount = 0;

	void Initialize();
	int PrepareResources();
	void DestroyResources();
	void PrepareFrame();
	void Draw();
	
	float* BGcolor; //цвет заднего фона
	std::vector <Camera*> camera;

public:
	InputDevice inputDevice;
	std::vector <GameComponent*> Components; // вектор компонент (акторов)
	ID3D11DeviceContext* context; // структура, содержащая сведения об атрибутах рисования устройства
	bool IsKeyDown(Keys key) { inputDevice.IsKeyDown(key); };
	virtual void Update();
	Game();
	void Run();
	void SetBackgroundColor(float* color) { BGcolor = color; };//установка цвета фона
};

