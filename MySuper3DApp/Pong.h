#pragma once
#include "Game.h"
#include "include.h"
#include "Bar.h"
struct Velocity {
	float x;
	float y;
};
class Pong;
class Ball : public TriangleComponent {
private:
	
	float radius=0.05f;

	float initialSpeed = 0.01f;
	//Velocity velocity;
	Pong* game;
	//Pong* my_nul = nullptr;
public:

	float speed = 1.0f;
	DirectX::SimpleMath::Vector3 direction;
	//Ball();
	float x;
	float y;
	void setPosition(float x1, float y1) {
		x = x1;
		y= y1;
	};
	Ball(Pong* game_, TriangleComponentParameters circle);
	void SetDirection();
	void Update(ID3D11DeviceContext* context) override;
	//TriangleComponentParameters Initialize(float x, float y);
};

class Bar : public TriangleComponent {
private:
	
	Pong* game;
	bool isLeft;
	
public:
	Bar();
	float xPos;
	float yPos;
	void setPosition(float x, float y) {
		xPos = x;
		yPos = y;
	};
	Bar(Pong* game_, TriangleComponentParameters rect, bool isLeft_);
	void Update(ID3D11DeviceContext* context) override;
	/*void Update(ID3D11DeviceContext* context) override;
	TriangleComponentParameters Initialize(float x, float y);
	void Update();*/
};

class Pong : public Game {
private:
	ID3D11DeviceContext* context; // структура, содержащая сведения об атрибутах рисования устройства, таких как экран или принтер
	Ball* ball;
	Bar* bar1;
	Bar* bar2;
	int scoreLeft;
	int scoreRight;
	void Initialize();
	int GetScore();
	float velocity;
	//void Update();
	//void Input(TriangleComponent TC);
	void PrepareComponents();
	TriangleComponentParameters PrepareRect(float xOff, float yOff);
	TriangleComponentParameters PrepareCircle(float xOff, float yOff);
	//void SetBackground(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv);
public:
	int redLight = 0;
	void Reset();
	void PrintScore();
	void Update() override;
	Pong();
	void Run();
};



