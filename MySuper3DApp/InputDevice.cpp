#include "include.h"
#include "InputDevice.h"

using namespace DirectX::SimpleMath;

InputDevice::InputDevice() {
	keys = new std::unordered_set<Keys>();
}

void InputDevice::Initialize(HWND handle) {
	hWnd = handle;
	keys = new std::unordered_set<Keys>();

	RAWINPUTDEVICE RID[2];
	RID[0].usUsagePage = 0x01;
	RID[0].usUsage = 0x02;
	RID[0].dwFlags = 0;   // добавляет HID-мышь, а также игнорирует устаревшие сообщения мыши 
	RID[0].hwndTarget = hWnd;
	RID[1].usUsagePage = 0x01;
	RID[1].usUsage = 0x06;
	RID[1].dwFlags = 0;   // добавляет HID-клавиатуру, а также игнорирует устаревшие сообщения клавиатуры
	RID[1].hwndTarget = hWnd;

	if (RegisterRawInputDevices(RID, 2, sizeof(RID[0])) == FALSE)
	{
		auto errorCode = GetLastError();
		std::cout << "RegisterRawInputDevices error: " << errorCode << std::endl;
	}
}

InputDevice::~InputDevice() {
	delete keys;
}

void InputDevice::OnKeyDown(KeyboardInputEventArgs args) {
	bool Break = (args.Flags & 0x80) >> 7;
	auto key = (Keys)args.VKey;
	if (args.MakeCode == 42)
		key = Keys::LeftShift;
	if (args.MakeCode == 54)
		key = Keys::RightShift;
	if (Break)
	{
		//std::cout << "Key Up: " << args.MakeCode << " " << args.VKey << std::endl;
		RemovePressedKey(key);
	}
	else
	{
		//std::cout << "Key Down: " << args.MakeCode << " " << args.VKey << std::endl;
		AddPressedKey(key);
	}
}

void InputDevice::OnMouseMove(RawMouseEventArgs args) {
	if (args.ButtonFlags & static_cast<USHORT>(MouseButtonFlags::LeftButtonDown))
	{
		//std::cout << "LeftButtonDown" << std::endl;
		AddPressedKey(Keys::LeftButton);
	}
	else
		RemovePressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<USHORT>(MouseButtonFlags::RightButtonDown))
	{
		//std::cout << "RightButtonDown" << std::endl;
		AddPressedKey(Keys::RightButton);
	}
	else
		RemovePressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<USHORT>(MouseButtonFlags::MiddleButtonDown))
	{
		//std::cout << "MiddleButtonDown" << std::endl;
		AddPressedKey(Keys::MiddleButton);
	}
	else
		RemovePressedKey(Keys::MiddleButton);
	if (args.ButtonFlags & static_cast<USHORT>(MouseButtonFlags::FirstXButton))
		AddPressedKey(Keys::MiddleButton);
	else
		RemovePressedKey(Keys::MiddleButton);
	if (args.ButtonFlags & static_cast<USHORT>(MouseButtonFlags::SecondXButton))
		AddPressedKey(Keys::MiddleButton);
	else
		RemovePressedKey(Keys::MiddleButton);

	MouseParam.Offset = Vector2(Vector2(args.X, args.Y) - MouseParam.Position);
	MouseParam.Position = Vector2(args.X, args.Y);
	MouseParam.WheelDelta = args.WheelDelta;
	MouseMove.Broadcast(MouseParam);
}

void InputDevice::OnChangeScreenSize(int width, int height) {
	ScreenParam.Width = width;
	ScreenParam.Height = height;
	ChangeScreenSize.Broadcast(ScreenParam);
};

void InputDevice::AddPressedKey(Keys key) {
	if (!keys->count(key))
		keys->insert(key);
}

void InputDevice::RemovePressedKey(Keys key) {
	if (keys->count(key))
		keys->erase(key);
}

bool InputDevice::IsKeyDown(Keys key) {
	return keys->count(key);
}
