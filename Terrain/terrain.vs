cbuffer  MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType TerrainVertexShader(VertexInputType input)
{
	PixelInputType output;

	//Set Position Vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	
	//Calculate vertex position against the matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Send Tex Coords to Pixel Shader
	output.tex = input.tex;

	//Get the normal vector against world matrix
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//Normalize the normal
	output.normal = normalize(output.normal);

	return output;
}