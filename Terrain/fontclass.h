#ifndef FONTCLASS_H
#define FONTCLASS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
using namespace std;
using namespace DirectX;

#include "textureclass.h"

class FontClass
{
public:
	FontClass();
	FontClass(const FontClass& other);
	~FontClass();

	bool Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename);
	void ShutDown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

private:
	bool LoadFontData(char* filename);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};

#endif