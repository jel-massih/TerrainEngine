#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include <d3d11.h>
#include "DDSTextureLoader.h"

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Initialize(ID3D11Device* device, WCHAR* filename);
	void ShutDown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif