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
		Label(Direct3D* direct3D, Font* font, int ScreenWidth, int ScreenHeight, int MaxLength, std::string Name, std::string text, DirectX::XMINT2 Pos, DirectX::XMFLOAT2 Size, DirectX::XMVECTORF32 Color);
		~Label();

		bool Update(float eTime);
		bool Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);

		void HandleInput();

		void SetPosition(DirectX::XMINT2 Pos);

		const std::string* GetString() const;

	protected:
		Text string;
	};
}

#endif // !LABEL

