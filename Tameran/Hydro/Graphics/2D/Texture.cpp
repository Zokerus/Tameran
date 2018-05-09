#include "Texture.h"
#include <exception>

Hydro::Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fileName)
	: pTargaData(nullptr), width(0), height(0), pTexture(nullptr), pTextureView(nullptr)
{
	bool result;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//Load the targa image data into memory
	result = LoadTarga(fileName, width, height);
	if (!result)
	{
		throw std::exception(("Failed to load texture data: " + fileName).c_str());
	}

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &pTexture);
	if (FAILED(hResult))
	{
		throw std::exception(("Failed to create Texture2D: " + fileName).c_str());
	}

	// Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(pTexture, 0, NULL, pTargaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);
	if (FAILED(hResult))
	{
		throw std::exception(("Failed to create ShaderResourceView of: " + fileName).c_str());
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(pTextureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] pTargaData;
	pTargaData = nullptr;
}

Hydro::Texture::~Texture()
{
	// Release the texture view resource.
	if (pTextureView)
	{
		pTextureView->Release();
		pTextureView = nullptr;
	}

	// Release the texture.
	if (pTexture)
	{
		pTexture->Release();
		pTexture = nullptr;
	}

	// Release the targa data.
	if (pTargaData)
	{
		delete[] pTargaData;
		pTargaData = nullptr;
	}
}

ID3D11ShaderResourceView* Hydro::Texture::GetTexture()
{
	return pTextureView;
}

int Hydro::Texture::GetTextureWidth() const
{
	return width;
}

int Hydro::Texture::GetTextureHeight() const
{
	return height;
}

bool Hydro::Texture::LoadTarga(std::string fileName, int & width, int & height)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE *filePtr;
	unsigned int count;
	TargaHeader targaHeader;
	unsigned char* targaImage;

	//Open the targa file for reading in binary
	error = fopen_s(&filePtr, fileName.c_str(), "rb");
	if (error != 0)
		return false;

	//Read in the file header
	count = (unsigned int)fread(&targaHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
		return false;

	//Get the important information from the header
	height = (int)targaHeader.height;
	width = (int)targaHeader.width;
	bpp = (int)targaHeader.bpp;

	//Check that it is 32 bit and not 24 bit

	if (bpp != 32)
		return false;

	//Calculate the size of the 32 bit image data
	imageSize = width * height * 4;

	//Allocate memomry for the targa image data
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
		return false;

	//Read in the targa image
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
		return false;

	//Close the file
	error = fclose(filePtr);
	if (error != 0)
		return false;

	//Allocate memory for the targa destination data
	pTargaData = new unsigned char[imageSize];
	if (!pTargaData)
		return false;

	unsigned char data[4096];

	//Initialize the index into the targa destination data array
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width; i++)
		{
			pTargaData[index + 0] = targaImage[k + 2];  // Red.
			pTargaData[index + 1] = targaImage[k + 1];  // Green.
			pTargaData[index + 2] = targaImage[k + 0];  // Blue
			pTargaData[index + 3] = targaImage[k + 3];  // Alpha

			if (imageSize == 4096)
			{
				data[index + 0] = targaImage[k + 2];  // Red.
				data[index + 1] = targaImage[k + 1];  // Green.
				data[index + 2] = targaImage[k + 0];  // Blue
				data[index + 3] = targaImage[k + 3];  // Alpha
			}

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = nullptr;

	return true;
}
