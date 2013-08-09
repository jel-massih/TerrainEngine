#include "quadtreeclass.h"

QuadTreeClass::QuadTreeClass()
{
	m_vertexList = 0;
	m_parentNode = 0;
}

QuadTreeClass::QuadTreeClass(const QuadTreeClass& other)
{
}

QuadTreeClass::~QuadTreeClass()
{
}

bool QuadTreeClass::Initialize(TerrainClass* terrain, ID3D11Device* device)
{
	int vertexCount;
	float centerX, centerY, centerZ;

	vertexCount = terrain->GetVertexCount();

	m_triangleCount = vertexCount/3;

	m_vertexList = new VertexType[vertexCount];
	if(!m_vertexList)
	{
		return false;
	}

	terrain->CopyVertexArray((void*)m_vertexList);

	CalculateMeshDimensions(vertexCount, centerX, centerY, centerZ);

	m_parentNode = new NodeType;
	if(!m_parentNode)
	{
		return false;
	}

	CreateTreeNode(m_parentNode, centerX, centerY, centerZ, device);

	if(m_vertexList)
	{
		delete[] m_vertexList;
		m_vertexList = 0;
	}

	return true;
}

void QuadTreeClass::Shutdown()
{
	if(m_parentNode)
	{
		ReleaseNode(m_parentNode);
		delete m_parentNode;
		m_parentNode = 0;
	}
	
	return;
}

void QuadTreeClass::Render(FrustrumClass* frustrum, ID3D11DeviceContext* deviceContext, TerrainShaderClass* terrainShader)
{
	m_drawCount = 0;

	RenderNode(m_parentNode, frustrum, deviceContext, terrainShader);

	return;
}

int QuadTreeClass::GetDrawCount()
{
	return m_drawCount;
}

int QuadTreeClass::GetTriangleCount()
{
	return m_triangleCount;
}

void QuadTreeClass::CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth)
{
	float maxWidth, maxDepth, minWidth, minDepth, width, depth, maxX, maxZ;

	centerX = 0.0f;
	centerZ = 0.0f;

	for(int i=0; i<vertexCount; i++)
	{
		centerX += m_vertexList[i].position.x;
		centerZ += m_vertexList[i].position.z;
	}

	centerX /= (float)vertexCount;
	centerZ /= (float)vertexCount;

	maxWidth = 0.0f;
	maxDepth = 0.0f;

	minWidth = fabsf(m_vertexList[0].position.x - centerX);
	minDepth = fabsf(m_vertexList[0].position.z - centerZ);

	for(int i=0; i<vertexCount; i++)
	{
		width = fabsf(m_vertexList[i].position.x - centerX);
		depth = fabsf(m_vertexList[i].position.z - centerZ);

		if(width > maxWidth) {maxWidth = width;}
		if(depth > maxDepth) {maxDepth = depth;}
		if(width < minWidth) { minWidth = width;}
		if(depth < minDepth) {minDepth = depth;}
	}

	maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	meshWidth = max(maxX, maxZ * 2.0f);
}

void QuadTreeClass::CreateTreeNode(NodeType* node, float posX, float posZ, float width, ID3D11Device* device)
{
	int numTriangles, count, vertexCount, index, vertexIndex;
	float offsetX, offsetZ;
	VertexType* vertices;
	unsigned long* indices;
	bool result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	node->posX = posX;
	node->posZ = posZ;
	node->width = width;

	node->triangleCount = 0;
	
	node->vertexBuffer = 0;
	node->indexBuffer = 0;

	node->nodes[0] = 0;
	node->nodes[1] = 0;
	node->nodes[2] = 0;
	node->nodes[3] = 0;

	numTriangles = CountTriangles(posX, posZ, width);

	if(numTriangles == 0)
	{
		return;
	}

	if(numTriangles > MAX_TRIANGLES)
	{
		for(int i=0; i<4; i++)
		{
			offsetX = (((i%2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			offsetZ = (((i%4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			count = CountTriangles(posX + offsetX, posZ, width);
			if(count > 0)
			{
				node->nodes[i] = new NodeType;

				CreateTreeNode(node->nodes[i], posX + offsetX, posZ + offsetZ, width / 2.0f, device);
			}
		}

		return;
	}

	node->triangleCount = numTriangles;

	vertexCount = numTriangles * 3;

	vertices = new VertexType[vertexCount];

	indices = new unsigned long[vertexCount];

	index = 0;

	for(int i=0; i<m_triangleCount; i++)
	{
		result = IsTriangleContained(i, posX, posZ, width);
		if(result == true)
		{
			vertexIndex = i*3;

			vertices[index].position = m_vertexList[vertexIndex].position;
			vertices[index].texture = m_vertexList[vertexIndex].texture;
			vertices[index].normal = m_vertexList[vertexIndex].normal;
			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index].position = m_vertexList[vertexIndex].position;
			vertices[index].texture = m_vertexList[vertexIndex].texture;
			vertices[index].normal = m_vertexList[vertexIndex].normal;
			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index].position = m_vertexList[vertexIndex].position;
			vertices[index].texture = m_vertexList[vertexIndex].texture;
			vertices[index].normal = m_vertexList[vertexIndex].normal;
			indices[index] = index;
			index++;
		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufferDesc, &vertexData, &node->vertexBuffer);

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufferDesc, &indexData, &node->indexBuffer);

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return;
}

int QuadTreeClass::CountTriangles(float posX, float posY, float width)
{
	int count;

	count = 0;

	for(int i=0; i<m_triangleCount; i++)
	{
		if(IsTriangleContained(i, posX, posY, width))
		{
			count++;
		}
	}

	return count;
}

bool QuadTreeClass::IsTriangleContained(int index, float posX, float posZ, float width)
{
	float radius;
	int vertexIndex;
	float x1, z1, x2, z2, x3, z3;
	float minimumX, maximumX, minimumZ, maximumZ;

	radius = width / 2.0f;

	vertexIndex = index * 3;

	x1 = m_vertexList[vertexIndex].position.x;
	z1 = m_vertexList[vertexIndex].position.z;
	vertexIndex++;

	x2 = m_vertexList[vertexIndex].position.x;
	z2 = m_vertexList[vertexIndex].position.z;
	vertexIndex++;

	x3 = m_vertexList[vertexIndex].position.x;
	z3 = m_vertexList[vertexIndex].position.z;
	vertexIndex++;

	minimumX = min(x1, min(x2, x3));
	if(minimumX > (posX + radius))
	{
		return false;
	}

	maximumX = max(x1, max(x2, x3));
	if(maximumX < posX - radius)
	{
		return false;
	}

	minimumZ = min(z1, min(z2, z3));
	if(minimumZ > posZ + radius)
	{
		return false;
	}

	maximumZ = max(z1, max(z2, z3));
	if(maximumZ < posZ - radius)
	{
		return false;
	}

	return true;
}

void QuadTreeClass::ReleaseNode(NodeType* node)
{
	for(int i=0; i<4; i++)
	{
		if(node->nodes[i] != 0)
		{
			ReleaseNode(node->nodes[i]);
		}
	}

	if(node->vertexBuffer)
	{
		node->vertexBuffer->Release();
		node->vertexBuffer = 0;
	}

	if(node->indexBuffer)
	{
		node->indexBuffer->Release();
		node->indexBuffer = 0;
	}

	for(int i=0; i<4; i++)
	{
		if(node->nodes[i])
		{
			delete node->nodes[i];
			node->nodes[i] = 0;
		}
	}

	return;
}

void QuadTreeClass::RenderNode(NodeType* node, FrustrumClass* frustrum, ID3D11DeviceContext* deviceContext, TerrainShaderClass* terrainShader)
{
	bool result;
	int count, indexCount;
	unsigned int stride, offset;

	result = frustrum->CheckCube(node->posX, 0.0f, node->posZ, node->width / (2.0f));
	if(!result)
	{
		return;
	}

	count = 0;
	for(int i=0; i<4; i++)
	{
		if(node->nodes[i] != 0)
		{
			count++;
			RenderNode(node->nodes[i], frustrum, deviceContext, terrainShader);
		}
	}

	if(count != 0)		
	{
		return;
	}

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &node->vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(node->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	indexCount = node->triangleCount * 3;

	terrainShader->RenderShaders(deviceContext, indexCount);

	m_drawCount += node->triangleCount;

	return;
}