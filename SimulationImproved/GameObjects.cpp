#include "GameObjects.h"

using namespace std;
using namespace DirectX;


GameObject::GameObject() :
	mSpeed(0),
	rotspeed(0)
{
}


GameObject::GameObject(const XMFLOAT3& translation, const XMFLOAT3& rotation, const XMFLOAT3& scale, const string& name, const float& speed, const float& rotationSpeed) :
	mTranslation(translation),
	rotation(rotation),
	scale(scale),
	name(name),
	mSpeed(speed),
	rotspeed(rotationSpeed)
{
	SetTransform();
	SetDirection();
}


GameObject::~GameObject()
{
}

void GameObject::AddShape(const MeshType& geometryType, const XMFLOAT3& translation, const XMFLOAT3& rotation, const XMFLOAT3& scale, 
	 const wstring& shader, const string& name, 
	const vector<InstanceData>* const instanceData)
{
	
	switch (geometryType)
	{

	case MeshType::CUBE:
	{
		const VerticeShapes Shape(MeshType::CUBE,  transform, translation, rotation, scale,  shader, name, instanceData);
		shapes.push_back(Shape);
		break;
	}
	
	
	
	
	
	
	}
}





const vector<VerticeShapes>& GameObject::Shapes() const
{
	return shapes;
}



const XMFLOAT3& GameObject::Up() const
{
	return up;
}

const XMFLOAT3& GameObject::Right() const
{
	return right;
}

const XMFLOAT3& GameObject::Forward() const
{
	return forward;
}

const XMFLOAT3& GameObject::Position() const
{
	return mTranslation;
}

const XMFLOAT4X4& GameObject::Transform() const
{
	return transform;
}

const string & GameObject::Name() const
{
	return name;
}

const float & GameObject::Speed() const
{
	return mSpeed;
}

const float & GameObject::RotationSpeed() const
{
	return rotspeed;
}

void GameObject::UpdateChildren()
{
	for (auto& shape : shapes)
	{
		shape.SetTransform(transform);
	}

	
}

void GameObject::SetTransform()
{	
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixScaling(scale.x, scale.y, scale.z);
	transformMatrix *= XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z);
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&mTranslation));
		
	XMStoreFloat4x4(&transform, transformMatrix);
}
void GameObject::SetDirection()
{	
	const XMVECTOR rightVector = XMLoadFloat3(&XMFLOAT3(transform._11, transform._12, transform._13));
	const XMVECTOR upVector = XMLoadFloat3(&XMFLOAT3(transform._21, transform._22, transform._23));
	const XMVECTOR forwardVector = XMLoadFloat3(&XMFLOAT3(transform._31, transform._32, transform._33));

	XMStoreFloat3(&right, XMVector3Normalize(rightVector));
	XMStoreFloat3(&up, XMVector3Normalize(upVector));
	XMStoreFloat3(&forward, XMVector3Normalize(forwardVector));
}

void GameObject::MoveEntity(const XMFLOAT3& translation, const float& timeScale, const float& dt)
{	
	XMVECTOR positionVector = XMLoadFloat3(&mTranslation);
	const XMVECTOR translationVector = XMLoadFloat3(&translation);
	
	positionVector = positionVector + translationVector * mSpeed * dt;
	XMStoreFloat3(&mTranslation, positionVector);

	XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
	transformMatrix = transformMatrix * XMMatrixTranslationFromVector(XMLoadFloat3(&translation) * timeScale * mSpeed * dt);
	XMStoreFloat4x4(&transform, transformMatrix);

		UpdateChildren();
}

void GameObject::RotateEntity(const XMFLOAT3 & rotation, const float& timeScale, const float & dt)
{
		XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&right), rotation.x * timeScale * rotspeed * dt) * XMMatrixRotationAxis(XMLoadFloat3(&up), rotation.y * timeScale * rotspeed * dt) * XMMatrixRotationAxis(XMLoadFloat3(&forward), rotation.z * timeScale * rotspeed * dt);

		const XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
	rotationMatrix = rotationMatrix * transformMatrix;
	XMStoreFloat4x4(&transform, rotationMatrix);

		SetDirection();
	UpdateChildren();
}
void GameObject::RotateAroundPoint(const XMFLOAT3& point, const XMFLOAT3& rotation, const float& timeScale, const float& dt)
{
	const XMMATRIX translation1 = XMMatrixTranslationFromVector(-XMLoadFloat3(&point));
	const XMMATRIX rotation1 = XMMatrixRotationX(rotation.x * timeScale * rotspeed * dt) * XMMatrixRotationY(rotation.y * timeScale * rotspeed * dt) * XMMatrixRotationZ(rotation.z * timeScale * rotspeed * dt);
	const XMMATRIX translation2 = XMMatrixTranslationFromVector(XMLoadFloat3(&point));

	XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
	transformMatrix = transformMatrix * translation1 * rotation1 * translation2;
	XMStoreFloat4x4(&transform, transformMatrix);
		
	mTranslation = XMFLOAT3(transform._41, transform._42, transform._43);

	SetDirection();
	UpdateChildren();
}

void GameObject::SetPosition(const XMFLOAT3 & position)
{
	mTranslation = position;
	SetTransform();
	UpdateChildren();
}

void GameObject::ResetRotation()
{
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixScaling(scale.x, scale.y, scale.z);
	transformMatrix *= XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z);
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&mTranslation));	
	XMStoreFloat4x4(&transform, transformMatrix);
	UpdateChildren();
}

void GameObject::SetSpeed(const float & speed)
{
	mSpeed = speed;
}

void GameObject::SetRotationSpeed(const float & speed)
{
	rotspeed = speed;
}

void GameObject::RemoveInstancesFromShape(const string& name, const vector<InstanceData>& instances)
{
	for (auto& shape : shapes)
	{
		if (shape.Name() == name)
		{
			shape.RemoveInstances(instances);
		}
	}
}

std::vector<InstanceData> GameObject::GetInstances() const
{
	std::vector<InstanceData> allInstances;

	// Loop over each shape in the GameObject
	for (const auto& shape : shapes)
	{
		// Get instances from this shape
		std::vector<InstanceData> shapeInstances = shape.Instances();

		// Add instances from this shape to allInstances
		allInstances.insert(allInstances.end(), shapeInstances.begin(), shapeInstances.end());
	}

	return allInstances;
}


bool operator==(const GameObject& a, const GameObject& b)
{
	return (a.Name() == b.Name());
}
