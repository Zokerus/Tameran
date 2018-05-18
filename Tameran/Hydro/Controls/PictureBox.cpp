#include "PictureBox.h"

Hydro::PictureBox::PictureBox(Direct3D* direct3D, std::string Name, DirectX::XMINT2 Pos, std::string textureFileName, int screenWidth, int screenHeight)
	: IControl(Name, DirectX::XMFLOAT2(0,0), Pos, false, true, true, false, DirectX::Colors::Black, ControlType::PictureBox), sprite(direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFileName)
{
	//Set the size of the picture box to  the limits of the image
	size = DirectX::XMFLOAT2((float)sprite.GetImageWidth(), (float)sprite.GetImageHeight());

	//Set the initial position of the pictureBox
	if (!sprite.Update(direct3D->GetDeviceContext(), pos))
	{
		throw std::exception(("Failed to position and create a sprite: " + textureFileName).c_str());
	}
}

Hydro::PictureBox::PictureBox(Direct3D * direct3D, std::string Name, DirectX::XMINT2 Pos, DirectX::XMFLOAT2 Size, std::string textureFileName, int screenWidth, int screenHeight)
	: IControl(Name, Size, Pos, false, true, true, false, DirectX::Colors::Black, ControlType::PictureBox), sprite(direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFileName)
{
	//Set the initial position of the pictureBox
	if (!sprite.Update(direct3D->GetDeviceContext(), pos))
	{
		throw std::exception(("Failed to position and create a sprite: " + textureFileName).c_str());
	}
}

Hydro::PictureBox::PictureBox(Direct3D * direct3D, std::string Name, Hydro::Rectangle rect, std::string textureFileName, int screenWidth, int screenHeight)
	: IControl(Name, DirectX::XMFLOAT2((float)rect.GetWidth(), (float)rect.GetHeight()), DirectX::XMINT2(rect.GetXPos(), rect.GetYPos()), false, true, true, false, DirectX::Colors::Black, ControlType::PictureBox), sprite(direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFileName)
{
	//Set the initial position of the pictureBox
	if (!sprite.Update(direct3D->GetDeviceContext(), pos))
	{
		throw std::exception(("Failed to position and create a sprite: " + textureFileName).c_str());
	}
}

Hydro::PictureBox::~PictureBox()
{
}

bool Hydro::PictureBox::Update(float eTime)
{
	return true;
}

bool Hydro::PictureBox::Draw(float eTime, ID3D11DeviceContext * deviceContext, ShaderManager * shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	bool result;

	//Prepare the picture box sprite for drawing
	result = sprite.Render(deviceContext);
	if (!result)
	{
		return false;
	}

	//Render the sprite object with the texture shader of the shader manager
	result = shaderManager->RenderTextureShader(deviceContext, sprite.GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, sprite.GetTexture());
	if (!result)
	{
		return false;
	}

	return true;
}

void Hydro::PictureBox::HandleInput(Input * input)
{
	return;
}

void Hydro::PictureBox::SetPosition(ID3D11DeviceContext* deviceContext, DirectX::XMINT2 pos)
{
	pos = pos;
	
	//Update the position of the sprite
	sprite.Update(deviceContext, pos);
}