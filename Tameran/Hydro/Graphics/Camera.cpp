#include "Camera.h"

Hydro::Camera::Camera()
	: position(0, 0, 0), rotation(0, 0, 0), viewMatrix(), baseViewMatrix()
{}

Hydro::Camera::~Camera()
{
}

void Hydro::Camera::SetPosition(const DirectX::XMFLOAT3 Position)
{
	position = Position;
}

void Hydro::Camera::SetRotation(const DirectX::XMFLOAT3 Rotation)
{
	rotation = Rotation;
}

DirectX::XMFLOAT3 Hydro::Camera::GetPosition() const
{
	return position;
}

DirectX::XMFLOAT3 Hydro::Camera::GetRotation() const
{
	return rotation;
}

void Hydro::Camera::Render(bool base)
{
	//Initial set up 
	DirectX::XMFLOAT3 up(0, 1, 0), lookAt(0, 0, 1);
	DirectX::XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;

	//Load it into a XMVECTOR structure
	upVector = XMLoadFloat3(&up);

	//Load the position into a Vector structure
	positionVector = XMLoadFloat3(&position);

	//Load it into a Vector
	lookAtVector = XMLoadFloat3(&lookAt);

	//Set the yaw(y Axis), pitch (x Axis) and roll (z Axis) in radians
	pitch = rotation.x;
	roll = rotation.z;
	yaw = rotation.y;

	//Create the rotation matrix from the three angles
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//Transform the upVector and the lookAtVector with the rotationMatrix
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	//Translate the rotated camera position to the location of the viewer
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	//Finally create the view matric from three updated vectors
	viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	//if you want to render the baseViewMatrix
	if (base)
	{
		baseViewMatrix = viewMatrix;
	}
}

void Hydro::Camera::GetViewMatrix(DirectX::XMMATRIX& view)
{
	view = viewMatrix;
}

void Hydro::Camera::GetBaseViewMatrix(DirectX::XMMATRIX& baseViewMatrix)
{
	baseViewMatrix = baseViewMatrix;
}
