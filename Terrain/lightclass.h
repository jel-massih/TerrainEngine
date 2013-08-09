#ifndef LIGHTCLASS_H
#define LIGHTCLASS_H

#include <DirectXMath.h>

using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	void SetAmbientColor(float r, float g, float b, float alpha);
	void SetDiffuseColor(float r, float g, float b, float alpha);
	void SetDirection(float x, float y, float z);
	void SetSpecularColor(float r, float g, float b, float alpha);
	void SetSpecularPower(float power);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_specularColor;
	float m_specularPower;
};

#endif