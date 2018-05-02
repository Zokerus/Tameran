#ifndef TEXTHEADER
#define TEXTHEADER

//Includes
#include <DirectXColors.h>
#include <string>

//My Includes
#include "Font.h"
#include "../Shader/ShaderManager.h"

//////////////////////////////////////////////////////////////////////////
//Class: Text
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Text
	{
	public:
		Text(Direct3D* direct3D, Font* inFont, int ScreenWidth, int ScreenHeight, int MaxLength, bool Shadow, const std::string* Text, DirectX::XMINT2 Pos, DirectX::XMVECTORF32 Color);
		~Text();

		bool Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);
		bool Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, DirectX::XMVECTORF32 foreColor);
	
		void SetText(const std::string* Text);
		void SetPosition(DirectX::XMINT2 Pos);
		void SetFont(Font* inFont);
		void SetColor(DirectX::XMVECTORF32 Color);

		const Font* GetFont() const;
		const std::string* GetText() const;
	private:
		bool UpdateSentence(ID3D11DeviceContext* deviceContext);
		bool RenderSentence(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, DirectX::XMVECTORF32 foreColor);
	
	private:
		ID3D11Buffer *vertexBuffer, *indexBuffer, *vertexBuffer2, *indexBuffer2;
		DirectX::XMVECTORF32 pixelColor;
		DirectX::XMINT2 pos;
		int screenWidth, screenHeight, maxLength, vertexCount, indexCount;
		bool shadow, update;
		Font* font;
		std::string text;
	};
}

#endif // !TEXTHEADER

