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
	int screenWidth; // ширина окна
	int screenHeight; // высота окна
	int posX; // позиция окна по ширине экрана
	int posY; // позиция окна по высоте экрана
public:
	HWND get_hWnd() { return hWnd; };
	int get_screenWidth() { return screenWidth; };
	int get_screenHeight() { return screenHeight; };
	void DisplayWin(InputDevice* iDev);
	void OnChangeScreenSize(const ScreenSize& args);
};

