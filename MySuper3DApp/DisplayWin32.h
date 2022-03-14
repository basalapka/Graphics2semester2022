#pragma once
#include "pch.h"
#include "InputDevice.h"

class DisplayWin32 {
private:
	LPCWSTR applicationName; // название приложения
	HINSTANCE hInstance; // дескриптор приложения
	HWND hWnd; // дексриптор окна
	WNDCLASSEX wc; // класс окна
	//виндовс сайз = клиент сайз+рамки и т.д.
	int screenWidth = 800;
	int screenHeight = 800;
	int posX;
	int posY;
public:
	HWND get_hWnd() { return hWnd; };
	int get_screenWidth() { return screenWidth; };
	int get_screenHeight() { return screenHeight; };
	void CreateDisplay(InputDevice* iDev);
	void OnChangeScreenSize(const ScreenSize& args);
};
