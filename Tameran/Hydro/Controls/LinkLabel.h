#ifndef LINKLABEL
#define LINKLABEL

//Includes
#include <functional>

//My Includes
#include "Label.h"

//////////////////////////////////////////////////////////////////////////
//Class: LinkLabel
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class LinkLabel : public Label
	{
	public:
		LinkLabel(Direct3D* direct3D, Font* font, int ScreenWidth, int ScreenHeight, int MaxLength, std::string Name, std::string text, DirectX::XMINT2 Pos, DirectX::XMFLOAT2 Size, DirectX::XMVECTORF32 Color, DirectX::XMVECTORF32 selectedColor, std::function<void(IControl*)> func);
		~LinkLabel();

		bool Update(float eTime);
		bool Draw(float eTime, ID3D11DeviceContext *deviceContext, ShaderManager *shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);
	
		void HandleInput(Input *input);

	private:
		DirectX::XMVECTORF32 selectedColor;
		int counter;
		std::function<void(IControl*)> select;
	};
}
#endif // !LINKLABEL

