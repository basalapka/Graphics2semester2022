#pragma once
#include "include.h"

class DisplayWin32 {
private:
	LPCWSTR applicationName; // �������� ����������
	HINSTANCE hInstance; // ���������� ����������
	HWND hWnd; // ���������� ����
	WNDCLASSEX wc; // ���������� � ������ ����
	int screenWidth; // ������ ����
	int screenHeight; // ������ ����
	//int posX; // ������� ���� �� ������ ������
	//int posY; // ������� ���� �� ������ ������
public:
	HWND get_hWnd() { return hWnd; };
	int get_screenWidth() { return screenWidth; };
	int get_screenHeight() { return screenHeight; };
	void DisplayWin();
};

