#ifndef TEXTURE
#define TEXTURE

//Includes
#include <d3d11.h>
#include <stdio.h>

//My Includes
#include "../../Util/Globals.h"

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
		Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fileName);
		~Texture();

		//bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fileName);
		//void Shutdown();

		ID3D11ShaderResourceView* GetTexture();
		int GetTextureWidth() const;
		int GetTextureHeight() const;

	private:
		bool LoadTarga(std::string fileName, int &width, int &height);  //Loading *.tga files
																		//TODO Load other formats and add the relevant method
	private:
		unsigned char* pTargaData;
		int width, height;
		ID3D11Texture2D* pTexture;
		ID3D11ShaderResourceView* pTextureView;
	};
}
#endif // !TEXTURE

