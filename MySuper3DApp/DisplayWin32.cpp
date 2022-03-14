#pragma once
#include <unordered_set>
#include <iostream>
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include "InputDevice.h"

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
	void CreateDisplay(InputDevice* iDev);
	void OnChangeScreenSize(const ScreenSize& args);
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	KeyboardInputEventArgs keyboardInfo;
	RawMouseEventArgs mouseInfo;
	switch (umessage) {
	case WM_NCCREATE:
		SetLastError(0);
		SetWindowLongPtr(hwnd, DWLP_USER, (LONG_PTR)(((CREATESTRUCT*)lparam)->lpCreateParams));
		if (GetLastError() != 0)
			std::cout << "SetWindowLongPtr error: " << GetLastError() << std::endl;
		return true;
	case WM_DESTROY: // закрытие окна
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE: // изменение размера окна
		((InputDevice*)GetWindowLongPtr(hwnd, DWLP_USER))->OnChangeScreenSize(LOWORD(lparam), HIWORD(lparam));
		return 0;
	case WM_KEYDOWN: // нажатие клавиши
		if (static_cast<unsigned int>(wparam) == (unsigned int)Keys::Escape)
		{
			std::cout << "Exiting the game..." << std::endl;
			PostQuitMessage(0);
		}
		else
		{
			keyboardInfo.Message = umessage;
			keyboardInfo.VKey = static_cast<unsigned int>(wparam);
			keyboardInfo.MakeCode = (HIWORD(lparam) & 0xFF);
			keyboardInfo.Flags = ((HIWORD(lparam) & 0xFF00) >> 8);
			((InputDevice*)GetWindowLongPtr(hwnd, DWLP_USER))->OnKeyDown(keyboardInfo); // Отправка в объект ввода для обработки
		}
		return 0;
	case WM_KEYUP: // отжатие клавиши
		keyboardInfo.Message = umessage;
		keyboardInfo.VKey = static_cast<unsigned int>(wparam);
		keyboardInfo.MakeCode = (HIWORD(lparam) & 0xFF);
		keyboardInfo.Flags = ((HIWORD(lparam) & 0xFF00) >> 8);
		((InputDevice*)GetWindowLongPtr(hwnd, DWLP_USER))->OnKeyDown(keyboardInfo); // Отправка в объект ввода для прекращения обработки
		return 0;
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		mouseInfo.X = LOWORD(lparam);
		mouseInfo.Y = HIWORD(lparam);
		mouseInfo.WheelDelta = 0;
		mouseInfo.ButtonFlags = LOWORD(wparam);
		((InputDevice*)GetWindowLongPtr(hwnd, DWLP_USER))->OnMouseMove(mouseInfo);
		return 0;
	case WM_MOUSEWHEEL:
		mouseInfo.X = LOWORD(lparam);
		mouseInfo.Y = HIWORD(lparam);
		mouseInfo.WheelDelta = (int)HIWORD(wparam);
		mouseInfo.ButtonFlags = LOWORD(wparam);
		((InputDevice*)GetWindowLongPtr(hwnd, DWLP_USER))->OnMouseMove(mouseInfo);
		return 0;
	default: // Все остальные сообщения передаются обработчику сообщений в системном классе
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

void DisplayWin32::CreateDisplay(InputDevice* iDev) {
	applicationName = L"Game";

	// Настройка класса окна начальными значениями
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // перерисовка окна при изменении ширины или высоты области + уникальный контекст устройства для каждого окна
	wc.lpfnWndProc = WndProc; // указатель на оконную процедуру
	wc.cbClsExtra = 0; // число дополнительных байт после структуры
	wc.cbWndExtra = 24; // число дополнительных байт после экземпляра окна (на ссылку iDev)
	wc.hInstance = hInstance; // дескриптор приложения
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO); // дескриптор иконки приложения
	wc.hIconSm = wc.hIcon; // дескриптор мини иконки
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // дескриптор курсора
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)); // дескриптор фона
	wc.lpszMenuName = nullptr; // название меню
	wc.lpszClassName = applicationName; // название класса окна
	wc.cbSize = sizeof(WNDCLASSEX); // размер этой структуры в байтах

	RegisterClassEx(&wc); // Регистрация созданного класса окна
	screenWidth = 1200;
	screenHeight = 800;
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	RECT windowRect = { 0, 0, static_cast<LONG>(screenWidth), static_cast<LONG>(screenHeight) }; // Определение левого верхнего и правого нижнего углов прямоугольника
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE); // вычисление фактического размера окна
	hWnd = CreateWindowEx( // Создание окна с настройками и получение дескриптора
		WS_EX_APPWINDOW, // окно верхнего уровня на панели задач
		applicationName, // имя класса
		applicationName, // имя окна
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME, // имеет меню окна + область заголовка + кнопку сворачивания + рамку изменения размера
		posX, posY, // положение окна на экране
		windowRect.right - windowRect.left, // фактическая ширина окна
		windowRect.bottom - windowRect.top, // фактическая высота окна
		nullptr, // дескриптор родительского окна
		nullptr, // дексриптор меню
		hInstance, // дескриптор приложения
		iDev); // указатель на обработчик ввода (не обязательный параметр, пользовательские данные)

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd); // Переводит поток, который создал окно, в приоритетный режим
	SetFocus(hWnd); // Устанавливает фокус клавиатуры на окно
	ShowCursor(true);

	iDev->ChangeScreenSize.AddRaw(this, &DisplayWin32::OnChangeScreenSize);
};

void DisplayWin32::OnChangeScreenSize(const ScreenSize& args) {
	screenWidth = args.Width;
	screenHeight = args.Height;
}