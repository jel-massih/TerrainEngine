#include "shadermanagerclass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_TerrainShader = 0;
	m_ColorShader = 0;
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other)
{
}

ShaderManagerClass::~ShaderManagerClass()
{
}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	if(!m_TerrainShader->Initialize(device, hwnd))
	{
		return false;
	}

	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
	{
		return false;
	}

	if(!m_ColorShader->Initialize(device, hwnd))
	{
		return false;
	}

	return true;
}

void ShaderManagerClass::Shutdown()
{
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}
}

bool ShaderManagerClass::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,
		XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, ID3D11ShaderResourceView* texture)
{
	bool result;

	result = m_TerrainShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, lightDirection, ambientColor, diffuseColor, texture);
	if(!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	bool result;

	result = m_ColorShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}

	return true;
}

TerrainShaderClass* ShaderManagerClass::GetTerrainShader()
{
	return m_TerrainShader;
}