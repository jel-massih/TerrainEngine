#ifndef TERRAINCLASS_H
#define TERRAINCLASS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
using namespace DirectX;

#include "textureclass.h"

const int TEXTURE_REPEAT = 8;

class TerrainClass
{
public:
	TerrainClass();
	TerrainClass(const TerrainClass& other);
	~TerrainClass();

	bool Initialize(ID3D11Device* device, char* heightMapFilename, WCHAR* textureFilename);
	void Shutdown();

	int GetVertexCount();

	void CopyVertexArray(void*);

	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct HeightMapType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

private:
	bool LoadHeightMap(char* filename);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	void CalculateTextureCoordinates();
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();

private:
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount;
	VertexType* m_vertices;
	HeightMapType* m_heightMap;
	TextureClass* m_texture;
};

#endif