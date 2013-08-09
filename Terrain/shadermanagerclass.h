#ifndef SHADERMANAGERCLASS_H
#define SHADERMANAGERCLASS_H

#include <d3d11.h>

#include "terrainshaderclass.h"
#include "colorshaderclass.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass& other);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();

	bool RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,
		XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, ID3D11ShaderResourceView* texture);

	bool RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);

	TerrainShaderClass* GetTerrainShader();

private:
	TerrainShaderClass* m_TerrainShader;
	ColorShaderClass* m_ColorShader;
};

#endif