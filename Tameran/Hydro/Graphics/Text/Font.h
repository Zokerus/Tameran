#ifndef FONT
#define FONT

//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <string>

//My Includes
#include "../2D/Texture.h"

//////////////////////////////////////////////////////////////////////////
//Class: Font
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Font
	{
	private:
		struct FontType
		{
			float left, right;
			int size;
		};

	public:
		struct VertexType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texture;
		};

	public:
		Font(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string fontFilename, float FontHeight, int SpaceSize);
		~Font();

		ID3D11ShaderResourceView* GetTexture();
		void BuildVertexArray(void* vertices, const std::string* sentence, float drawX, float drawY);
		float GetTextWidth(const std::string* sentence) const;
		float GetFontHeight() const;

	private:
		bool LoadFontData(const std::string filename);
		void ReleaseFontData();

	private:
		FontType* pFont;
		Texture texture;
		float fontHeight;
		int spaceSize;
	};
}

#endif // !FONT

