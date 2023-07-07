#pragma once

#include <directxmath.h>
using namespace DirectX;
class Camera
{
private:
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 transform;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT3 right;
	XMFLOAT3 lookatv;
	float speed;
	float rotSpeed;
	bool isMovementAllowed;
	bool isLookAt;

	void SetView();
	void SetProjection(const float& fov, const float& aspectRatio, const float& near, const float& far);
	void SetTransform();
	void SetDirection();

public:
	Camera();
	Camera(const bool& allowMovement, const XMFLOAT3& position, const XMFLOAT3& rotation, const float& fov, const float& aspectRatio,
		const float& near, const float& far, const float& speed, const float& rotationSpeed);
	Camera(const bool& allowMovement, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& lookAt, const float& fov,
		const float& aspectRatio, const float& near, const float& far, const float& speed, const float& rotationSpeed);
	~Camera();

	const XMFLOAT4X4& View() const;
	const XMFLOAT4X4& Projection() const;
	const XMFLOAT3& Position() const;
	const XMFLOAT3& Up() const;
	const XMFLOAT3& Right() const;
	const XMFLOAT3& Forward() const;
	const bool& AllowMovement() const;
	const float& Speed() const;
	const float& RotationSpeed() const;

	void MoveCamera(const XMFLOAT3& translation, const float& dt);
	void MoveCamera(const XMFLOAT3& translation, const float& timeScale, const float& dt);
	void RotateCamera(const XMFLOAT3& rotation, const float& dt);
	void SetLookAt(const XMFLOAT3& lookAt);
	void SetPosition(const XMFLOAT3& position);
	void SetSpeed(const float& speed);
	void SetRotationSpeed(const float& speed);
};