//vertex shader
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
};

float heightMap(float2 uv)
{
	float texColour;
	texColour = texture0.SampleLevel(sampler0, uv, 0);
	return texColour;
}

OutputType main(InputType input)
{
	OutputType output;

	//input.position.y = (padding.x * sin((padding.y * input.position.x) + (time * padding.z))); //+ padding.x * cos((padding.y * input.normal.z) + (time * padding.z)));


	//input.normal = float3(padding.x * (-cos((padding.y * input.position.x) + (time * padding.z))), 1, 0);// +float3(padding.x * (-cos((padding.y * input.position.z) + (time * padding.z))), 1, 0);

    input.position.y += heightMap(input.tex) * 40;


	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
		
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}