#ifndef ICONTROL
#define ICONTROL

//Includes
#include <string>
#include <exception>

//My Includes
#include "../Graphics/Shader/ShaderManager.h"
#include "../Input/Input.h"

//////////////////////////////////////////////////////////////////////////
//Class: Control Interface
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class IControl
	{
	public:
		enum ControlType { Lablel, LinkLabel, PictureBox, Button, ProgressBar};

	public:
		IControl(std::string Name, DirectX::XMFLOAT2 Size, DirectX::XMINT2 Pos, bool Focus, bool Enable, bool Visible, bool Tabstop, DirectX::XMVECTORF32 Color, ControlType Type);
		~IControl();

		virtual bool Update(float eTime);
		virtual bool Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);
		virtual void HandleInput(Input* input);

		void OnSelect();
		void SetFocus(bool Focus);
		void SetStatus(bool Enable);
		void Visibility(bool Visibility);
		void SetTabStop(bool TabStop);
		void SetPosition(DirectX::XMINT2 Pos);
		void SetSize(DirectX::XMFLOAT2 Size);

		std::string GetName() const;
		DirectX::XMFLOAT2 GetSize() const;
		DirectX::XMINT2 GetPosition() const;
		bool HasFocus() const;
		bool IsEnable() const;
		bool IsVisible() const;
		bool GetTabStop() const;
		DirectX::XMVECTORF32 GetColor() const;
		ControlType GetType() const;

	protected:
		std::string name;
		DirectX::XMFLOAT2 size;
		DirectX::XMINT2 pos;
		bool focus, enable, visible, tabstop;
		DirectX::XMVECTORF32 color;
		ControlType type;
	};
}

#endif // !ICONTROL

