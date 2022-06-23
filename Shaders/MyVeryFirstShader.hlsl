struct VS_IN
{
	float4 pos : POSITION;
	float4 col : COLOR;
	float4 normal : NORMAL;
};
struct LightData
{
	float4 Direction;
	float4 Color;
	float4 ViewerPos;
	//float4 Normals;
};

cbuffer LightsBuf:register(b1)
{
	LightData Lights;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float4 worldPos : TEXCOORD1;
	float4 normal :NORMAL;
};
struct ConstantData
{
	float4x4 WorldViewProj;
	float4x4 World;
	float4x4 invertedWorldTransform;
};

cbuffer ConstBuf:register(b0)
{
	ConstantData ConstData;
};

PS_IN VSMain(VS_IN input)
{

	PS_IN output = (PS_IN)0;
	output.pos = mul(float4(input.pos.xyz, 1.0f), ConstData.WorldViewProj);
	output.worldPos = mul(float4(input.pos.xyz, 1.0f), ConstData.World);
	output.col = input.col;
	output.normal = mul(float4(input.normal.xyz, 1.0f), ConstData.invertedWorldTransform);
	return output;
	/*output.pos = mul(float4(input.pos, 1.0), Transform.world_view_proj);
	output.norm = mul(float4(input.norm, 0.0), Transform.inverse_transpose_world);
	output.tex = input.tex;
	output.world_pos = mul(float4(input.pos, 1.0), Transform.world);*/
}

float4 PSMain(PS_IN input) : SV_Target
{	 

	float4 color = input.col;

	float3 LightDir = Lights.Direction.xyz;
	float3 normal = normalize(input.normal.xyz);

	float3 viewDir = normalize(Lights.ViewerPos.xyz - input.worldPos.xyz);
	float3 refVec = normalize(reflect(LightDir, normal));

	float3 ambient = color.xyz * 0.5;
	float3 diffuse = saturate(dot(LightDir, normal)) * color.xyz;
	float3 specular = pow(saturate(dot(-viewDir, refVec)), 0.7) * 0.3;

	return float4(Lights.Color.xyz * (ambient + diffuse + specular), 1.0f);
}