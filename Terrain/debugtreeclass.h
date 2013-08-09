#ifndef DEBUGTREECLASS_H
#define DEBUGTREECLASS_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "terrainclass.h"

class DebugTreeClass
{
public:
	DebugTreeClass();
	DebugTreeClass(const DebugTreeClass& other);
	~DebugTreeClass();

	bool Initialize(ID3D11Device* device, TerrainClass* terrain);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext);

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

private:
	bool InitializeBuffers(ID3D11Device* device, TerrainClass*terrain);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadVertexPositions(TerrainClass* terrain, VertexType* vertices);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif