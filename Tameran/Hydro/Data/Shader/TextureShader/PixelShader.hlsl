#include "Header.hlsli"

//Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

// Sample the pixel color from the texture using the sampler at this texture coordinate location.
textureColor = shaderTexture.Sample(SampleType, input.tex);

//If the color is black on the texture then treat this pixel as transparent
//if (textureColor.a == 0.0f)
//{
//	textureColor.r = 1.0f;
//}
//else
//{
//	// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
//	textureColor.g = 1.0f;
//}

return textureColor;
}