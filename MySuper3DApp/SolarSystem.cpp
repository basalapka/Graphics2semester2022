#include "SolarSystem.h"


SolarSystem::SolarSystem() {
	Initialize();
	SetBackgroundColor(new float[4]{ 0.2f, 0.2f, 0.5f, 1.0f });
}
void SolarSystem::Initialize() {
	TriangleComponentParameters cube;
	cube.numPoints = 48;
	cube.numIndeces = 36;
	cube.points = new DirectX::XMFLOAT4[48]{
	DirectX::XMFLOAT4(-0.5f, 0.0f, 0.5f, 1.0f), // ������ �����
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, 0.5f, 1.0f), // ������� �����
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, -0.5f, 1.0f), // �������� �����
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, -0.5f, 1.0f), // ������ �����
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, -0.5f, 1.0f), // ����� �����
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, 0.5f, 1.0f), // ������ �����
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)};
	cube.indeces = new int[36] {
		0, 1, 2, // ������ �����
			2, 3, 0,
			6, 5, 4, // ������� �����
			4, 7, 6,
			8, 9, 10, // �������� �����
			10, 11, 8,
			14, 13, 12, // ������ �����
			12, 15, 14,
			18, 17, 16, // ����� �����
			16, 19, 18,
			20, 21, 22, // ������ �����
			22, 23, 20};
	cube.compPosition = DirectX::SimpleMath::Vector3(2, 0, 3);
	Components.push_back(new TriangleComponent(cube));

	TriangleComponentParameters rect;
	rect.numPoints = 8;
	rect.numIndeces = 6;
	rect.indeces = new int[6]{ 0,1,2, 1,0,3 };
	rect.points = new DirectX::XMFLOAT4[rect.numPoints]{
		DirectX::XMFLOAT4(0.05f, 0.1f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f, -0.1f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f, -0.1f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f, 0.1f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
	};
	Components.push_back(new TriangleComponent(rect));
}
void SolarSystem::Run() {
	Game::Run();
}
