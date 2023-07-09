//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix WorldInverse;
	matrix View;
	matrix Projection;
	float4 vCameraPos;
	float time;
	int ID;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 colorToApply : COLORTOAPPLY;  // New field
	float3 InstancePos : INSTANCEPOSITION;
	int ID : INSTANCEID;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 colorToApply : COLORTOAPPLY;  // New field
	float3 PosWorld : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	input.Pos.x += input.InstancePos.x;
	input.Pos.y += input.InstancePos.y;
	input.Pos.z += input.InstancePos.z;

	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.colorToApply = input.colorToApply;  // Pass the color to pixel shader

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColour = input.colorToApply;  // Use the color passed from vertex shader

	float3 viewDir = normalize(vCameraPos - input.PosWorld);

	finalColour.a = 1;
	return finalColour;
}
