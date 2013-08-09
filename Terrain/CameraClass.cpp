#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_posX, m_posY, m_posZ);
}

XMFLOAT4 CameraClass::GetRotation()
{
	return XMFLOAT4(m_rotX, m_rotY, m_rotZ, 1.0f);
}

void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, roll, pitch;
	XMMATRIX rotationMatrix;

	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	position = XMVectorSet(m_posX, m_posY, m_posZ, 0.0f);

	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	pitch = m_rotX * 0.0174532925f;
	yaw = m_rotY * 0.0174532925f;
	roll = m_rotZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	lookAt = position + lookAt;

	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}