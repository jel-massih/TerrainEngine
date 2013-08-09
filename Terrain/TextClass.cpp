#include "textclass.h"

TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;

	m_strings = 0;
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX& baseViewMatrix)
{
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_baseViewMatrix = baseViewMatrix;

	m_stringCount = 0;

	m_Font = new FontClass;
	if(!m_Font)
	{
		return false;
	}

	result = m_Font->Initialize(device, "fontdata.txt", L"font.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not Initialize the font object.", L"Error", MB_OK);
		return false;
	}

	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	result = m_FontShader->Initialize(device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Initialize the Font Shader Object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	if(m_strings)
	{
		for(int i=0; i<m_stringCount; i++)
		{
			ReleaseSentence(&m_strings[i]);
		}

		delete m_strings;
		m_strings = 0;
	}

	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	if(m_Font)
	{
		m_Font->ShutDown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& orthoMatrix)
{
	bool result;

	for(int i=0; i<m_stringCount; i++)
	{
		result = RenderSentence(deviceContext, m_strings[i], worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}
	}
}

bool TextClass::InitializeSentence(SentenceType** sentence, const char* stringID, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}

	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	(*sentence)->maxLength = maxLength;

	(*sentence)->vertexCount = 6*maxLength;

	(*sentence)->indexCount = (*sentence)->vertexCount;

	(*sentence)->stringID = stringID;

	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));
	
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int posX, int posY, float r, float g, float b, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	sentence->red = r;
	sentence->green = g;
	sentence->blue = b;

	numLetters = (int)strlen(text);

	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	memset(vertices, 0, sizeof(VertexType) * sentence->vertexCount);

	drawX = (float)(((m_screenWidth / 2) * -1) + posX);
	drawY = (float)((m_screenHeight / 2) - posY);

	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, sizeof(VertexType) * sentence->vertexCount);

	deviceContext->Unmap(sentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX& worldMatrix, XMMATRIX& orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TextClass::AddString(const char* stringID, ID3D11Device* device)
{
	bool result;
	SentenceType** m_tempStrings;

	m_tempStrings = m_strings;

	m_strings = new SentenceType*[m_stringCount+1];
	if(!m_strings)
	{
		return false;
	}

	for(int i=0; i<m_stringCount; i++)
	{
		m_strings[i] = m_tempStrings[i];
	}


	result = InitializeSentence(&m_strings[m_stringCount], stringID, 32, device);
	if(!result)
	{
		return false;
	}

	m_stringCount++;

	delete m_tempStrings;
	m_tempStrings = 0;

	return true;
}

bool TextClass::AddString(char* stringID, char* text, int posX, int posY, ID3D11Device* device)
{
	return false;
}

bool TextClass::AddString(char* stringID, char* text, int posX, int posY, float r, float g, float b, ID3D11Device* device)
{
	return false;
}

bool TextClass::UpdateString(const char* stringID, char* text, ID3D11DeviceContext* deviceContext)
{
	bool result;
	int index;

	for(index = 0; index<m_stringCount; index++)
	{
		if(m_strings[index]->stringID == stringID)
		{
			result = UpdateSentence(m_strings[index], text, 20, 10 + index*20, 1.0f, 1.0f, 1.0f, deviceContext);
			if(!result)
			{
				return false;
			}
		}
	}

	return true;
}