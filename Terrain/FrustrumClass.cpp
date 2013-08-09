#include "frustrumclass.h"

FrustrumClass::FrustrumClass()
{
}

FrustrumClass::FrustrumClass(const FrustrumClass& other)
{
}

FrustrumClass::~FrustrumClass()
{
}

void FrustrumClass::ConstructFrustrum(float screenDepth, XMMATRIX& projectionMatrix, XMMATRIX& viewMatrix)
{
	float zMinimum, r;
	XMFLOAT4X4A matrix;
	XMFLOAT4X4A projFloat;

	XMStoreFloat4x4A(&projFloat, projectionMatrix);

	zMinimum = -projFloat._43 / projFloat._33;
	r = screenDepth / (screenDepth - zMinimum);
	projFloat._33 = r;
	projFloat._43 = -r*zMinimum;

	projectionMatrix = XMLoadFloat4x4A(&projFloat);

	XMStoreFloat4x4A(&matrix, XMMatrixMultiply(viewMatrix, projectionMatrix));

	m_planes[0].x = matrix._14 + matrix._13;
	m_planes[0].y = matrix._24 + matrix._23;
	m_planes[0].z = matrix._34 + matrix._33;
	m_planes[0].w = matrix._44 + matrix._43;
	XMStoreFloat4(&m_planes[0], XMPlaneNormalize(XMLoadFloat4(&m_planes[0])));

	m_planes[1].x = matrix._14 - matrix._13;
	m_planes[1].y = matrix._24 - matrix._23;
	m_planes[1].z = matrix._34 - matrix._33;
	m_planes[1].w = matrix._44 - matrix._43;
	XMStoreFloat4(&m_planes[1], XMPlaneNormalize(XMLoadFloat4(&m_planes[1])));

	m_planes[2].x = matrix._14 + matrix._11;
	m_planes[2].y = matrix._24 + matrix._21;
	m_planes[2].z = matrix._34 + matrix._31;
	m_planes[2].w = matrix._44 + matrix._41;
	XMStoreFloat4(&m_planes[2], XMPlaneNormalize(XMLoadFloat4(&m_planes[2])));

	m_planes[3].x = matrix._14 - matrix._11;
	m_planes[3].y = matrix._24 - matrix._21;
	m_planes[3].z = matrix._34 - matrix._31;
	m_planes[3].w = matrix._44 - matrix._41;
	XMStoreFloat4(&m_planes[3], XMPlaneNormalize(XMLoadFloat4(&m_planes[3])));

	m_planes[4].x = matrix._14 - matrix._12;
	m_planes[4].y = matrix._24 - matrix._22;
	m_planes[4].z = matrix._34 - matrix._32;
	m_planes[4].w = matrix._44 - matrix._42;
	XMStoreFloat4(&m_planes[4], XMPlaneNormalize(XMLoadFloat4(&m_planes[4])));

	m_planes[5].x = matrix._14 + matrix._12;
	m_planes[5].y = matrix._24 + matrix._22;
	m_planes[5].z = matrix._34 + matrix._32;
	m_planes[5].w = matrix._44 + matrix._42;
	XMStoreFloat4(&m_planes[5], XMPlaneNormalize(XMLoadFloat4(&m_planes[5])));

	return;
}

bool FrustrumClass::CheckPoint(float x, float y, float z)
{
	XMFLOAT4 tempFloat;
	for(int i=0; i<6; i++)
	{
		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x, y, z))));

		if( tempFloat.x < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool FrustrumClass::CheckCube(float x, float y, float z, float radius)
{
	XMFLOAT4 tempFloat;

	for(int i=0; i<6; i++)
	{
		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - radius, y - radius, z - radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + radius, y - radius, z - radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - radius, y + radius, z - radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + radius, y + radius, z - radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - radius, y - radius, z + radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + radius, y - radius, z + radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - radius, y + radius, z + radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + radius, y + radius, z + radius))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool FrustrumClass::CheckSphere(float x, float y, float z, float radius)
{
	XMFLOAT4 tempFloat;

	for(int i=0; i<6; i++)
	{
		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x, y, z))));

		if( tempFloat.x < -radius)
		{
			return false;
		}
	}

	return true;
}

bool FrustrumClass::CheckRectangle(float x, float y, float z, float xSize, float ySize, float zSize)
{
	XMFLOAT4 tempFloat;

	for(int i=0; i<6; i++)
	{
		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - xSize, y - ySize, z - zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + xSize, y - ySize, z - zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - xSize, y + ySize, z - zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + xSize, y + ySize, z - zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - xSize, y - ySize, z + zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + xSize, y - ySize, z + zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x - xSize, y + ySize, z + zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		XMStoreFloat4(&tempFloat, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat3(&XMFLOAT3(x + xSize, y + ySize, z + zSize))));
		if( tempFloat.x >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}