#ifndef INPUTHEADER
#define INPUTHEADER

//Pre-Processing Directives
#define DIRECTINPUT_VERSION 0x0800

//Linking
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//Includes
#include <dinput.h>
#include <algorithm>

//Class name: Input Class
namespace Hydro
{
	class Input
	{
	public:
		Input(unsigned int screenWidth, unsigned int screenHeight);
		~Input();

		bool Initialize(HINSTANCE hinstance, HWND hwnd);
		void Shutdown();
		bool Update();

		void GetMouseLocation(int &xPosition, int &yPosition);

		bool IsKeyPressed(int key);
		bool IsKeyReleased(int key);
		bool IsKeyDown(int key);

	private:
		bool ReadKeyboard();
		bool ReadMouse();
		void ProcessInput();

	private:
		IDirectInput8 *m_directInput;
		IDirectInputDevice8 *m_keyboard;
		IDirectInputDevice8 *m_mouse;

		unsigned char m_keyboardState[256];
		unsigned char m_lasKeyboradState[256];
		bool m_releasedKeyboardState[256];
		DIMOUSESTATE m_mouseState;


		int m_screenWidth, m_screenHeight;
		int m_mouseX, m_mouseY;

		bool m_ready;
	};
}
#endif //	!INPUTHEADER
