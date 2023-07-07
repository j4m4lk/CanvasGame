//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix WorldInverse;
	matrix View;
	matrix Projection;
	float4 vLightPos[4];
	float4 vLightColour[4];
	float4 vCameraPos;
	float numOfLights;
	float time;
	float2 padding;
}

Texture2D txColour : register(t0);
Texture2D txBump : register(t1);
Texture2D txHeight : register(t2);
SamplerState txSampler : register(s0);


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float3 Tan : TANGENT;
	float3 BiTan : BITANGENT;
	float3 InstancePos : INSTANCEPOSITION;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
    float3 Norm : TEXCOORD1;
	float3 PosWorld : TEXCOORD2;
	float3x3 TBN : TEXCOORD5;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	input.Pos.x += input.InstancePos.x;
	input.Pos.y += input.InstancePos.y;
	input.Pos.z += input.InstancePos.z;
	input.Pos.w = 1;

    PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = normalize(input.Norm);
	output.PosWorld = mul(input.Pos, World).xyz;
	output.Tex = input.Tex;
	float3 tangent = mul(input.Tan, World).xyz;
	tangent = normalize(tangent);
	float3 biTangent = mul(input.BiTan, World).xyz;
	biTangent = normalize(biTangent);

	output.TBN = float3x3(
		tangent,
		biTangent,
		output.Norm
		);

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	float4 finalColour = float4(1.0f, 0.0f, 0.0f, 1.0f);  // Red color

	float3 viewDir = normalize(vCameraPos - input.PosWorld);
	float3 viewDirT = normalize(mul(input.TBN, viewDir));

	for (int i = 0; i < numOfLights; i++)
	{
		float3 lightDir = normalize(vLightPos[i] - input.PosWorld);
		float3 lightDirT = normalize(mul(input.TBN, lightDir));
		float3 reflection = normalize(reflect(-lightDirT, input.Norm));

		float diffuse = saturate(dot(lightDirT, input.Norm));
		float specular = pow(saturate(dot(viewDirT, reflection)), 0.8 * 128);
		if (diffuse <= 0)
		{
			specular = 0;
		}

		finalColour += saturate((diffuse * vLightColour[i]) + (specular * vLightColour[i]));
	}

	finalColour.a = 1;
	return finalColour;
}
