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
		Input(unsigned int ScreenWidth, unsigned int ScreenHeight, HINSTANCE hinstance, HWND hwnd);
		~Input();

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
		IDirectInput8* pDirectInput;
		IDirectInputDevice8* pKeyboard;
		IDirectInputDevice8* pMouse;

		unsigned char keyboardState[256];
		unsigned char lasKeyboradState[256];
		bool releasedKeyboardState[256];
		DIMOUSESTATE mouseState;


		int screenWidth, screenHeight;
		int mouseX, mouseY;
	};
}
#endif //	!INPUTHEADER
