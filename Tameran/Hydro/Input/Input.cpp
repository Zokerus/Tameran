#include "Input.h"
#include <exception>

Hydro::Input::Input(unsigned int ScreenWidth, unsigned int ScreenHeight, HINSTANCE hinstance, HWND hwnd)
	: pDirectInput(nullptr), pKeyboard(nullptr), pMouse(nullptr), mouseState(), screenWidth(ScreenWidth), screenHeight(ScreenHeight), mouseX(0), mouseY(0)
{
	HRESULT result = false;
	int i = 0;

	//Initialize the main direct input device
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, NULL);
	if (FAILED(result))
	{
		throw std::exception("Failed to initialize Input driver");
	}

	//Initialize the direct input interface for the keyboard
	result = pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL);
	if (FAILED(result))
	{
		throw std::exception("Failed to initialize keyboard device");
	}

	//Set the data format, in this case is it a keyboard, we can use the predefined data format
	result = pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		throw std::exception("Failed to set keyboard format");
	}

	//Set the cooperative level of the keyboard to not share with other programs
	result = pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		throw std::exception("Failed to set cooperation level of the keyboard device");
	}

	//Now acquire the keyboard
	result = pKeyboard->Acquire();
	if (FAILED(result))
	{
		throw std::exception("Failed to aquire keyboard device");
	}

	//Initialize the direct input device for the mouse
	result = pDirectInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result))
	{
		throw std::exception("Failed to initialize mouse device");
	}

	//Set the data format for the mouse using the pre-defined mouse data format.
	result = pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		throw std::exception("Failed to set mouse format");
	}

	//Set the cooperative level of the mouse to share with other programs
	result = pMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		throw std::exception("Failed to set cooperation level of the mouse device");
	}

	//Acquire the mouse
	result = pMouse->Acquire();
	if (FAILED(result))
	{
		throw std::exception("Failed to aquire mouse device");
	}

	//Initialize the released key state
	for (i = 0; i < 256; i++)
		releasedKeyboardState[i] = true;

	//Read the current state of the keyboard
	result = ReadKeyboard();
	if (!result)
	{
		throw std::exception("Failed to read initial keyboard data");
	}

	//Store the keyboardstate from last frame
	memcpy(lasKeyboradState, keyboardState, sizeof(lasKeyboradState));
}

Hydro::Input::~Input()
{
	// Release the mouse.
	if (pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = nullptr;
	}

	// Release the keyboard.
	if (pKeyboard)
	{
		pKeyboard->Unacquire();
		pKeyboard->Release();
		pKeyboard = nullptr;
	}

	// Release the main interface to direct input.
	if (pDirectInput)
	{
		pDirectInput->Release();
		pDirectInput = nullptr;
	}
}

bool Hydro::Input::Update()
{
	bool result = true;

	//Store the keyboardstate from last frame
	memcpy(lasKeyboradState, keyboardState, sizeof(lasKeyboradState));

	//Read the current state of the keyboard
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	//Read the current state of the mouse
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//Process the changes in the mouse and keyboard
	ProcessInput();

	return true;
}

void Hydro::Input::GetMouseLocation(int& xPosition, int& yPosition)
{
	//Return the position of the mouse inside the window
	xPosition = mouseX;
	yPosition = mouseY;
	return;
}

bool Hydro::Input::IsKeyPressed(int key)
{
	//Check if key is outside of the array limits and return false value
	if (key > 256 || key < 0)
		return false;

	//Check if the key is currently depressed and was released in the former frame
	if ((keyboardState[key] & 0x80) && !(lasKeyboradState[key] & 0x80))
		return true;

	return false;
}

bool Hydro::Input::IsKeyReleased(int key)
{
	//Check if key is outside of the array limits and return false value
	if (key > 256 || key < 0)
		return false;

	//Is the key currently released and was pressed a frame before
	if (!(keyboardState[key] & 0x80) && (lasKeyboradState[key] & 0x80))
		return true;

	return false;
}

bool Hydro::Input::IsKeyDown(int key)
{
	//Check if key is outside of the array limits and return false value
	if (key < 0 && key > 256)
		return false;

	//Check if the key is currently depressed
	if (keyboardState[key] & 0x80)
		return true;

	return false;
}

bool Hydro::Input::ReadKeyboard()
{
	HRESULT result = true;

	//Read the keyboard device
	result = pKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result))
	{
		//If the keyboard lost focus or was not acquired then try to get control back.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool Hydro::Input::ReadMouse()
{
	HRESULT result = true;

	//Read the mouse device.
	result = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		//If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			pMouse->Acquire();
		else
			return false;
	}

	return true;
}

void Hydro::Input::ProcessInput()
{
	//Update the location of the mouse cursor based on the change of the mouse location during the frame
	mouseX = mouseX + mouseState.lX;
	mouseY = mouseY + mouseState.lY;

	//Ensure the mouse location does not exceed the screen width or height
	if (mouseX < 0) { mouseX = 0; }
	if (mouseY < 0) { mouseY = 0; }
	if (mouseX >= screenWidth) { mouseX = screenWidth - 1; }
	if (mouseY >= screenHeight) { mouseY = screenHeight - 1; }
}
