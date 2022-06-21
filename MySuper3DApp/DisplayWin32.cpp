#include "include.h"
#include "DisplayWin32.h"

//std::unordered_set<unsigned int> pressedKeys;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	KeyboardInputEventArgs keyboardInfo;
	RawMouseEventArgs mouseInfo;
	//проверка сообщения
	switch (umessage)
	{
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

		//каждый раз когда клавиша нажимается передается сообщение
	case WM_KEYDOWN:
	{
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
	}
	case WM_KEYUP:
	{
		keyboardInfo.Message = umessage;
		keyboardInfo.VKey = static_cast<unsigned int>(wparam);
		keyboardInfo.MakeCode = (HIWORD(lparam) & 0xFF);
		keyboardInfo.Flags = ((HIWORD(lparam) & 0xFF00) >> 8);
		((InputDevice*)GetWindowLongPtr(hwnd, DWLP_USER))->OnKeyDown(keyboardInfo); // Отправка в объект ввода для прекращения обработки
		return 0;
	}
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

void DisplayWin32::DisplayWin(InputDevice* iDev) {
	//имя класса
	applicationName = L"My3DApp";
	hInstance = GetModuleHandle(nullptr);

#pragma region Window init
	//если мы изменяем размер окна, то выйдет сообщение
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	//окно получает инфу о вводе
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

	// Register the window class.
	RegisterClassEx(&wc);

	//виндовс сайз = клиент сайз+рамки и т.д.
	screenWidth = 800;
	screenHeight = 800;
	//функция чтобы клиент сайз был 800 на 800
	RECT windowRect = { 0, 0, static_cast<LONG>(screenWidth), static_cast<LONG>(screenHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	//стиль окна
	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;
	//позиция окна по середине всего экрана
	auto posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	//при создании окна мы получаем указатель на это окно
	// Создание окна с настройками и получение дескриптора
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
		dwStyle,
		// положение окна на экране
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, // дескриптор родительского окна
		nullptr, // дексриптор меню
		hInstance, // дескриптор приложения
		iDev); // указатель на обработчик ввода
	//показ окна
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
	//iDev->ChangeScreenSize.AddRaw(this, &DisplayWin32::OnChangeScreenSize);
#pragma endregion Window init
}
void DisplayWin32::OnChangeScreenSize(const ScreenSize& args) {
	screenWidth = args.Width;
	screenHeight = args.Height;
}
