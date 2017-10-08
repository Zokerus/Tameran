#include "Input.h"

Input::Input(unsigned int screenWidth, unsigned int screenHeight)
	: m_directInput(nullptr), m_keyboard(nullptr), m_mouse(nullptr), m_mouseState(), m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_mouseX(0), m_mouseY(0), m_ready(false)
{}

Input::~Input()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Input::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result = false;
	int i = 0;

	//Initialize the main direct input device
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the direct input interface for the keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Set the data format, in this case is it a keyboard, we can use the predefined data format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//Set the cooperative level of the keyboard to not share with other programs
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Now acquire the keyboard
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the direct input device for the mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	//Set the cooperative level of the mouse to share with other programs
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Acquire the mouse
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the released key state
	for (i = 0; i < 256; i++)
		m_releasedKeyboardState[i] = true;

	//Read the current state of the keyboard
	result = ReadKeyboard();
	if (!result)
		return false;

	//Store the keyboardstate from last frame
	memcpy(m_lasKeyboradState, m_keyboardState, sizeof(m_lasKeyboradState));
	
	//Flag the class as ready
	m_ready = true;
	return true;
}

void Input::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = nullptr;
	}

	//Reset the ready flag of the class
	m_ready = false;
}

bool Input::Update()
{
	bool result;

	//Is the class ready to work?
	if (!m_ready)
	{
		return false;
	}

	//Store the keyboardstate from last frame
	memcpy(m_lasKeyboradState, m_keyboardState, sizeof(m_lasKeyboradState));

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

void Input::GetMouseLocation(int& xPosition, int& yPosition)
{
	//Is the class ready to work?
	if (!m_ready)
	{
		return;
	}

	//Return the position of the mouse inside the window
	xPosition = m_mouseX;
	yPosition = m_mouseY;
	return;
}

bool Input::IsKeyPressed(int key)
{
	//Is the class ready to work?
	if (!m_ready)
	{
		return false;
	}

	//Check if key is outside of the array limits and return false value
	if (key > 256 || key < 0)
		return false;

	//Check if the key is currently depressed and was released in the former frame
	if ((m_keyboardState[key] & 0x80) && !(m_lasKeyboradState[key] & 0x80))
		return true;

	return false;
}

bool Input::IsKeyReleased(int key)
{
	//Is the class ready to work?
	if (!m_ready)
	{
		return false;
	}

	//Check if key is outside of the array limits and return false value
	if (key > 256 || key < 0)
		return false;

	//Is the key currently released and was pressed a frame before
	if (!(m_keyboardState[key] & 0x80) && (m_lasKeyboradState[key] & 0x80))
		return true;

	return false;
}

bool Input::IsKeyDown(int key)
{
	//Is the class ready to work?
	if (!m_ready)
	{
		return false;
	}

	//Check if key is outside of the array limits and return false value
	if (key < 0 && key > 256)
		return false;

	//Check if the key is currently depressed
	if (m_keyboardState[key] & 0x80)
		return true;

	return false;
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	//Read the keyboard device
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		//If the keyboard lost focus or was not acquired then try to get control back.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	//Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		//If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_mouse->Acquire();
		else
			return false;
	}

	return true;
}

void Input::ProcessInput()
{
	//Update the location of the mouse cursor based on the change of the mouse location during the frame
	m_mouseX = m_mouseX + m_mouseState.lX;
	m_mouseY = m_mouseY + m_mouseState.lY;

	//Ensure the mouse location does not exceed the screen width or height
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }
	if (m_mouseX >= m_screenWidth) { m_mouseX = m_screenWidth - 1; }
	if (m_mouseY >= m_screenHeight) { m_mouseY = m_screenHeight - 1; }
}
