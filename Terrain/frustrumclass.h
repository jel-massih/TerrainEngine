#ifndef FRUSTRUMCLASS_H
#define FRUSTRUMCLASS_H

#include <DirectXMath.h>

using namespace DirectX;

class FrustrumClass
{
public:
	FrustrumClass();
	FrustrumClass(const FrustrumClass& other);
	~FrustrumClass();

	void ConstructFrustrum(float screenDepth, XMMATRIX& projectionMatrix, XMMATRIX& viewMatrix);

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float x, float y, float z, float radius);
	bool CheckSphere(float x, float y, float z, float radius);
	bool CheckRectangle(float x, float y, float z, float xSize, float ySize, float zSize);

private:
	XMFLOAT4 m_planes[6];
};

#endif