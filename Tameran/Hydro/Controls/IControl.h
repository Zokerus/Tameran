#ifndef ICONTROL
#define ICONTROL

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
		enum ControlType { Lablel, LinkLabel, PictureBox, Button};

	public:
		IControl();
		~IControl();

		bool Initialize();
		void Shutdown();

		virtual bool Update(float eTime);
		virtual bool Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);
		virtual void HandleInput(Input* input);

		void OnSelect();
		void SetFocus(bool focus);

		char* GetName() const;
		char* GetString() const;
		DirectX::XMFLOAT2 GetSize() const;
		DirectX::XMINT2 GetPosition() const;
		bool HasFocus() const;
		bool IsEnable() const;
		bool IsVisible() const;
		bool GetTabStop() const;
		DirectX::XMVECTORF32 GetColor() const;
		ControlType GetType() const;

	protected:
		char* m_name;
		char* m_text;
		DirectX::XMFLOAT2 m_size;
		DirectX::XMINT2 m_pos;
		bool m_focus, m_enable, m_visible, m_tabstop, m_ready;
		DirectX::XMVECTORF32 m_color;
		ControlType m_type;
	};
}

#endif // !ICONTROL

