#include "Header.hlsli"


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
// Add the following source code to define the parameters for the vertex shader
// to include the normal
VS_OUTPUT main(float4 Pos : POSITION, float2 texCoord : TEXCOORD0, float3 normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT) 0; // Create a new vertex to store transformed vertex + colour

	output.Pos = mul(Pos, World); // Transform vertex with respect to mesh world co-ords

	//Need position in world before frustum applied for specular light
	output.PosW = output.Pos.xyz;

	output.Pos = mul(output.Pos, View); // Now transform vertex with respect to the view
	output.Pos = mul(output.Pos, Projection); // Now transform with respect to field of view

	output.texCoord = texCoord; // uv coord for sampler

	//Calculate the normal with respect
	// to model transformation.
	output.normal = mul(normal, World);

	return output;
}