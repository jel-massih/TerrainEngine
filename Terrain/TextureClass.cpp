#include "textureclass.h"

const WCHAR* defaultTextureName = L"default.dds";

TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	result = CreateDDSTextureFromFile(device, filename, nullptr, &m_texture);

	if(FAILED(result))
	{
		result = CreateDDSTextureFromFile(device, defaultTextureName, nullptr, &m_texture);
		if(FAILED(result))
		{
			return false;
		}
	}

	return true;
}

void TextureClass::ShutDown()
{
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}