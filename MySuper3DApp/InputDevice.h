#pragma once

#include "include.h"

#include <unordered_set>
#include "Keys.h"
#include "Delegates.h"
#include "SimpleMath.h"

struct MouseMoveEventArgs
{
	DirectX::SimpleMath::Vector2 Position;
	DirectX::SimpleMath::Vector2 Offset;
	int WheelDelta;
};

struct KeyboardInputEventArgs {
	UINT   Message;
	USHORT VKey;
	USHORT MakeCode;
	USHORT Flags;
	// 0 бит: является ли клавиша расширенной клавишей, например правыми клавишами ALT и CTRL. 1 - расширенный ключ; в противном случае - 0.
	// 1-4 биты: зарезервировано
	// 5 бит: код контекста (всегда 0 для WM_KEYDOWN и WM_KEYUP)
	// 6 бит: 
	// для WM_KEYDOWN - предыдущее ключевое состояние. 1 - клавиша уже была нажата до отправки сообщения; 0 - клавиша только что нажата (до этого отжата).
	// для WM_KEYUP - всегда 1.
	// 7 бит:
	// для WM_KEYDOWN - всегда 0.
	// для WM_KEYUP - всегда 1.
};

struct RawMouseEventArgs
{
	int X;
	int Y;
	SHORT WheelDelta;
	USHORT ButtonFlags;
};

enum class MouseButtonFlags
{
	LeftButtonDown = 1, // 0x0001
	RightButtonDown = 2, // 0x0002
	MiddleButtonDown = 16, // 0x0010
	FirstXButton = 32, // 0x0020
	SecondXButton = 64, //0x0040

	None = 0,
};

struct ScreenSize {
	int Width;
	int Height;
};

class InputDevice {
private:
	HWND hWnd;
	std::unordered_set<Keys>* keys;
	MouseMoveEventArgs MouseParam;
	ScreenSize ScreenParam;
	void AddPressedKey(Keys key);
	void RemovePressedKey(Keys key);
public:
	InputDevice();
	~InputDevice();
	void Initialize(HWND handle);
	void OnKeyDown(KeyboardInputEventArgs args);
	void OnMouseMove(RawMouseEventArgs args);
	void OnChangeScreenSize(int width, int height);
	bool IsKeyDown(Keys key);
	MouseMoveEventArgs getMouseParam() { return MouseParam; };
	MulticastDelegate<const MouseMoveEventArgs&> MouseMove;
	MulticastDelegate<const ScreenSize&> ChangeScreenSize;
};
