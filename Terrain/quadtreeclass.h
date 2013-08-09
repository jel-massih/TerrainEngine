#ifndef QUADTREECLASS_H
#define QUADTREECLASS_H

#include "terrainclass.h"
#include "frustrumclass.h"
#include "terrainshaderclass.h"

const int MAX_TRIANGLES = 10000;

class QuadTreeClass
{
public:
	QuadTreeClass();
	QuadTreeClass(const QuadTreeClass& other);
	~QuadTreeClass();

	bool Initialize(TerrainClass* terrain, ID3D11Device* device);
	void Shutdown();
	void Render(FrustrumClass* frustrum, ID3D11DeviceContext* deviceContext, TerrainShaderClass* terrainShader);

	int GetDrawCount();
	int GetTriangleCount();
	
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct NodeType
	{
		float posX, posZ, width;
		int triangleCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		NodeType* nodes[4];
	};

private:
	void CalculateMeshDimensions(int, float&, float&, float&);
	void CreateTreeNode(NodeType* node, float posX, float posY, float width, ID3D11Device* device);
	int CountTriangles(float, float, float);
	bool IsTriangleContained(int, float, float, float);
	void ReleaseNode(NodeType* node);
	void RenderNode(NodeType* node, FrustrumClass* frustrum, ID3D11DeviceContext* deviceContext, TerrainShaderClass* terrainShader);

private:
	int m_triangleCount, m_drawCount;
	VertexType* m_vertexList;
	NodeType* m_parentNode;
};

#endif