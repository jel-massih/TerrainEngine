#include "camerapositionclass.h"

const float DEFAULT_CAM_LOOK_SPEED = 0.075f;
const float DEFAULT_CAM_MOVE_SPEED = 0.05f;
const float FAST_CAM_MOVE_SPEED = 0.005f;

CameraPositionClass::CameraPositionClass()
{
	m_frameTime = 0.0f;

	m_CamSpeed = DEFAULT_CAM_MOVE_SPEED;

	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;

	m_forwardSpeed = 0.0f;
	m_backwardSpeed = 0.0f;
	m_upwardSpeed = 0.0f;
	m_downwardSpeed = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed = 0.0f;
	m_lookDownSpeed = 0.0f;
}

CameraPositionClass::CameraPositionClass(const CameraPositionClass& other)
{
}

CameraPositionClass::~CameraPositionClass()
{
}

void CameraPositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void CameraPositionClass::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;

	return;
}

void CameraPositionClass::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;

	return;
}

void CameraPositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_posX;
	y = m_posY;
	z = m_posZ;

	return;
}

void CameraPositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotX;
	y = m_rotY;
	z = m_rotZ;

	return;
}

void CameraPositionClass::UpdateInput(InputClass* input)
{
	bool keyDown;

	if(input->IsKeyPressed(DIK_LSHIFT))
	{
		m_CamSpeed = FAST_CAM_MOVE_SPEED;
	}
	else
	{
		m_CamSpeed = DEFAULT_CAM_MOVE_SPEED;
	}

	keyDown = input->IsKeyPressed(DIK_D);
	MoveRight(keyDown);

	keyDown = input->IsKeyPressed(DIK_A);
	MoveLeft(keyDown);

	keyDown = input->IsKeyPressed(DIK_W);
	MoveForward(keyDown);

	keyDown = input->IsKeyPressed(DIK_S);
	MoveBackward(keyDown);

	keyDown = input->IsKeyPressed(DIK_LCONTROL);
	MoveUpward(keyDown);

	keyDown = input->IsKeyPressed(DIK_LALT);
	MoveDownward(keyDown);

	keyDown = input->IsKeyPressed(DIK_UPARROW);
	LookUpward(keyDown);

	keyDown = input->IsKeyPressed(DIK_DOWNARROW);
	LookDownward(keyDown);

	keyDown = input->IsKeyPressed(DIK_RIGHTARROW);
	LookRight(keyDown);

	keyDown = input->IsKeyPressed(DIK_LEFTARROW);
	LookLeft(keyDown);
}

void CameraPositionClass::MoveForward(bool keydown)
{
	float radians, radians2;

	if(keydown)
	{
		m_forwardSpeed += m_frameTime * 0.01f;

		if(m_forwardSpeed > m_frameTime * m_CamSpeed)
		{
			m_forwardSpeed = m_frameTime * m_CamSpeed;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.005f;

		if(m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	radians = m_rotY * 0.0174532925f;
	radians2 = m_rotX * 0.0174532925f;

	m_posX += cosf(radians2) * sinf(radians) * m_forwardSpeed;
	m_posZ += cosf(radians2) * cosf(radians) * m_forwardSpeed;


	m_posY -= sinf(radians2) * m_forwardSpeed;

	return;
}

void CameraPositionClass::MoveBackward(bool keydown)
{
	float radians, radians2;

	if(keydown)
	{
		m_backwardSpeed += m_frameTime * 0.01f;

		if(m_backwardSpeed > m_frameTime * m_CamSpeed)
		{
			m_backwardSpeed = m_frameTime * m_CamSpeed;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.005f;

		if(m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	radians = m_rotY * 0.0174532925f;
	radians2 = m_rotX * 0.0174532925f;

	m_posX -= cosf(radians2) * sinf(radians) * m_backwardSpeed;
	m_posZ -= cosf(radians2) * cosf(radians) * m_backwardSpeed;


	m_posY += sinf(radians2) * m_backwardSpeed;

	return;
}

void CameraPositionClass::MoveRight(bool keydown)
{
	float radians;

	if(keydown)
	{
		m_rightMoveSpeed += m_frameTime * 0.01f;

		if(m_rightMoveSpeed > m_frameTime * m_CamSpeed)
		{
			m_rightMoveSpeed = m_frameTime * m_CamSpeed;
		}
	}
	else
	{
		m_rightMoveSpeed -= m_frameTime * 0.005f;

		if(m_rightMoveSpeed < 0.0f)
		{
			m_rightMoveSpeed = 0.0f;
		}
	}

	radians = m_rotY * 0.0174532925f;

	m_posX += cosf(radians) * m_rightMoveSpeed;
	m_posZ -= sinf(radians) * m_rightMoveSpeed;

	return;
}

void CameraPositionClass::MoveLeft(bool keydown)
{
	float radians;

	if(keydown)
	{
		m_leftMoveSpeed += m_frameTime * 0.01f;

		if(m_leftMoveSpeed > m_frameTime * m_CamSpeed)
		{
			m_leftMoveSpeed = m_frameTime * m_CamSpeed;
		}
	}
	else
	{
		m_leftMoveSpeed -= m_frameTime * 0.005f;

		if(m_leftMoveSpeed < 0.0f)
		{
			m_leftMoveSpeed = 0.0f;
		}
	}

	radians = m_rotY * 0.0174532925f;

	m_posX -= cosf(radians) * m_leftMoveSpeed;
	m_posZ += sinf(radians) * m_leftMoveSpeed;

	return;
}

void CameraPositionClass::MoveUpward(bool keydown)
{
	if(keydown)
	{
		m_upwardSpeed += m_frameTime * 0.01f;

		if(m_upwardSpeed > m_frameTime * m_CamSpeed)
		{
			m_upwardSpeed = m_frameTime * m_CamSpeed;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.005f;

		if(m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	m_posY += m_upwardSpeed;

	return;
}

void CameraPositionClass::MoveDownward(bool keydown)
{
	if(keydown)
	{
		m_downwardSpeed += m_frameTime * 0.01f;

		if(m_downwardSpeed > m_frameTime * m_CamSpeed)
		{
			m_downwardSpeed = m_frameTime * m_CamSpeed;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.005f;

		if(m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	m_posY -= m_downwardSpeed;

	return;
}

void CameraPositionClass::LookLeft(bool keydown)
{
	if(keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if(m_leftTurnSpeed > m_frameTime * DEFAULT_CAM_LOOK_SPEED)
		{
			m_leftTurnSpeed = m_frameTime * DEFAULT_CAM_LOOK_SPEED;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.005f;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	m_rotY -= m_leftTurnSpeed;
	if(m_rotY < 0.0f)
	{
		m_rotY += 360.0f;
	}

	return;
}

void CameraPositionClass::LookRight(bool keydown)
{
	if(keydown)
	{
		m_rightTurnSpeed  += m_frameTime * 0.01f;

		if(m_rightTurnSpeed  > m_frameTime * DEFAULT_CAM_LOOK_SPEED)
		{
			m_rightTurnSpeed  = m_frameTime * DEFAULT_CAM_LOOK_SPEED;
		}
	}
	else
	{
		m_rightTurnSpeed  -= m_frameTime * 0.005f;

		if(m_rightTurnSpeed  < 0.0f)
		{
			m_rightTurnSpeed  = 0.0f;
		}
	}

	m_rotY += m_rightTurnSpeed ;
	if(m_rotY > 0.0f)
	{
		m_rotY -= 360.0f;
	}

	return;
}

void CameraPositionClass::LookUpward(bool keydown)
{
	if(keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if(m_lookUpSpeed > m_frameTime * DEFAULT_CAM_LOOK_SPEED)
		{
			m_lookUpSpeed = m_frameTime * DEFAULT_CAM_LOOK_SPEED;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime * 0.005f;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	m_rotX -= m_lookUpSpeed;
	if(m_rotX < -90.0f)
	{
		m_rotX = -90.0f;
	}

	return;
}

void CameraPositionClass::LookDownward(bool keydown)
{
	if(keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if(m_lookDownSpeed > m_frameTime * DEFAULT_CAM_LOOK_SPEED)
		{
			m_lookDownSpeed = m_frameTime * DEFAULT_CAM_LOOK_SPEED;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime * 0.005f;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	m_rotX += m_lookDownSpeed;
	if(m_rotX > 90.0)
	{
		m_rotX = 90.0f;
	}

	return;
}