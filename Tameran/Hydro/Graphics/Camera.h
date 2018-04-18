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

		void SetPosition(const DirectX::XMFLOAT3 Position);
		void SetRotation(const DirectX::XMFLOAT3 Rotation);

		DirectX::XMFLOAT3 GetPosition() const;
		DirectX::XMFLOAT3 GetRotation() const;

		void Render(bool base);
		void GetViewMatrix(DirectX::XMMATRIX &view);
		void GetBaseViewMatrix(DirectX::XMMATRIX &baseViewMatrix);

	private:
		DirectX::XMFLOAT3 position, rotation;
		DirectX::XMMATRIX viewMatrix, baseViewMatrix;
	};
}
#endif // !CAMERA

