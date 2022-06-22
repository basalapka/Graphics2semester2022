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
	DirectX::XMFLOAT4(-0.5f, 0.0f, 0.5f, 1.0f), // нижн€€ гр€нь 0
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f),//1
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, -0.5f, 1.0f),//2
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, -0.5f, 1.0f),//3
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, 0.5f, 1.0f), // верхн€€ грань 4
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),//5
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, -0.5f, 1.0f),//6
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, -0.5f, 1.0f), // передн€€ грань
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, -0.5f, 1.0f), // задн€€ грань
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, -0.5f, 1.0f), // лева€ грань
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, -0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.0f, 0.5f, 1.0f), // права€ грань
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)};
	cube.indeces = new int[36] {
		0, 1, 2, // нижн€€ гр€нь
			2, 3, 0,
			6, 5, 4, // верхн€€ грань
			4, 7, 6,
			8, 9, 10, // передн€€ грань
			10, 11, 8,
			14, 13, 12, // задн€€ грань
			12, 15, 14,
			18, 17, 16, // лева€ грань
			16, 19, 18,
			20, 21, 22, // права€ грань
			22, 23, 20};
	cube.compPosition = DirectX::SimpleMath::Vector3(2, 0, 3);
	Components.push_back(new TriangleComponent(cube));

	CreateSphere();

}
void SolarSystem::Run() {
	Game::Run();
}
void SolarSystem::CreateSphere() {
	TriangleComponentParameters sphere;
	int parallels = 100; // параллели
	int meridians = 100; // меридианы
	float r = 1; // радиус сферы
	sphere.numPoints = parallels * meridians * 4 - 1 * 2 * meridians;
	DirectX::SimpleMath::Vector4* positions = new DirectX::SimpleMath::Vector4[sphere.numPoints];
	sphere.numIndeces = parallels * meridians * 6 - 3 * 2 * meridians;
	sphere.numPoints *= 2;
	sphere.points = new DirectX::SimpleMath::Vector4[sphere.numPoints];
	sphere.indeces = new int[sphere.numIndeces];
	int tempPos = 0;
	int tempInd = 0;
	DirectX::SimpleMath::Vector4 vertex1, vertex2, vertex3, vertex4;
	for (int i = 0; i < parallels; i++)
	{
		float teta1 = ((float)(i) / parallels) * PI;
		float teta2 = ((float)(i + 1) / parallels) * PI;
		for (int j = 0; j < meridians; j++)
		{
			float fi1 = ((float)(j) / meridians) * 2 * PI;
			float fi2 = ((float)(j + 1) / meridians) * 2 * PI;

			float x, y, z;
			//с правого верхнего угла квадрата против часовой стрелки
			x = r * sin(teta1) * cos(fi1);
			y = r * sin(teta1) * sin(fi1);
			z = r * cos(teta1);
			vertex1 = DirectX::SimpleMath::Vector4(x, y, z, 1.0f);
			x = r * sin(teta1) * cos(fi2);
			y = r * sin(teta1) * sin(fi2);
			z = r * cos(teta1);
			vertex2 = DirectX::SimpleMath::Vector4(x, y, z, 1.0f);
			x = r * sin(teta2) * cos(fi2);
			y = r * sin(teta2) * sin(fi2);
			z = r * cos(teta2);
			vertex3 = DirectX::SimpleMath::Vector4(x, y, z, 1.0f);
			x = r * sin(teta2) * cos(fi1);
			y = r * sin(teta2) * sin(fi1);
			z = r * cos(teta2);
			vertex4 = DirectX::SimpleMath::Vector4(x, y, z, 1.0f);

			if (i == 0)
			{
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex2);
				sphere.indeces[tempInd] = tempPos;
				tempPos++;
				tempInd++;
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex3);
				sphere.indeces[tempInd] = tempPos;
				tempPos++;
				tempInd++;
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex4);
				sphere.indeces[tempInd] = tempPos;
				tempPos++;
				tempInd++;
			}
			else if (i == parallels - 1)
			{
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex4);
				sphere.indeces[tempInd] = tempPos;
				tempPos++;
				tempInd++;
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex1);
				sphere.indeces[tempInd] = tempPos;
				tempPos++;
				tempInd++;
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex2);
				sphere.indeces[tempInd] = tempPos;
				tempPos++;
				tempInd++;
			}
			else
			{
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex1);
				tempPos++;
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex2);
				tempPos++;
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex3);
				tempPos++;
				positions[tempPos] = DirectX::SimpleMath::Vector4(vertex4);
				tempPos++;

				sphere.indeces[tempInd] = tempPos - 1; // vertex4
				tempInd++;
				sphere.indeces[tempInd] = tempPos - 4; // vertex1
				tempInd++;
				sphere.indeces[tempInd] = tempPos - 3; // vertex2
				tempInd++;
				sphere.indeces[tempInd] = tempPos - 3; // vertex2
				tempInd++;
				sphere.indeces[tempInd] = tempPos - 2; // vertex3
				tempInd++;
				sphere.indeces[tempInd] = tempPos - 1; // vertex4
				tempInd++;
			}
		}
	}
	int temp = 0;
	for (int i = 0; i < sphere.numPoints; i += 2) {
		sphere.points[i] = positions[temp];
		sphere.points[i + 1] = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		temp++;
	}
	sphere.compPosition = DirectX::SimpleMath::Vector3(1.5, 0, 1);
	Components.push_back(new TriangleComponent(sphere));
}
