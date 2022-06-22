#include "include.h"
#include "Camera.h"

void Camera::Initialize(DirectX::SimpleMath::Vector3 pos, float yaw, float pitch, int screenWidth, int screenHeight, InputDevice* inpDev) {
	inputDevice = inpDev;
	ViewMatrix = DirectX::SimpleMath::Matrix::Identity;
	//std::cout << "Width: " << screenWidth << " Height: " << screenHeight << std::endl;
	ProjectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		((float)PI) / 2.0f, // ���� ���� ������ �� ��� y (� ��������)
		(float)screenWidth / screenHeight, // ����������� ������ �����������(������ / ������)
		0.1f, // ���������� �� ������� ��������� �������
		100.0f); // ���������� �� ������� ��������� �������
	Yaw = yaw;
	Pitch = pitch;
	position = pos;
	if (inputDevice != nullptr)
		inputDevice->MouseMove.AddRaw(this, &Camera::OnMouseMove);
}

void Camera::Update(float deltaTime, int screenWidth, int screenHeight) {
	auto rotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(Yaw, Pitch, 0);
	if (inputDevice != nullptr) {
		auto velDirection = DirectX::SimpleMath::Vector3::Zero;
		if (inputDevice->IsKeyDown(Keys::W)) velDirection += DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
		if (inputDevice->IsKeyDown(Keys::S)) velDirection += DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
		if (inputDevice->IsKeyDown(Keys::A)) velDirection += DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
		if (inputDevice->IsKeyDown(Keys::D)) velDirection += DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);

		if (inputDevice->IsKeyDown(Keys::Space)) velDirection += DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
		if (inputDevice->IsKeyDown(Keys::Q)) velDirection += DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);

		velDirection.Normalize();

		auto velDir = rotation.Forward() * velDirection.x + DirectX::SimpleMath::Vector3::Up * velDirection.y + rotation.Right() * velDirection.z;
		if (velDir.Length() != 0) {
			velDir.Normalize();
		}
		position = position + velDir * VelocityMagnitude * deltaTime;
	}
	ViewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(position, position + rotation.Forward(), rotation.Up());
	ProjectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		(float)PI / 2.0f, // ���� ���� ������ �� ��� y (� ��������)
		(float)screenWidth / screenHeight, // ����������� ������ �����������(������ / ������)
		0.1f, // ���������� �� ������� ��������� �������
		100.0f); // ���������� �� ������� ��������� �������
}

void Camera::OnMouseMove(const MouseMoveEventArgs& args) {
	if (inputDevice == nullptr)
		return;
	if (inputDevice->IsKeyDown(Keys::LeftShift))
		return;

	Yaw -= args.Offset.x * 0.003f * MouseSensetivity;
	Pitch -= args.Offset.y * 0.003f * MouseSensetivity;

	if (args.WheelDelta == 0) return;
	if (args.WheelDelta > 0) VelocityMagnitude += 1;
	if (args.WheelDelta < 0) VelocityMagnitude -= 1;
}
