////--------------------------------------------------------------------------------------
//// Constant Buffer Variables
////--------------------------------------------------------------------------------------
//cbuffer ConstantBuffer : register(b0)
//{
//    matrix World;
//    matrix WorldInverse;
//    matrix View;
//    matrix Projection;
//    float4 vCameraPos;
//    float time;
//    float2 padding;
//    float4 cubeId;
//    float4 colorToApply;
//    float4 changeColor;
//    float4 originalColor;
//}
//
//
//
//Texture2D txColour : register(t0);
//Texture2D txBump : register(t1);
//Texture2D txHeight : register(t2);
//SamplerState txSampler : register(s0);
//
//
//
//
////--------------------------------------------------------------------------------------
//struct VS_INPUT
//{
//    float4 Pos : POSITION;
//    float3 Norm : NORMAL;
//    float2 Tex : TEXCOORD;
//    float3 Tan : TANGENT;
//    float3 BiTan : BITANGENT;
//    float3 InstancePos : INSTANCEPOSITION;
//};
//
//
//
//struct PS_INPUT
//{
//    float4 Pos : SV_POSITION;
//    float2 Tex : TEXCOORD0;
//    float3 Norm : TEXCOORD1;
//    float3 PosWorld : TEXCOORD2;
//    float3x3 TBN : TEXCOORD5;
//};
//
//
//
//
////--------------------------------------------------------------------------------------
//// Vertex Shader
////--------------------------------------------------------------------------------------
//PS_INPUT VS(VS_INPUT input)
//{
//    input.Pos.x += input.InstancePos.x;
//    input.Pos.y += input.InstancePos.y;
//    input.Pos.z += input.InstancePos.z;
//    input.Pos.w = 1;
//
//
//
//    PS_INPUT output = (PS_INPUT)0;
//
//    output.Pos = mul(input.Pos, World);
//    output.Pos = mul(output.Pos, View);
//    output.Pos = mul(output.Pos, Projection);
//    output.Norm = normalize(input.Norm);
//    output.PosWorld = mul(input.Pos, World).xyz;
//    output.Tex = input.Tex;
//
//    float3 tangent = mul(input.Tan, World).xyz;
//    tangent = normalize(tangent);
//    float3 biTangent = mul(input.BiTan, World).xyz;
//    biTangent = normalize(biTangent);
//
//
//
//    output.TBN = float3x3(
//        tangent,
//        biTangent,
//        output.Norm
//        );
//
//
//
//    return output;
//}



//
////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------
//float4 PS(PS_INPUT input) : SV_Target
//{
//    float4 finalColour;
//   //float4 originalColor = float4(1.0f, 0.0f, 0.0f, 1.0f); // RGBA for red
//  //  float4 colorToApply = float4(1.0f, 1.0f, 1.0f, 1.0f); // RGBA for red
//   
//    if (changeColor.x == 1)
//    {
//        finalColour = colorToApply;
//    }
//
//    else
//    {
//        finalColour = originalColor;
//    }
//
//    return finalColour;
//}










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
	float4 originalColor : ORIGINALCOLOR;  // Changed
	float4 hitColor : HITCOLOR;  // Added
	float3 InstancePos : INSTANCEPOSITION;
	int isHit : ISHIT;  // Added
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

	if (input.isHit == 1)  // Checking if isHit is 1 instead of true
	{
		output.colorToApply = float4(1.0f, 1.0f,1.0f, 1.0f); // Use the hit color if the cube has been hit
	}
	else
	{
		output.colorToApply = input.originalColor; // Use the original color if the cube has not been hit
	}
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColour = input.colorToApply; // Use the color that was decided in the vertex shader

	// other code...

	return finalColour;
}
