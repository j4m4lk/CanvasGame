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
	XMFLOAT4X4 viewProj;

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

	// Function to create a ray from camera position and screen coordinates
	DirectX::XMFLOAT3 CreateRayFromScreenCoordinates(int screenX, int screenY, int screenWidth, int screenHeight) const
	{
		// Convert screen coordinates to normalized device coordinates (NDC)
		float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
		float ndcY = 1.0f - (2.0f * screenY) / screenHeight;

		// Create the view-projection matrix
		XMMATRIX viewMatrix = XMLoadFloat4x4(&view);
		XMMATRIX projMatrix = XMLoadFloat4x4(&proj);
		XMMATRIX viewProjMatrix = viewMatrix * projMatrix;

		// Invert the view-projection matrix to get the world-to-screen transformation
		XMMATRIX invViewProjMatrix = XMMatrixInverse(nullptr, viewProjMatrix);

		// Transform NDC coordinates to world space
		XMVECTOR rayOrigin = XMVectorSet(ndcX, ndcY, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVectorSet(ndcX, ndcY, 1.0f, 1.0f);
		rayOrigin = XMVector3TransformCoord(rayOrigin, invViewProjMatrix);
		rayDir = XMVector3TransformNormal(rayDir, invViewProjMatrix);
		rayDir = XMVector3Normalize(rayDir);

		// Create the ray with the camera position as the origin
		XMFLOAT3 rayOrigin3, rayDir3;
		XMStoreFloat3(&rayOrigin3, rayOrigin);
		XMStoreFloat3(&rayDir3, rayDir);

		return rayDir3;
	}

};