#ifndef PICTUREBOX
#define PICTUREBOX

//My Includes
#include "IControl.h"
#include "../Graphics/Direct3D.h"
#include "../Input/Input.h"
#include "../Graphics/2D/Sprite.h"
#include "../Graphics/2D/Forms/Rectangle.h"

//////////////////////////////////////////////////////////////////////////
//Class: PictureBox
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class PictureBox : public IControl
	{
	public:
		PictureBox(Direct3D* direct3D, std::string Name, DirectX::XMINT2 Pos, std::string textureFileName, int screenWidth, int screenHeight);
		PictureBox(Direct3D* direct3D, std::string Name, DirectX::XMINT2 Pos, DirectX::XMFLOAT2 Size, std::string textureFileName, int screenWidth, int screenHeight);
		PictureBox(Direct3D* direct3D, std::string Name, Hydro::Rectangle rect, std::string textureFileName, int screenWidth, int screenHeight);
		~PictureBox();

		bool Update(float eTime);
		bool Draw(float eTime, ID3D11DeviceContext *deviceContext, ShaderManager *shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);

		void HandleInput(Input *input);
		void SetPosition(ID3D11DeviceContext * deviceContext, DirectX::XMINT2 pos);

	private:
		Sprite sprite;
	};
}

#endif // !PICTUREBOX

