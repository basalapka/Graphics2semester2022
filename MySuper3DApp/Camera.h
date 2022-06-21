#pragma once
#include "include.h"
#include "InputDevice.h"

#define PI 3.1415926535

class Camera {
private:
	InputDevice* inputDevice;
public:
	DirectX::SimpleMath::Matrix ViewMatrix;
	DirectX::SimpleMath::Matrix ProjectionMatrix;

	float Yaw;
	float Pitch;
	DirectX::SimpleMath::Vector3 position;
	float VelocityMagnitude = 5.0f;
	float MouseSensetivity = 1.0f;

	void Initialize(DirectX::SimpleMath::Vector3 pos, float yaw, float pitch, int screenWidth, int screenHeight, InputDevice* inpDev);
	void Update(float deltaTime, int screenWidth, int screenHeight);
	void OnMouseMove(const MouseMoveEventArgs& args);
};
