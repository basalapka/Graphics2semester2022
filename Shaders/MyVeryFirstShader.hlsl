struct VS_IN
{
	float4 pos : POSITION;
	float4 col : COLOR;
	// tex : TEXCOORD;
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
	/*float4 pos : SV_POSITION;
	float4 norm : NORMAL;
	float2 uv : TEXCOORD0;
	float4 worldPos: TEXCOORD1;*/
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	//float4 tex : TEXCOORD0;
	float4 worldPos : TEXCOORD1;
	float4 normal :NORMAL;
};
struct ConstantData
{
	float4x4 WorldViewProj;
	float4x4 World;
};

cbuffer ConstBuf:register(b0)
{
	ConstantData ConstData;
};

PS_IN VSMain(VS_IN input)
{
	//PS_IN output = (PS_IN)0;

	//output.pos = mul(input.pos, mat);
	//output.col = input.col;
	////output.normal = mul(float4(input.normal.xyz, 1.0f), invert(mat));;
	//output.normal = input.normal;

	//return output;

	PS_IN output = (PS_IN)0;
	output.pos = mul(float4(input.pos.xyz, 1.0f), ConstData.WorldViewProj);
	output.worldPos = mul(float4(input.pos.xyz, 1.0f), ConstData.World);
	output.col = input.col;
	output.normal = mul(float4(input.normal.xyz, 1.0f), ConstData.World);;
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{	 
	/*float4 color = input.col;
	const float3 ambient = Lights.Color.xyz * 0.5;

	const float3 normal = normalize(input.norm.xyz);
	const float diff = max(0.0, dot(-Lights.Direction.xyz, normal));
	const float3 diffuse = diff * Lights.Color.xyz;

	const float3 viewDirection = normalize(Lights.ViewerPos.xyz - input.worldPos.xyz);
	const float3 reflectDirection = normalize(reflect(Lights.Direction.xyz, normal));
	const float spec = pow(max(0.0, dot(viewDirection, reflectDirection)), 0.7);
	const float3 specular = 0.3 * spec * Lights.Color.xyz;

	float3 result = (ambient + diffuse + specular) * Lights.Color.xyz;
	return float4(result,1.0F);*/

	float4 color = input.col;
	float3 LightDir = Lights.Direction.xyz;
	float3 normal = normalize(input.normal.xyz);

	float3 viewDir = normalize(Lights.ViewerPos.xyz - input.worldPos.xyz);
	float3 refVec = normalize(reflect(LightDir, normal));

	float3 ambient = color.xyz * 0.5;
	/*const float diff = max(0.0, dot(-LightDir, normal));
	const float3 diffuse = diff * Lights.Color.xyz;*/
	float3 diffuse = saturate(dot(LightDir, normal)) * color.xyz;
	//float3 specular = pow(saturate(dot(-viewDir, refVec)), 0.7) * 0.3;
	float spec = pow(max(0.0, dot(viewDir, refVec)), 0.7);
	float3 specular = 0.3 * spec * Lights.Color.xyz;

	return float4(Lights.Color.xyz * (ambient + diffuse + specular), 1.0f);
  //return color;
}