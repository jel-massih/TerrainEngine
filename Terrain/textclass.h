#ifndef TEXTCLASS_H
#define TEXTCLASS_H

#include "fontclass.h"
#include "fontshaderclass.h"

class TextClass
{
public:
	TextClass();
	TextClass(const TextClass& other);
	~TextClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX& baseViewMatrix);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& orthoMatrix);
	
	bool AddString(const char* stringID, ID3D11Device* device);
	bool AddString(char* stringID, char* text, int posX, int posY, ID3D11Device* device);
	bool AddString(char* stringID, char* text, int posX, int posY, float r, float g, float b, ID3D11Device* device);

	bool UpdateString(const char* stringID, char* text, ID3D11DeviceContext* deviceContext);

private:
	struct SentenceType
	{
		const char* stringID;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

private:
	bool InitializeSentence(SentenceType** sentence, const char* stringID, int maxLength, ID3D11Device* device);
	bool UpdateSentence(SentenceType* sentence, char* text, int posX, int posY, float r, float g, float b, ID3D11DeviceContext* deviceContext);
	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX& worldMatrix, XMMATRIX& orthoMatrix);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	XMMATRIX m_baseViewMatrix;
	SentenceType** m_strings;
	int m_stringCount;
};

#endif