#include "Game.h"
#include "Pong.h"
#include "Keys.h"
#include <random>
#include "SimpleMath.h"

Pong::Pong() {
	Initialize();
	SetBackgroundColor(new float[4]{ 0.0f, 0.4f, 0.3f, 1.0f });
}
void Pong::Initialize() {
	ball = new Ball(this, this->PrepareCircle(0.0f, 0.0f));
	ball->SetDirection();
	Components.push_back(ball);
	bar1 = new Bar(this, this->PrepareRect(0.7, 0), false);
	bar1->setPosition(0.7, 0.0);
	Components.push_back(bar1);
	bar2 = new Bar(this, this->PrepareRect(-0.7, 0), true);
	bar2->setPosition(-0.7, 0.0);
	Components.push_back(bar2);
}
void Pong::PrintScore()
{
	std::cout << "Player Left : " << scoreLeft << "\n"
		<< "Player Right : " << scoreRight << "\n"
		<< std::endl;
}
void Pong::Reset() {
	redLight = 15; //сколько апдейтов держится красный экран
	/*ball->constData = { 0, 0 };
	ball->setPosition(0.0, 0.0);
	bar1->constData = { 0, 0 };
	bar1->setPosition(0.7, 0.0);
	bar2->constData = { 0, 0 };
	bar2->setPosition(-0.7, 0.0);
	ball->SetDirection();
	ball->speed = 1.0f;*/
}
void Pong::Run() {
	Game::Run();
}
void Pong::Update() {
	if (redLight > 0) {
		SetBackgroundColor(new float[4]{ 1.0f, 0.3f, 0.3f, 1.0f });
		redLight -= 1;
	}
	if (redLight == 0) {
		SetBackgroundColor(new float[4]{ 0.0f, 0.4f, 0.3f, 1.0f });
		redLight -= 1;
	}
	for (int i = 0; i < Components.size(); i++)
		//Components[i]->Update(Game::context);
	if (ball->x >= 1.0 - 0.05)
	{
		Reset();
		scoreLeft += 1;
		PrintScore();
	}
	if (ball->x <= -1.0 + 0.05)
	{
		Reset();
		scoreRight += 1;
		PrintScore();
	}
	//от торцов когда отскакивает, то меняется y
	if ((((ball->x - 0.05) <= (bar1->xPos + 0.05)) && ((ball->x + 0.05) >= (bar1->xPos - 0.05)))&&
		(((ball->y - 0.05) <= (bar1->yPos + 0.1)) && ((ball->y + 0.05) >= (bar1->yPos - 0.1)))) {
		ball->direction.x = -ball->direction.x;
		ball->speed += 0.1f;
	}
	if ((((ball->x - 0.05) <= (bar2->xPos + 0.05)) && ((ball->x + 0.05) >= (bar2->xPos - 0.05))) &&
		(((ball->y - 0.05) <= (bar2->yPos + 0.1)) && ((ball->y + 0.05) >= (bar2->yPos - 0.1)))) {
		ball->direction.x = -ball->direction.x;
		ball->speed += 0.1f;
	}
}
Bar::Bar() {
	xPos = 0.0f;
	yPos = 0.0f;
}
Bar::Bar(Pong* game_, TriangleComponentParameters rect, bool isLeft_) : TriangleComponent(rect), game(game_) {
	xPos = 0.0f;
	yPos = 0.0f;
	isLeft = isLeft_;
}
//void Bar::Update(ID3D11DeviceContext* context) {
//	//движение платформочек
//	if (isLeft == true) {
//		if (game->inputDevice.IsKeyDown(Keys::W)) {
//			if (yPos < 1.0-0.1) {
//				constData.y += 0.05;
//				//yPos += 0.05;
//			}
//		};
//		if (game->inputDevice.IsKeyDown(Keys::S)) {
//			if (yPos > - (1.0 - 0.1)) {
//				constData.y -= 0.05;
//				//yPos -= 0.05;
//			}
//		};
//	}
//	else {
//		if (game->inputDevice.IsKeyDown(Keys::Up)) {
//			if (yPos < 1.0 - 0.1) {
//				constData.y += 0.05;
//				//yPos += 0.05;
//			}
//		};
//		if (game->inputDevice.IsKeyDown(Keys::Down)) {
//			if (yPos > -(1.0 - 0.1)) {
//				constData.y -= 0.05;
//				//yPos -= 0.05;
//			}
//		};
//	}
//	yPos = constData.y;
//	D3D11_MAPPED_SUBRESOURCE res = {};
//	context->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
//
//	auto dataP = reinterpret_cast<float*>(res.pData);
//	memcpy(dataP, &constData, sizeof(ConstData));
//
//	context->Unmap(cb, 0);
//}
Ball::Ball(Pong* game_, TriangleComponentParameters circle): TriangleComponent(circle), game(game_) {
	x = 0.0f;
	y = 0.0f;
}
void Ball::SetDirection()
{
	//рандомное направление шарика в начале игры
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 359);
	//перевести в радианаы
	auto angle = uni(rng);
	//x=r*cos(angle); angle= от 0 до 2пи
	//y=r*sin(angle);
	direction.x = sin(angle) * initialSpeed;
	direction.y = cos(angle) * initialSpeed;
	direction.z = 0;
}
//void Ball::Update(ID3D11DeviceContext* context) {
//	x += direction.x*speed;
//	y += direction.y*speed;
//
//	//верх и низ экрана
//	if (y >= 1.0f - radius)
//	{
//		speed += 0.1f;
//		direction.y = -direction.y;
//	}
//	if (y <= -1.0f + radius)
//	{
//		speed += 0.1f;
//		direction.y = -direction.y;
//	}
//	constData.x = x;
//	constData.y = y;
//	D3D11_MAPPED_SUBRESOURCE res = {};
//	context->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
//
//	auto dataP = reinterpret_cast<float*>(res.pData);
//	memcpy(dataP, &constData, sizeof(ConstData));
//
//	context->Unmap(cb, 0);
//}

//Отрисовка круга и прямоугольника
TriangleComponentParameters Pong::PrepareCircle(float xOff, float yOff) {
	float radius = 0.05f;
	//x^2+y^2=r^2
	//y^2=r^2-x^2
	TriangleComponentParameters circle;
	/*circle.numPoints = 114;
	circle.numIndeces = 57;
	int i = 0;
	float y = 0.0f;
	int temp = 1;
	circle.points = new DirectX::XMFLOAT4[circle.numPoints];
	circle.indeces = new int[circle.numIndeces];
	for (int j = 0; j < circle.numIndeces; j++) {
		circle.indeces[j] = 0;
		j++;
		circle.indeces[j] = temp;
		j++;
		temp++;
		circle.indeces[j] = temp;
	}
	temp = 2;
	circle.points[0] = DirectX::XMFLOAT4(xOff, yOff, 0.5f, 1.0f);
	circle.points[1] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 2; i < circle.numIndeces; i++) {
		circle.points[i] = DirectX::XMFLOAT4(radius* std::cos(i*2* std::_Pi/ 57), radius * std::sin(i * 2 * std::_Pi / 57), 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(radius * std::cos(i * 2 * std::_Pi / 57), radius * std::sin(i * 2 * std::_Pi / 57), 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	}*/
	circle.numPoints = 72;
	circle.numIndeces = 36;
	int i = 0;
	float y = 0.0f;
	float temp = 0.0f;
	circle.points = new DirectX::XMFLOAT4[circle.numPoints * 2];
	circle.indeces = new int[circle.numIndeces*2];
	for (int j = 0; j < circle.numIndeces; j++) {
		circle.indeces[j] = j;
	}
	for (float x = - radius; x < radius - 0.00001; x= x + (radius / 6)) {
		//y = sqrt(pow(radius, 2) - pow(x, 2));
		circle.points[i] = DirectX::XMFLOAT4(xOff, yOff, 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(x + xOff, y + yOff, 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
		temp = x + (radius / 6);
		y = sqrt(pow(radius, 2) - pow(temp, 2));
		circle.points[i] = DirectX::XMFLOAT4(temp + xOff, y + yOff, 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
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
		circle.points[i] = DirectX::XMFLOAT4(xOff, yOff, 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(x + xOff, -y + yOff, 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
		temp = x + (radius / 6);
		y = sqrt(pow(radius, 2) - pow(temp, 2));
		circle.points[i] = DirectX::XMFLOAT4(temp + xOff, -y + yOff, 0.5f, 1.0f);
		i++;
		circle.points[i] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		i++;
	}
	return circle;
};

TriangleComponentParameters Pong::PrepareRect(float xOff, float yOff) {
	TriangleComponentParameters rect;
	rect.numPoints = 8;
	rect.numIndeces = 6;
	rect.indeces = new int[6]{ 0,1,2, 1,0,3 };
	rect.points = new DirectX::XMFLOAT4[rect.numPoints]{
		DirectX::XMFLOAT4(0.05f + xOff, 0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f + xOff, -0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f + xOff, 0.1f + yOff, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	return rect;
};