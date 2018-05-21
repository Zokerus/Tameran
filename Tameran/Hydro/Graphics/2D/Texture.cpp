#include "Texture.h"
#include <exception>

Hydro::Texture::Texture(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, std::string _fileName, Rectangle& _srcRect)
	: pTargaData(nullptr), width(0), height(0), pTexture(nullptr), pTextureView(nullptr)
{
	//Load the targa image data into memory
	if (!LoadTarga(_fileName, width, height, _srcRect))
	{
		throw std::exception(("Failed to load texture data: " + _fileName).c_str());
	}

	if (!Initialize(_device, _deviceContext, _fileName))
	{
		throw std::exception(("Failed to create Texture2D: " + _fileName).c_str());
	}
}

Hydro::Texture::Texture(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, std::string _fileName)
	: pTargaData(nullptr), width(0), height(0), pTexture(nullptr), pTextureView(nullptr)
{
	//Load the targa image data into memory
	if (!LoadTarga(_fileName, width, height, Rectangle()))
	{
		throw std::exception(("Failed to load texture data: " + _fileName).c_str());
	}

	if (!Initialize(_device, _deviceContext, _fileName))
	{
		throw std::exception(("Failed to create Texture2D: " + _fileName).c_str());
	}
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

bool Hydro::Texture::Initialize(ID3D11Device * _device, ID3D11DeviceContext * _deviceContext, std::string _fileName)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 3;
	textureDesc.ArraySize = 4;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = _device->CreateTexture2D(&textureDesc, NULL, &pTexture);
	if (FAILED(hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	_deviceContext->UpdateSubresource(pTexture, 0, NULL, pTargaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = _device->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);
	if (FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	_deviceContext->GenerateMips(pTextureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] pTargaData;
	pTargaData = nullptr;

	return true;
}

bool Hydro::Texture::LoadTarga(std::string fileName, int& width, int& height, Rectangle& _srcRect)
{
	int error, bpp, srcImageSize, dstImageSize, index, i, j, k;
	FILE *filePtr;
	unsigned int count;
	TargaHeader targaHeader;
	unsigned char* targaImage;
	int _height, _width;

	//Open the targa file for reading in binary
	error = fopen_s(&filePtr, fileName.c_str(), "rb");
	if (error != 0)
		return false;

	//Read in the file header
	count = (unsigned int)fread(&targaHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
		return false;
	//temporary storage for loops
	_height = (int)targaHeader.height;
	_width = (int)targaHeader.width;

	if (_srcRect.GetWidth() <= 0 || _srcRect.GetHeight() <= 0)
	{
		//Get the important information from the header
		height = (int)targaHeader.height;
		width = (int)targaHeader.width;
		bpp = (int)targaHeader.bpp;
	}
	else if (_srcRect.GetWidth() > 0 && _srcRect.GetHeight() > 0)
	{
		//Get the important information from the header
		height = _srcRect.GetHeight();
		width = _srcRect.GetWidth();
		bpp = (int)targaHeader.bpp;

		if ((int)targaHeader.height < height || (int)targaHeader.width < width)
		{
			return false;
		}

		if (_srcRect.GetXPos() < 0 && _srcRect.GetXPos() >= _width && _srcRect.GetYPos() < 0 && _srcRect.GetYPos() >= height)
		{
			//Rectangle is out og boundaries
			return false;
		}
	}

	//Check that it is 32 bit and not 24 bit

	if (bpp != 32)
	{
		return false;
	}

	//Calculate the size of the 32 bit image data
	srcImageSize = _width * _height * 4;
	dstImageSize = width * height * 4;

	//Allocate memomry for the targa image data
	targaImage = new unsigned char[srcImageSize];
	if (!targaImage)
	{
		return false;
	}

	//Read in the targa image
	count = (unsigned int)fread(targaImage, 1, srcImageSize, filePtr);
	if (count != srcImageSize)
	{
		return false;
	}

	//Close the file
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}
	
	//Allocate memory for the targa destination data
	pTargaData = new unsigned char[dstImageSize];
	if (!pTargaData)
		return false;

	//Initialize the index into the targa destination data array
	index = 0;

	// Initialize the index into the targa image data.
	k = ((_height - _srcRect.GetYPos() - 1) * _width * 4) + _srcRect.GetXPos() * 4;

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			pTargaData[index + 0] = targaImage[k + 2];  // Red.
			pTargaData[index + 1] = targaImage[k + 1];  // Green.
			pTargaData[index + 2] = targaImage[k + 0];  // Blue
			pTargaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (_width + width) * 4;
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = nullptr;

	return true;
}
