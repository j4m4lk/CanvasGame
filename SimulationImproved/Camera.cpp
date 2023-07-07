#include "Camera.h"

using namespace DirectX;


void Camera::SetView()
{
	
	const XMVECTOR positionVector = XMLoadFloat3(&pos);
	const XMVECTOR upDirVector = XMLoadFloat3(&up);

	
	if (isLookAt)
	{
		const XMVECTOR mLookAtVector = XMLoadFloat3(&lookatv);
		const XMMATRIX viewMatrix = XMMatrixLookAtLH(positionVector, mLookAtVector, upDirVector);
		XMStoreFloat4x4(&view, viewMatrix);
	}
	else
	{
		const XMVECTOR forwardVector = XMLoadFloat3(&forward);
		const XMMATRIX viewMatrix = XMMatrixLookAtLH(positionVector, positionVector + forwardVector, upDirVector);
		XMStoreFloat4x4(&view, viewMatrix);
	}
}


void Camera::SetProjection(const float& fov, const float& aspectRatio, const float& near, const float& far)
{
	
	const XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, near, far);	
	XMStoreFloat4x4(&proj, projectionMatrix);
}


void Camera::SetTransform()
{
	
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
	transformMatrix *= XMMatrixRotationX(rot.x) * XMMatrixRotationY(rot.y) * XMMatrixRotationZ(rot.z);
	XMStoreFloat4x4(&transform, transformMatrix);
}


void Camera::SetDirection()
{
	
	const XMVECTOR rightVector = XMLoadFloat3(&XMFLOAT3(transform._11, transform._12, transform._13));
	const XMVECTOR upVector = XMLoadFloat3(&XMFLOAT3(transform._21, transform._22, transform._23));
	const XMVECTOR forwardVector = XMLoadFloat3(&XMFLOAT3(transform._31, transform._32, transform._33));

	
	XMStoreFloat3(&right, XMVector3Normalize(rightVector));
	XMStoreFloat3(&up, XMVector3Normalize(upVector));
	XMStoreFloat3(&forward, XMVector3Normalize(forwardVector));
}


Camera::Camera() :
	speed(0),
	rotSpeed(0),
	isMovementAllowed(false),
	isLookAt(false)
{
}


Camera::Camera(const bool& allowMovement, const XMFLOAT3& position, const XMFLOAT3& rotation, const float& fov, const float& aspectRatio,
	const float& near, const float& far, const float& speed, const float& rotationSpeed) :
	pos(position),
	rot(rotation),
	speed(speed),
	rotSpeed(rotationSpeed),
	isMovementAllowed(allowMovement),
	isLookAt(false)
{
	
	SetProjection(fov, aspectRatio, near, far);
	SetTransform();
	SetDirection();
	SetView();
}


Camera::Camera(const bool& allowMovement, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& lookAt, const float& fov,
	const float& aspectRatio, const float& near, const float& far, const float& speed, const float& rotationSpeed) :
	pos(position),
	rot(rotation),
	lookatv(lookAt),
	speed(speed),
	rotSpeed(rotationSpeed),
	isMovementAllowed(allowMovement),
	isLookAt(true)
{
	
	SetProjection(fov, aspectRatio, near, far);
	SetTransform();
	SetDirection();
	SetView();
}

Camera::~Camera()
{
}


const XMFLOAT4X4& Camera::View() const
{
	return view;
}


const XMFLOAT4X4& Camera::Projection() const
{
	return proj;
}


const XMFLOAT3& Camera::Position() const
{
	return pos;
}


const XMFLOAT3& Camera::Up() const
{
	return up;
}

const XMFLOAT3 & Camera::Right() const
{
	return right;
}


const XMFLOAT3 & Camera::Forward() const
{
	return forward;
}


const bool & Camera::AllowMovement() const
{
	return isMovementAllowed;
}

const float & Camera::Speed() const
{
	return speed;
}


const float & Camera::RotationSpeed() const
{
	return rotSpeed;
}


void Camera::MoveCamera(const XMFLOAT3& translation, const float& dt)
{
	
	const XMVECTOR translationVector = XMLoadFloat3(&translation);
	XMVECTOR positionVector = XMLoadFloat3(&pos);


	positionVector = positionVector + translationVector * speed * dt;
	XMStoreFloat3(&pos, positionVector);

	
	XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
	transformMatrix = transformMatrix * XMMatrixTranslationFromVector(translationVector);
	XMStoreFloat4x4(&transform, transformMatrix);

	
	SetView();
}

void Camera::MoveCamera(const XMFLOAT3 & translation, const float & timeScale, const float & dt)
{
	
	const XMVECTOR translationVector = XMLoadFloat3(&translation);
	XMVECTOR positionVector = XMLoadFloat3(&pos);

	
	positionVector = positionVector + translationVector * timeScale * speed * dt;
	XMStoreFloat3(&pos, positionVector);

	
	XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
	transformMatrix = transformMatrix * XMMatrixTranslationFromVector(translationVector);
	XMStoreFloat4x4(&transform, transformMatrix);

	
	SetView();
}


void Camera::RotateCamera(const XMFLOAT3& rotation, const float& dt)
{
	
	const XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&right), rotation.x * rotSpeed * dt) * XMMatrixRotationY(rotation.y * rotSpeed * dt);

	
	const XMVECTOR rotationVector = XMLoadFloat3(&rot);
	XMVector3Transform(rotationVector, rotationMatrix);
	XMStoreFloat3(&rot, rotationVector);

	
	XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
	transformMatrix = transformMatrix * rotationMatrix;
	XMStoreFloat4x4(&transform, transformMatrix);

	
	SetDirection();
	SetView();
}

void Camera::SetLookAt(const XMFLOAT3 & lookAt)
{
	lookatv = lookAt;
	SetView();
}


void Camera::SetPosition(const XMFLOAT3 & position)
{
	
	pos = position;


	SetTransform();
	SetView();
}


void Camera::SetSpeed(const float & speed)
{
	this->speed = speed;
}


void Camera::SetRotationSpeed(const float & speed)
{
	rotSpeed = speed;
}
