#include "Header.hlsli"

// structure for light parameters from program
cbuffer Light {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float3 eyePos;
	bool useLight;
};

// Pixel colour data and sampler
Texture2D imageData : register(t1);
SamplerState colourSampler : register(s1);

float4 main(VS_OUTPUT input) : SV_Target
{
	// Get colour from texture as normal
	float4 pixColour = imageData.Sample(colourSampler, input.texCoord);

	[flatten]
	if (useLight)
	{
		// Normalise normal into range 0 to 1
		input.normal = normalize(input.normal);

		// Calculate the distance from the eye (camera) to the vertex
		float toEye = normalize(eyePos - input.PosW);

		// Calculate ambient factor
		float4 a = ambient;

		// Calculate the diffuse factor
		float d = dot(-direction, input.normal);

		// Now calculate the amount of specular between vertex and eye
		float3 v = reflect(direction, input.normal);
		float s = pow(max(dot(v, toEye), 0.0f), specular.w);

		// Calculate the final intensity
		float4 intensity = a + d * diffuse + s * specular;

		// Calculate the final intensity of the pixel
		pixColour = float4(pixColour.rgb * intensity.rgb, pixColour.a);
	}

return pixColour;
}
