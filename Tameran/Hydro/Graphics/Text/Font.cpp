#include "Font.h"
#include <exception>

Hydro::Font::Font(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string fontFilename, float FontHeight, int SpaceSize)
	: pFont(nullptr), texture(device, deviceContext, fontFilename + ".tga"), fontHeight(FontHeight), spaceSize(SpaceSize)
{
	bool result = true;

	//Load the text filecontaining the font data
	result = LoadFontData(fontFilename);
	if (!result)
	{
		throw std::exception(("Failed to load FontData from " + fontFilename).c_str());
	}
}

Hydro::Font::~Font()
{
	//Release the font data
	ReleaseFontData();
}

ID3D11ShaderResourceView* Hydro::Font::GetTexture()
{
	return texture.GetTexture();
}

void Hydro::Font::BuildVertexArray(void* vertices, const std::string* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;
	float ratio;

	//Calculate the height/width ratio
	ratio = fontHeight / 18.0f;

	//Cast the input vertices into a VertexType structure
	vertexPtr = (VertexType*)vertices;

	//Get the number of letters in the sentence
	numLetters = (int)sentence->length();

	//Initialize the index of the vertex array
	index = 0;

	//Draw each letter onto a quad
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence->at(i)) - 32;

		//if the letter is a space then just move over three pixels
		if (letter == 0)
			drawX = drawX + (float)spaceSize * ratio;
		else
		{
			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, (drawY - fontHeight), 0.0f);  // Bottom left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(pFont[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(pFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3((drawX + pFont[letter].size * ratio), (drawY - fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(pFont[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX + pFont[letter].size * ratio, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(pFont[letter].right, 0.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + pFont[letter].size * ratio + 1.0f;
		}
	}
}

float Hydro::Font::GetTextWidth(const std::string* sentence) const
{
	int numLetters, i, letter;
	float ratio, width;

	//Initialize the width
	width = 0.0f;

	//Calculate the height/width ratio
	ratio = fontHeight / 18.0f;

	//Get the number of letters in the sentence
	numLetters = (int)sentence->length();

	//Messure the width of the sentence
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence->at(i)) - 32;

		//if the letter is a space then just move over three pixels
		if (letter == 0)
			width = width + (float)spaceSize * ratio;
		else
		{
			// Update the x location for drawing by the size of the letter and one pixel.
			width = width + pFont[letter].size * ratio + 1.0f;
		}
	}

	return width;
}

float Hydro::Font::GetFontHeight() const
{
	return fontHeight;
}

bool Hydro::Font::LoadFontData(const std::string filename)
{
	std::ifstream fin;
	int i;
	char temp;

	//Create the font spacing buffer
	pFont = new FontType[95];  //TODO dynamic number of letters
	if (!pFont)
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

		fin >> pFont[i].left;
		fin >> pFont[i].right;
		fin >> pFont[i].size;
	}

	//Close the file
	fin.close();

	return true;
}

void Hydro::Font::ReleaseFontData()
{
	//Release the font data array.
	if (pFont)
	{
		delete[] pFont;
		pFont = nullptr;
	}
}