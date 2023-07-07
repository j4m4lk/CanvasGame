#pragma once

#include "VerticeShapes.h"
#include "Lighting.h"
#include <string>
#include <vector>
#include <directxmath.h>

using namespace DirectX;
class GameObject
{
private:
	vector<VerticeShapes> shapes;
	vector<Lighting> lights;

	XMFLOAT4X4 transform;
	XMFLOAT3 mTranslation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT3 right;

	string name;
	float mSpeed;
	float rotspeed;

	void UpdateChildren();
	void SetTransform();
	void SetDirection();

public:
	GameObject();
	GameObject(const XMFLOAT3& translation, const XMFLOAT3& rotation, const XMFLOAT3& scale, const string& name, const float& speed, const float& rotationSpeed);
	~GameObject();

	const vector<VerticeShapes>& Shapes() const;
	const vector<Lighting>& Lights() const;	
	const XMFLOAT3& Up() const;
	const XMFLOAT3& Right() const;
	const XMFLOAT3& Forward() const;
	const XMFLOAT3& Position() const;
	const XMFLOAT4X4& Transform() const;
	const string& Name() const;
	const float& Speed() const;
	const float& RotationSpeed() const;

	void AddShape(const MeshType& geometryType,  const XMFLOAT3& translation, const XMFLOAT3& rotation, const XMFLOAT3& scale, 
		 const wstring& shader, const string &name, 
		const vector<InstanceData>* const instanceData, const bool& particleSystem);
	void AddLight(const XMFLOAT4& translation, const XMFLOAT4& colour, const string& name);
	void MoveEntity(const XMFLOAT3& translation, const float& timeScale, const float& dt);
	void RotateEntity(const XMFLOAT3& rotation, const float& timeScale, const float& dt);
	void RotateAroundPoint(const XMFLOAT3& point, const XMFLOAT3& rotation, const float& timeScale, const float& dt);
	void SetPosition(const XMFLOAT3& position);
	void ResetRotation();
	void SetSpeed(const float& speed);
	void SetRotationSpeed(const float& speed);
	void RemoveInstancesFromShape(const string& name, const vector<InstanceData>& instances);
	void ChangeLightColour(const string& name, const XMFLOAT4& colour);
};

bool operator==(const GameObject& a, const GameObject& b);
