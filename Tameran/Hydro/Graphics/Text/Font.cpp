#include "Font.h"

Hydro::Font::Font()
	: m_font(nullptr), m_texture(), m_fontHeight(0), m_spaceSize(0)
{}

Hydro::Font::~Font()
{
	Shutdown();
}

bool Hydro::Font::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fontFilename, float fontHeight, int spaceSize)
{
	bool result;

	//Store the size of the font
	m_fontHeight = fontHeight;

	//Store the size of spaces in pixel size
	m_spaceSize = spaceSize;

	//Load the text filecontaining the font data
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	//Load the texture that has the font characters on it
	result = LoadTexture(device, deviceContext, fontFilename);
	if (!result) 
	{
		return false;
	}

	return true;
}

void Hydro::Font::Shutdown()
{
	//Release the font texture
	m_texture.Shutdown();
	m_texture.~Texture();

	//Release the font data
	ReleaseFontData();
}

ID3D11ShaderResourceView * Hydro::Font::GetTexture()
{
	return m_texture.GetTexture();
}

void Hydro::Font::BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY)
{
	VertexType *vertexPtr;
	int numLetters, index, i, letter;
	float ratio;

	//Calculate the height/width ratio
	ratio = m_fontHeight / 18.0f;

	//Cast the input vertices into a VertexType structure
	vertexPtr = (VertexType*)vertices;

	//Get the number of letters in the sentence
	numLetters = (int)strlen(sentence);

	//Initialize the index of the vertex array
	index = 0;

	//Draw each letter onto a quad
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		//if the letter is a space then just move over three pixels
		if (letter == 0)
			drawX = drawX + (float)m_spaceSize * ratio;
		else
		{
			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, (drawY - m_fontHeight), 0.0f);  // Bottom left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3((drawX + m_font[letter].size * ratio), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX + m_font[letter].size * ratio, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].right, 0.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_font[letter].size * ratio + 1.0f;
		}
	}
}

float Hydro::Font::GetTextWidth(const char * sentence) const
{
	int numLetters, i, letter;
	float ratio, width;

	//Initialize the width
	width = 0.0f;

	//Calculate the height/width ratio
	ratio = m_fontHeight / 18.0f;

	//Get the number of letters in the sentence
	numLetters = (int)strlen(sentence);

	//Messure the width of the sentence
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		//if the letter is a space then just move over three pixels
		if (letter == 0)
			width = width + (float)m_spaceSize * ratio;
		else
		{
			// Update the x location for drawing by the size of the letter and one pixel.
			width = width + m_font[letter].size * ratio + 1.0f;
		}
	}

	return width;
}

float Hydro::Font::GetFontHeight() const
{
	return m_fontHeight;
}

bool Hydro::Font::LoadFontData(std::string filename)
{
	std::ifstream fin;
	int i;
	char temp;

	//Create the font spacing buffer
	m_font = new FontType[95];  //TODO dynamic number of letters
	if (!m_font)
		return false;

	//Refer to the working directory
	std::string path = sDir + filename + ".txt";

	//Read in the font 
	fin.open(path.c_str());
	if (fin.fail())
		return false;

	//Read in the 95 used ascii characters for the text
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	//Close the file
	fin.close();

	return true;
}

void Hydro::Font::ReleaseFontData()
{
	//Release the font data array.
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}
}

bool Hydro::Font::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string filename)
{
	bool result;

	//Initialize the texture object
	result = m_texture.Initialize(device, deviceContext, filename + ".tga");
	if (!result)
	{
		return false;
	}

	return true;
}
