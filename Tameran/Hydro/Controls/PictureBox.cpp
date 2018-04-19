#include "PictureBox.h"

Hydro::PictureBox::PictureBox()
	: IControl(), m_sprite(nullptr)
{}

Hydro::PictureBox::~PictureBox()
{
	if(m_ready)
	{
		Shutdown();
	}
}

bool Hydro::PictureBox::Initialize(Direct3D * direct3D, Hydro::Rectangle rect, std::string textureFileName, int screenWidth, int screenHeight)
{
	m_name = "Background";
	m_text = "";
	m_size = DirectX::XMFLOAT2((float)rect.GetWidth(), (float)rect.GetHeight());
	m_pos = DirectX::XMINT2(rect.GetXPos(), rect.GetYPos());

	//Create the sprite object of the picture box
	m_sprite = new Sprite(direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFileName, (int)m_size.x, (int)m_size.y);
	if (!m_sprite)
	{
		return false;
	}

	//Set the initial position of the pictureBox
	m_sprite->Update(direct3D->GetDeviceContext(), m_pos.x, m_pos.y);

	m_ready = true;

	return true;
}

bool Hydro::PictureBox::Initialize(Direct3D * direct3D, DirectX::XMINT2 pos, std::string textureFileName, int screenWidth, int screenHeight)
{
	m_name = "Background";
	m_text = "";
	m_pos = pos;

	//Create the sprite object of the picture box
	m_sprite = new Sprite(direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFileName);
	if (!m_sprite)
	{
		return false;
	}

	//Set the size of the picture box to  the limits of the image
	m_size = DirectX::XMFLOAT2((float)m_sprite->GetImageWidth(), (float)m_sprite->GetImageHeight());

	//Set the initial position of the pictureBox
	m_sprite->Update(direct3D->GetDeviceContext(), m_pos.x, m_pos.y);

	m_ready = true;

	return true;
}

void Hydro::PictureBox::Shutdown()
{
	//Shutdown the sprite object of the picture box
	if (m_sprite)
	{
		delete m_sprite;
		m_sprite = nullptr;
	}

	IControl::Shutdown();
	m_ready = false;
}

bool Hydro::PictureBox::Update(float eTime)
{
	return true;
}

bool Hydro::PictureBox::Draw(float eTime, ID3D11DeviceContext * deviceContext, ShaderManager * shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	bool result;

	//Prepare the picture box sprite for drawing
	result = m_sprite->Render(deviceContext);
	if (!result)
	{
		return false;
	}

	//Render the sprite object with the texture shader of the shader manager
	result = shaderManager->RenderTextureShader(deviceContext, m_sprite->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_sprite->GetTexture());
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
	m_pos = pos;
	
	//Update the position of the sprite
	m_sprite->Update(deviceContext, m_pos.x, m_pos.y);
}