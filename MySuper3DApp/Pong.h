#pragma once
#include "Game.h"
#include "include.h"
class Ball : public TriangleComponent {
private:
	float x;
	float y;
public:
	//Ball();
	TriangleComponentParameters Initialize(float x, float y);
};

class Bar : public TriangleComponent {
private:
	float x;
	float y;
public:
	TriangleComponentParameters Initialize(float x, float y);
};

class Pong : public Game {
private:
	ID3D11DeviceContext* context; // структура, содержащая сведения об атрибутах рисования устройства, таких как экран или принтер
	Ball ball;
	Bar bar1;
	Bar bar2;
	int score1;
	int score2;
	void Initialize();
	int GetScore();
	float velocity;
	void Update();
	//void SetBackground(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv);
public:
	Pong();
	void Run();
};



