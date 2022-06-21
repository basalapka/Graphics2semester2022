#pragma once
#include "include.h"
#include "Game.h"

class SolarSystem : public Game
{
private:
	void Initialize();
	TriangleComponentParameters PrepareRect(float xOff, float yOff);
	TriangleComponentParameters PrepareCircle(float xOff, float yOff);
	//void SetBackground(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv);
public:
	SolarSystem();
	//void Update() override;
	void Run();
};

