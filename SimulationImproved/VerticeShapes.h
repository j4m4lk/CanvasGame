#pragma once

#include <string>
#include <vector>
#include <directxmath.h>
#include <Windows.h>
#include <algorithm>
#include <stdexcept>

using namespace DirectX;
using namespace std;


enum class MeshType
{
	CUBE
	
};


struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
	XMFLOAT3 Tangent;
	XMFLOAT3 BiTangent;
};

struct AABB
{
	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 extents;
};

struct InstanceData
{
	XMFLOAT3 Pos;
	XMFLOAT4 cubeId; // New field
	XMFLOAT4 originalColor;
	XMFLOAT4 hitColor;
	XMFLOAT4 color;
	AABB aabb;  // New field
	int isHit;
	int id;
	float mass;  // New field


	bool operator==(const InstanceData& a) const
	{
		return (fabs(a.Pos.x - Pos.x) < FLT_EPSILON && fabs(a.Pos.y - Pos.y) < FLT_EPSILON && fabs(a.Pos.z - Pos.z) < FLT_EPSILON);
	}
};

class VerticeShapes
{
private:
	vector<SimpleVertex> vertices;
	vector<WORD> indices;
	vector<InstanceData> mInstanceData;
	XMFLOAT4X4 transform;
	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	XMFLOAT3 wolrdPos;	
	wstring shader;
	string name;
	MeshType geometryType;
	
	

	void Cube();
	

public:
	VerticeShapes();
	VerticeShapes(const MeshType& geometryType, const XMFLOAT4X4& entityTransform, const XMFLOAT3& translation, const XMFLOAT3& rotation,
		const XMFLOAT3& scale, const wstring& shader,
		const string& name, const vector<InstanceData>* const instanceData);
	~VerticeShapes();

	const vector<SimpleVertex>& Vertices() const;
	const vector<WORD>& Indices() const;
	const vector<InstanceData>& Instances() const;	
	const wstring& Shader() const;
	const XMFLOAT3& WorldPos() const;
	const XMFLOAT4X4& Transform() const;
	const MeshType& Geometry() const;
	const string& Name() const;

	InstanceData& VerticeShapes::GetInstance(size_t index);
	// Add new methods to retrieve ID and mass of a cube by its index
	int GetCubeId(size_t index);
	float GetCubeMass(size_t index);
	std::size_t InstanceCount() const;
	void SetTransform(const XMFLOAT4X4& entityTransform);
	void RemoveInstances(const vector<InstanceData>& instancesToRemove);
};