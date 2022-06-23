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
	TriangleComponent* Sun;
	TriangleComponent* Mercury;
	TriangleComponent* Venus;
	TriangleComponent* Mars;
	TriangleComponent* Phobos;
	TriangleComponent* Earth;
	//void Update() override;
	void Run();
	void Update() override;
	TriangleComponent CreateSphere(float r, DirectX::SimpleMath::Vector4 mainColor, DirectX::SimpleMath::Vector4 color);
	TriangleComponent CreateCube();
	std::vector <TriangleComponent*> Asteroids;
	float angle = 0;
};

