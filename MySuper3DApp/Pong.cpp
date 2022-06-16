#include "Game.h"
#include "Pong.h"

Pong::Pong() {

	Components.push_back(new TriangleComponent(bar1.Initialize(0.7, 0)));
	Components.push_back(new TriangleComponent(bar2.Initialize(-0.7, 0)));
	Components.push_back(new TriangleComponent(ball.Initialize(0, 0)));
	//BGcolor = new float[4]{ 0.0f,0.0f, 0.0f, 0.0f };
	SetBackgroundColor(new float[4]{ 0.0f, 0.4f, 0.3f, 1.0f });
}
void Pong::Update() {
	//bar1.Update(Game::context, 0.5f, 0.5f);
	std::cout << "Update " << std::endl;
}
void Pong::Run() {
	Game::Run();
	//bar1.Update(Game::context, 0.5f, 0.5f);
}
//void Game::Update() override {
//
//}
//void Input(TriangleComponent TC) {
//	bool a = Pong.IsKeyDown(Keys::87);
//	if (GetKeyboardState().IsKeyDown(graph::Keys::kW)) {
//		input_component.up = true;
//	}
//	if (GetKeyboardState().IsKeyDown(graph::Keys::kS)) {
//		input_component.down = true;
//	}
//	break;
//}

//COMPONENTS
TriangleComponentParameters Ball::Initialize(float xOff, float yOff) {
	radius = 0.05f;
	//x^2+y^2=r^2
	//y^2=r^2-x^2
	TriangleComponentParameters circle;
	circle.numPoints = 72;
	circle.numIndeces = 36;
	int i = 0;
	y = 0.0f;
	float temp = 0.0f;
	circle.positions = new DirectX::XMFLOAT4[circle.numPoints*2];
	circle.indeces = new int[circle.numIndeces*2];
	for (int j = 0; j < circle.numIndeces; j++) {
		circle.indeces[j] = j;
	}
	for (float x = - radius; x < radius - 0.00001; x= x + (radius / 6)) {
		//y = sqrt(pow(radius, 2) - pow(x, 2));
		circle.positions[i] = DirectX::XMFLOAT4(xOff, yOff, 0.5f, 1.0f);
		//circle.indeces[i] = i;
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(x + xOff, y + yOff, 0.5f, 1.0f);
		//circle.indeces[i] = i;
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
		temp = x + (radius / 6);
		y = sqrt(pow(radius, 2) - pow(temp, 2));
		circle.positions[i] = DirectX::XMFLOAT4(temp + xOff, y + yOff, 0.5f, 1.0f);
		//circle.indeces[i] = i;
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
	}
	circle.numPoints = circle.numPoints*2;
	circle.numIndeces = circle.numIndeces*2;
	y = 0.0f;
	temp = 0.0f;
	for (int j = 24; j < circle.numIndeces; j++) {
		circle.indeces[j] = j;
	}
	for (float x = -radius; x < radius -0.00001; x = x + (radius / 6)) {
		//y = sqrt(pow(radius, 2) - pow(x, 2));
		circle.positions[i] = DirectX::XMFLOAT4(xOff, yOff, 0.5f, 1.0f);
		//circle.indeces[i] = i;
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(x + xOff, -y + yOff, 0.5f, 1.0f);
		//circle.indeces[i] = i;
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
		temp = x + (radius / 6);
		y = sqrt(pow(radius, 2) - pow(temp, 2));
		circle.positions[i] = DirectX::XMFLOAT4(temp + xOff, -y + yOff, 0.5f, 1.0f);
		//circle.indeces[i] = i;
		i++;
		circle.positions[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
	}
	/*circle.positions = new DirectX::XMFLOAT4[circle.numPoints]{
		DirectX::XMFLOAT4(0.05f + xOff, 0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f + xOff, 0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f + xOff, 0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.1f + xOff, 0.0f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)

	};*/
	x = xOff;
	y = yOff;
	return circle;
};

TriangleComponentParameters Bar::Initialize(float xOff, float yOff) {
	TriangleComponentParameters rect;
	rect.numPoints = 8;
	rect.numIndeces = 6;
	x = xOff;
	y = yOff;
	rect.indeces = new int[6]{ 0,1,2, 1,0,3 };
	rect.positions = new DirectX::XMFLOAT4[rect.numPoints]{
		DirectX::XMFLOAT4(0.05f + xOff, 0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f + xOff, 0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	//rect.colors = new DirectX::XMFLOAT4[rect.numPoints]/*{
	//	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//	DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//}*/;
	//for (int i = 0; i < rect.numPoints; i++)
	//	rect.colors[i] = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	return rect;

	
};