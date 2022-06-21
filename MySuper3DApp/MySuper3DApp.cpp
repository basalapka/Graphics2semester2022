// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "include.h"
#include "DisplayWin32.h"
#include "Game.h"
#include "Pong.h"
#include "SolarSystem.h"

//#pragma pack(push, 4)
////Константный буфер 
//struct ConstData
//{
//	float x;
//	float y;
//	float dummy0;
//	float dummy1;
//};
//#pragma pack(pop)
//
//ConstData constData = {0,0};


int main()
{
	/*Pong test;
	test.Run();*/
	SolarSystem SS;
	SS.Run();
	
	
}
////подсчет времени между кадрами и фпс
//auto	curTime = std::chrono::steady_clock::now();
//float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
//PrevTime = curTime;

//totalTime += deltaTime;
//frameCount++;

//if (totalTime > 1.0f) {
//	float fps = frameCount / totalTime;

//	totalTime -= 1.0f;

//	WCHAR text[256];
//	swprintf_s(text, TEXT("FPS: %f"), fps);
//	SetWindowText(DW.get_hWnd(), text);

//	frameCount = 0;
//}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
