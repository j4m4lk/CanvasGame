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
	float3 InstancePos : INSTANCEPOSITION;
	int ID : INSTANCEID;

};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float3 PosWorld : TEXCOORD2;

};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	input.Pos.x += input.InstancePos.x;
	input.Pos.y += input.InstancePos.y;
	input.Pos.z += input.InstancePos.z;

	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	return output;

}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	float4 finalColour = float4(1.0f, 0.0f, 0.0f, 1.0f);  // Red color

	float3 viewDir = normalize(vCameraPos - input.PosWorld);


	

	finalColour.a = 1;
	return finalColour;
}
