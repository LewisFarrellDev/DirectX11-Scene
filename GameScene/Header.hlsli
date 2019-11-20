//--------------------------------------------------------------------------------------
// Common structure to pass vertex and texture data
// through graphics pipeline
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBufferForObject
{
	matrix World;
	matrix View;
	matrix Projection;
};

//Structure to store transformed vertex
struct VS_OUTPUT
{
	float4 Pos: SV_POSITION;
	float3 PosW : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal :	NORMAL;
};
