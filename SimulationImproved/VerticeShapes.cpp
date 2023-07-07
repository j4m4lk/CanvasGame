#include "VerticeShapes.h"

using namespace std;
using namespace DirectX;


void VerticeShapes::Cube()
{	
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });

	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });

	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });

	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });

	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });

	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });

	indices.push_back(3);	indices.push_back(1);	indices.push_back(0);
	indices.push_back(2);	indices.push_back(1);	indices.push_back(3);
	indices.push_back(6);	indices.push_back(4);	indices.push_back(5);
	indices.push_back(7);	indices.push_back(4);	indices.push_back(6);
	indices.push_back(11);	indices.push_back(9);	indices.push_back(8);
	indices.push_back(10);	indices.push_back(9);	indices.push_back(11);
	indices.push_back(14);	indices.push_back(12);	indices.push_back(13);
	indices.push_back(15);	indices.push_back(12);	indices.push_back(14);
	indices.push_back(19);	indices.push_back(17);	indices.push_back(16);
	indices.push_back(18);	indices.push_back(17);	indices.push_back(19);
	indices.push_back(22);	indices.push_back(20);	indices.push_back(21);
	indices.push_back(23);	indices.push_back(20);	indices.push_back(22);
}



void VerticeShapes::Cylinder()
{
	const auto pointsOnCircle = 30;

	
	for (auto i = 0; i < pointsOnCircle; i++)
	{
		const auto fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const auto theta = 2 * XM_PI * fraction;

		
		XMFLOAT3 normal(sin(theta), 0.0f, cos(theta));
		XMFLOAT3 tangent(0, 1.0f, 0);
		XMFLOAT3 biTangent;
		XMStoreFloat3(&biTangent, XMVector3Cross(XMLoadFloat3(&normal), XMLoadFloat3(&tangent)));
		vertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), 1.0f, cos(theta)), normal, XMFLOAT2(fraction, 1.0f), tangent, biTangent });	

		
		tangent.y = -1.0f;
		XMStoreFloat3(&biTangent, XMVector3Cross(XMLoadFloat3(&normal), XMLoadFloat3(&tangent)));
		vertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), normal, XMFLOAT2(fraction, 0.0f), tangent, biTangent });
	}

	
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT3(0, 0, 1.0f) });

	
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });

	
	for (int i = 0; i < pointsOnCircle; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const float theta = 2 * XM_PI * fraction;

		
		vertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), 1.0f, cos(theta)), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(1.0f, 0, 0), XMFLOAT3(0, 0, 1.0f) });

		
		vertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });
	}

	
	for (int i = 0; i < (pointsOnCircle * 2); i += 2)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 3);
		indices.push_back(i + 2);
	}

	
	for (int i = 2; (i < pointsOnCircle * 2); i += 2)
	{
		
		indices.push_back(pointsOnCircle * 2);
		indices.push_back((pointsOnCircle * 2) + i);
		indices.push_back((pointsOnCircle * 2) + (i + 2));

	
		indices.push_back(pointsOnCircle * 2 + 1);
		indices.push_back((pointsOnCircle * 2 + 1) + (i + 2));
		indices.push_back((pointsOnCircle * 2 + 1) + i);
	}
}

void VerticeShapes::Cone()
{
	const int pointsOnCircle = 600;

	
	const float radius = 1;
	const float height = 2;
	const float sideLength = sqrtf(radius * radius + height * height);
	const float coneX = radius / sideLength;
	const float coneY = -height / sideLength;

	
	for (int i = 0; i < pointsOnCircle; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const float theta = 2 * XM_PI * fraction;
	
		vertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT3(-coneY * sin(theta), coneX, -coneY * cos(theta)), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT3(0, 0, 1.0f) });

		vertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), XMFLOAT3(-coneY * sin(theta), coneX, -coneY * cos(theta)), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });
	}

	
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });

	
	for (int i = 0; i < pointsOnCircle; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const float theta = 2 * XM_PI * fraction;

	
		vertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });
	}
	
	for (int i = 0; i < (pointsOnCircle * 2) - 2; i += 2)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 3);
	}
	indices.push_back(58);
	indices.push_back(59);
	indices.push_back(1);

	
	for (int i = 1; (i < pointsOnCircle); i++)
	{	
		indices.push_back(pointsOnCircle * 2);
		indices.push_back((pointsOnCircle * 2) + (i + 1));
		indices.push_back((pointsOnCircle * 2) + i);
	}
}

void VerticeShapes::Quad()
{
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	vertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	indices.push_back(3);
	indices.push_back(1);	
	indices.push_back(0);
	indices.push_back(2);	
	indices.push_back(1);	
	indices.push_back(3);
}

VerticeShapes::VerticeShapes() :
	geometryType(MeshType::CUBE),
	
	particlesSystem(0)
{
}

VerticeShapes::VerticeShapes(const MeshType& geometryType, const XMFLOAT4X4& entityTransform, const XMFLOAT3& translation, const XMFLOAT3& rotation,
	const XMFLOAT3& scale,  const wstring& shader, 
	const string& name, const vector<InstanceData>* const instanceData, const bool& particleSystem) :
	translation(translation),
	rotation(rotation),
	scale(scale),
	
	shader(shader),
	name(name),
	geometryType(geometryType),
	particlesSystem(particleSystem)
{

	SetTransform(entityTransform);
	if (instanceData)
	{
		mInstanceData = *instanceData;
	}

	switch (geometryType)
	{
	
	case MeshType::CUBE: Cube();
		break;	

	

	case MeshType::QUAD: Quad();
		break;
	}
}

VerticeShapes::~VerticeShapes()
{
}

const vector<SimpleVertex>& VerticeShapes::Vertices() const
{
	return vertices;
}

const vector<WORD>& VerticeShapes::Indices() const
{
	return indices;
}

const vector<InstanceData>& VerticeShapes::Instances() const
{
	return mInstanceData;
}

const wstring& VerticeShapes::DiffuseTexture() const
{
	return diffuseTexture;
}

const wstring& VerticeShapes::NormalMap() const
{
	return normalMap;
}

const wstring& VerticeShapes::DisplacementMap() const
{
	return displacementMap;
}

const wstring& VerticeShapes::Shader() const
{
	return shader;
}

const XMFLOAT3& VerticeShapes::WorldPos() const
{
	return wolrdPos;
}

const XMFLOAT4X4& VerticeShapes::Transform() const
{
	return transform;
}

const MeshType& VerticeShapes::Geometry() const
{
	return geometryType;
}

const string& VerticeShapes::Name() const
{
	return name;
}

const bool& VerticeShapes::ParticleSystem() const
{
	return particlesSystem;
}

void VerticeShapes::SetTransform(const XMFLOAT4X4& entityTransform)
{
	
	const XMMATRIX entityTransformMatrix = XMLoadFloat4x4(&entityTransform);
	XMMATRIX transformMatrix = XMMatrixIdentity();
	
	transformMatrix *= XMMatrixScaling(scale.x, scale.y, scale.z);
	transformMatrix *= XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z);
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&translation));
	transformMatrix *= entityTransformMatrix;

	XMStoreFloat4x4(&transform, transformMatrix);
	wolrdPos = XMFLOAT3(transform._41, transform._42, transform._43);
}

void VerticeShapes::RemoveInstances(const vector<InstanceData>& instancesToRemove)
{
	for (auto& instance : instancesToRemove)
	{
		mInstanceData.erase(remove(mInstanceData.begin(), mInstanceData.end(), instance), mInstanceData.end());
	}
}
