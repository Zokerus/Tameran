#include "Header.hlsli"

//Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
	float4 color;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	color = shaderTexture.Sample(SampleType, input.tex);

	//If the color is black on the texture then treat this pixel as transparent
	//TODO change transparency color to (255,0,255) or Alpha channel to 0
	if (color.r == 0.0f && color.g == 0.0f && color.b == 0.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
		color.a = 1.0f;
		color = color * pixelColor;
	}

	return color;
}