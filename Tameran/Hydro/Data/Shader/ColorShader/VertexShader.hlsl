#include "Header.hlsli"

PixelInputType main( VertexInputType input )
{
	PixelInputType output;

	//Change the 4 unit of the position vector for proper matrix calculation
	input.position.w = 1.0f;

	//Calculate the position of the vertex against the matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Store the input color for the pixel shader
	output.color = input.color;

	return output;
}