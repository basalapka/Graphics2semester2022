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
	float radius;
	float speed;
	float x;
	float y;
	Velocity velocity;
	Pong* game;
	//Pong* my_nul = nullptr;
public:
	Ball();
	Ball(Pong* game_, TriangleComponentParameters circle);
	void Update(ID3D11DeviceContext* context) override;
	//TriangleComponentParameters Initialize(float x, float y);
};

class Bar : public TriangleComponent {
private:
	float x;
	float y;
	Pong* game;
	bool isLeft;
	
public:
	Bar();
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
	int score1;
	int score2;
	void Initialize();
	int GetScore();
	float velocity;
	//void Update();
	void Input(TriangleComponent TC);
	void PrepareComponents();
	TriangleComponentParameters PrepareRect(float xOff, float yOff);
	TriangleComponentParameters PrepareCircle(float xOff, float yOff);
	//void SetBackground(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv);
public:
	Pong();
	void Run();
};



