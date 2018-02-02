#ifndef LINKLABEL
#define LINKLABEL

//Includes
#include <functional>

//My Includes
#include "Label.h"

//////////////////////////////////////////////////////////////////////////
//Class: LinkLabel
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class LinkLabel : public Label
	{
	public:
		LinkLabel(DirectX::XMVECTORF32 selectedColor, std::function<void(IControl*)> func, int screenWidth, int screenHeight, int maxLength);
		~LinkLabel();

		//bool Initialize(Direct3D *direct3D, Font *font, char* string, DirectX::XMINT2 pos, DirectX::XMFLOAT2 size, DirectX::XMVECTORF32 color);
		//void Shutdown();

		bool Update(float eTime);
		bool Draw(float eTime, ID3D11DeviceContext *deviceContext, ShaderManager *shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);
	
		void HandleInput(Input *input);

	private:
		DirectX::XMVECTORF32 m_selectedColor;
		int m_counter;
		std::function<void(IControl*)> m_select;
	};
}
#endif // !LINKLABEL

