#pragma once
#include "include.h"
#include "InputDevice.h"
#include "Keys.h"

class DisplayWin32 {
private:
	LPCWSTR applicationName; // название приложения
	HINSTANCE hInstance; // дескриптор приложения
	HWND hWnd; // дексриптор окна
	WNDCLASSEX wc; // информация о классе окна
	int screenWidth;
	int screenHeight;
	int posX;
	int posY;
public:
	HWND get_hWnd() { return hWnd; };
	int get_screenWidth() { return screenWidth; };
	int get_screenHeight() { return screenHeight; };
	void DisplayWin(InputDevice* iDev);
	void OnChangeScreenSize(const ScreenSize& args);
};

