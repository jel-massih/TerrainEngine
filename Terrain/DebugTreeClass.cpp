#include "debugtreeclass.h"

DebugTreeClass::DebugTreeClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

DebugTreeClass::DebugTreeClass(const DebugTreeClass& other)
{
}

DebugTreeClass::~DebugTreeClass()
{
}

bool DebugTreeClass::Initialize(ID3D11Device* device, TerrainClass* terrain)
{
	if(!InitializeBuffers(device, terrain))
	{
		return false;
	}

	return true;
}

void DebugTreeClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

bool DebugTreeClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return true;
}

bool DebugTreeClass::InitializeBuffers(ID3D11Device* device, TerrainClass* terrain)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[4];
	if(!vertices)
	{
		return false;
	}

	indices = new unsigned long[4];
	if(!indices)
	{
		return false;
	}

	LoadVertexPositions(terrain, vertices);

	for(int i=0; i<4; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 8;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

bool DebugTreeClass::LoadVertexPositions(TerrainClass* terrain, VertexType* vertices)
{
	float size = 100.0f; 
	vertices[0].position = XMFLOAT3(-size, 0, size);
	vertices[1].position = XMFLOAT3(size, 0, size);
	vertices[2].position = XMFLOAT3(-size, 0, -size);
	vertices[3].position = XMFLOAT3(size, 0, -size);

	//vertices[4].position = XMFLOAT3(size, size, -size);
	//vertices[5].position = XMFLOAT3(size, size, size);
	//vertices[6].position = XMFLOAT3(size, -size, -size);
	//vertices[7].position = XMFLOAT3(size, -size, size);

	//vertices[8].position = XMFLOAT3(-size, size, size);
	//vertices[9].position = XMFLOAT3(size, size, size);
	//vertices[10].position = XMFLOAT3(-size, size, -size);
	//vertices[11].position = XMFLOAT3(size, size, -size);

	//vertices[12].position = XMFLOAT3(size, size, size);
	//vertices[13].position = XMFLOAT3(-size, size, size);
	//vertices[14].position = XMFLOAT3(size, -size, size);
	//vertices[15].position = XMFLOAT3(-size, -size, size);

	//vertices[16].position = XMFLOAT3(-size, size, size);
	//vertices[17].position = XMFLOAT3(-size, size, -size);
	//vertices[18].position = XMFLOAT3(-size, -size, size);
	//vertices[19].position = XMFLOAT3(-size, -size, -size);

	//vertices[20].position = XMFLOAT3(-size, -size, -size);
	//vertices[21].position = XMFLOAT3(size, -size, -size);
	//vertices[22].position = XMFLOAT3(-size, -size, size);
	//vertices[23].position = XMFLOAT3(size, -size, size);

	for(int i=0; i<4; i++)
	{
		vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	return true;
}

void DebugTreeClass::ShutdownBuffers()
{
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	return;
}

void DebugTreeClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return;
}