#include "Lighting.h"

using namespace DirectX;
using namespace std;


Lighting::Lighting() :
	translation(XMFLOAT4(0, 0, 0, 0)),
	color(XMFLOAT4(0, 0, 0, 0))
{
}

Lighting::Lighting(const XMFLOAT4X4& entityTransform, const XMFLOAT4& lightPos, const XMFLOAT4& lightColour, const string& name):
	translation(lightPos),
	color(lightColour),
	name(name)
{
	SetTransform(entityTransform);
}

Lighting::~Lighting()
{
}


const XMFLOAT4& Lighting::Position() const
{
	return wolrdpos;
}


const XMFLOAT4& Lighting::Colour() const
{
	return color;
}


const string & Lighting::Name() const
{
	return name;
}


void Lighting::SetTransform(const XMFLOAT4X4 & entityTransform)
{	
	const XMMATRIX entityTransformMatrix = XMLoadFloat4x4(&entityTransform);
	XMMATRIX transformMatrix = XMMatrixIdentity();	
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat4(&translation));
	transformMatrix *= entityTransformMatrix;	
	XMStoreFloat4x4(&transform, transformMatrix);
	wolrdpos = XMFLOAT4(transform._41, transform._42, transform._43, 1.0f);
}


void Lighting::SetColour(const XMFLOAT4 & colour)
{
	color = colour;
}
