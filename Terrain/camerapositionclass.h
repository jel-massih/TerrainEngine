#ifndef CAMERAPOSITIONCLASS_H
#define CAMERAPOSITIONCLASS_H

#include <math.h>
#include "InputClass.h"

class CameraPositionClass
{
public:
	CameraPositionClass();
	CameraPositionClass(const CameraPositionClass&);
	~CameraPositionClass();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void GetPosition(float& x, float& y, float& z);
	void GetRotation(float& x, float& y, float& z);

	void SetFrameTime(float);
	void UpdateInput(InputClass* input);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveLeft(bool);
	void MoveRight(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void LookLeft(bool);
	void LookRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

private:
	float m_frameTime;

	float m_CamSpeed;

	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;

	float m_forwardSpeed, m_backwardSpeed;
	float m_leftMoveSpeed, m_rightMoveSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;
};

#endif