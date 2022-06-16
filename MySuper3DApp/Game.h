#pragma once

#include "include.h"
#include "DisplayWin32.h"
#include "GameComponent.h"
#include "TriangleComponent.h"

class Game {
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	DisplayWin32 DW;
	D3D11_VIEWPORT viewport; // размеры вьюпорта
	int numVP; // количество вьюпортов
	
	// Все вызовы рисования выполняются через объект контекста устройства, который инкапсулирует интерфейсы.
	IDXGISwapChain* swapChain; // свапчейн (цепочка подкачки)
	ID3D11RenderTargetView* rtv; // целевой объект рендеринга
	// Позволяет переходить к интересующим частям временной шкалы или понимать, какой набор вызовов Direct3D производится какими разделами кода приложения
	ID3D11Debug* debug; // Интерфейс отладки управляет настройками отладки и проверяет состояние конвейера
	
	
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
	std::vector <GameComponent*> Components; // вектор компонент (акторов)
	ID3D11DeviceContext* context; // структура, содержащая сведения об атрибутах рисования устройства, таких как экран или принтер
	bool IsKeyDown(Keys key) { inputDevice.IsKeyDown(key); };
	virtual void Update();
	Game();
	void Run();
	void SetBackgroundColor(float* color) { BGcolor = color; };
};

