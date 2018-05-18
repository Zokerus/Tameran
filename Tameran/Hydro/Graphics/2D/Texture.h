#ifndef TEXTURE
#define TEXTURE

//Includes
#include <d3d11.h>
#include <stdio.h>
#include <string>

//My Includes
#include "Forms/Rectangle.h"

//////////////////////////////////////////////////////////////////////////
//Class: Texture
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Texture
	{
	private:
		struct TargaHeader
		{
			unsigned char data1[12];
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char data2;
		};

	public:
		Texture(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, std::string _fileName, Rectangle& _srcRect);
		Texture(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, std::string _fileName);
		~Texture();

		ID3D11ShaderResourceView* GetTexture();
		int GetTextureWidth() const;
		int GetTextureHeight() const;

	private:
		bool Initialize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, std::string _fileName);
		bool LoadTarga(std::string fileName, int& width, int& height, Rectangle& _srcRect);  //Loading *.tga files
																		//TODO Load other formats and add the relevant method
	private:
		unsigned char* pTargaData;
		int width, height;
		ID3D11Texture2D* pTexture;
		ID3D11ShaderResourceView* pTextureView;
	};
}
#endif // !TEXTURE

