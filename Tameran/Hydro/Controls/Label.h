#ifndef LABEL
#define LABEL

//My Includes
#include "../Graphics/Direct3D.h"
#include "IControl.h"
#include "../Graphics/Text/Font.h"
#include "../Graphics/Text/Text.h"

//////////////////////////////////////////////////////////////////////////
//Class: Label
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Label : public IControl
	{
	public:
		Label(int screenWidth, int screenHeight, int maxLength);
		~Label();
		//TODO get the text storage and type straight
		bool Initialize(Direct3D *direct3D, Font *font, char* string, DirectX::XMINT2 pos, DirectX::XMFLOAT2 size, DirectX::XMVECTORF32 color);
		void Shutdown();

		bool Update(float eTime);
		bool Draw(float eTime, ID3D11DeviceContext *deviceContext, ShaderManager *shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);

		void HandleInput();

	protected:
		Text m_string;
		Font *m_font;
		bool m_ready;
	};
}

#endif // !LABEL

