#ifndef CAMERA
#define CAMERA

//Includes
#include <DirectXMath.h>

//////////////////////////////////////////////////////////////////////////
//Class: Camera
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void SetPosition(const DirectX::XMFLOAT3 position);
		void SetRotation(const DirectX::XMFLOAT3 rotation);

		DirectX::XMFLOAT3 GetPosition() const;
		DirectX::XMFLOAT3 GetRotation() const;

		void Render(bool base);
		void GetViewMatrix(DirectX::XMMATRIX &view);
		void GetBaseViewMatrix(DirectX::XMMATRIX &baseViewMatrix);

	private:
		DirectX::XMFLOAT3 m_position, m_rotation;
		DirectX::XMMATRIX m_viewMatrix, m_baseViewMatrix;
	};
}
#endif // !CAMERA

