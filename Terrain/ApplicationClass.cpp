#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_D3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_Light = 0;
	m_Frustrum = 0;
	m_QuadTree = 0;
	m_DebugTree = 0;
	m_ShaderManager = 0;

	m_bF3Down = false;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float camX, camY, camZ;
	XMMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	result = m_Input->Initialize(hInstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}
	
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	camX = 50.0f;
	camY = 2.0f;
	camZ = -7.0f;

	m_Camera->SetPosition(camX, camY, camZ);

	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	result = m_Terrain->Initialize(m_D3D->GetDevice(), "height.bmp", L"dirt01.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}
	
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}
	
	m_Position = new CameraPositionClass;
	if(!m_Position)
	{
		return false;
	}

	m_Position->SetPosition(camX, camY, camZ);
	
	m_Fps = new FPSClass;
	if(!m_Fps)
	{
		return false;
	}

	m_Fps->Initialize();

	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	m_Cpu->Initialize();

	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	result = m_FontShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	m_Text->AddString("POS", m_D3D->GetDevice());
	m_Text->AddString("ROT", m_D3D->GetDevice());

	m_Text->AddString("FPS", m_D3D->GetDevice());
	m_Text->AddString("CPU", m_D3D->GetDevice());

	m_Text->AddString("QTR", m_D3D->GetDevice());
	m_Text->AddString("XCS", m_D3D->GetDevice());

	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	result = m_TerrainShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the Terrain Shader", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	m_Light->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 0.5f);

	m_Frustrum = new FrustrumClass;
	if(!m_Frustrum)
	{
		return false;
	}

	m_QuadTree = new QuadTreeClass;
	if(!m_QuadTree)
	{
		return false;
	}

	result = m_QuadTree->Initialize(m_Terrain, m_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could Not INitialize QuadTree", L"Error", MB_OK);
		return false;
	}

	m_DebugTree = new DebugTreeClass;
	if(!m_DebugTree)
	{
		return false;
	}

	result = m_DebugTree->Initialize(m_D3D->GetDevice(), m_Terrain);
	if(!result)
	{
		MessageBox(hwnd, L"Could Not Initialize DebugTree", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{
	if(m_DebugTree)
	{
		m_DebugTree->Shutdown();
		delete m_DebugTree;
		m_DebugTree = 0;
	}

	if(m_QuadTree)
	{
		m_QuadTree->Shutdown();
		delete m_QuadTree;
		m_QuadTree = 0;
	}

	if(m_Frustrum)
	{
		delete m_Frustrum;
		m_Frustrum = 0;
	}

	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}

bool ApplicationClass::Frame()
{
	char text[32], itoaTemp[16], videoCard[128];

	if(!m_Input->Frame())
	{
		return false;
	}

	if(m_Input->IsKeyPressed(DIK_ESCAPE))
	{
		return false;
	}

	m_Timer->Frame();
	m_Cpu->Frame();
	m_Fps->Frame();

	_itoa_s(m_Fps->GetFps(), itoaTemp, 10);
	strcpy_s(text, "FPSTime: ");
	strcat_s(text, itoaTemp);

	if(!m_Text->UpdateString("FPS", text, m_D3D->GetDeviceContext()))
	{
	}

	_itoa_s(m_Cpu->GetCpuPercentage(), itoaTemp, 10);
	strcpy_s(text, "CPUTime: ");
	strcat_s(text, itoaTemp);
	strcat_s(text, "%");

	if(!m_Text->UpdateString("CPU", text, m_D3D->GetDeviceContext()))
	{
	}

	float x, y, z;
	m_Position->GetPosition(x, y, z);

	strcpy_s(text, "POS (X: ");
	_itoa_s((int)x, itoaTemp, 10);
	strcat_s(text, itoaTemp);

	strcat_s(text, " Y: ");
	_itoa_s((int)y, itoaTemp, 10);
	strcat_s(text, itoaTemp);

	strcat_s(text, " Z: ");
	_itoa_s((int)z, itoaTemp, 10);
	strcat_s(text, itoaTemp);

	strcat_s(text, ")");

	if(!m_Text->UpdateString("POS", text, m_D3D->GetDeviceContext()))
	{
		MessageBox(NULL, L"Failed To Update Position String", L"Error", MB_OK);
	}
	
	m_Position->GetRotation(x, y, z);

	strcpy_s(text, "ROT (X: ");
	_itoa_s((int)x, itoaTemp, 10);
	strcat_s(text, itoaTemp);

	strcat_s(text, " Y: ");
	_itoa_s((int)y, itoaTemp, 10);
	strcat_s(text, itoaTemp);

	strcat_s(text, " Z: ");
	_itoa_s((int)z, itoaTemp, 10);
	strcat_s(text, itoaTemp);

	strcat_s(text, ")");

	if(!m_Text->UpdateString("ROT", text, m_D3D->GetDeviceContext()))
	{
		MessageBox(NULL, L"Failed To Update Rotation String", L"Error", MB_OK);
	}

	float drawpct = (float)m_QuadTree->GetDrawCount() / (float)m_QuadTree->GetTriangleCount();
	_itoa_s(drawpct*100, itoaTemp, 10);
	strcpy_s(text, "Draw: ");
	strcat_s(text, itoaTemp);
	strcat_s(text, "%");
	
	if(!m_Text->UpdateString("QTR", text, m_D3D->GetDeviceContext()))
	{
	}

	_itoa_s(m_QuadTree->GetDrawCount(), itoaTemp, 10);
	strcpy_s(text, "Tri's: ");
	strcat_s(text, itoaTemp);

	if(!m_Text->UpdateString("XCS", text, m_D3D->GetDeviceContext()))
	{
	}

	if(!HandleInput(m_Timer->GetTime()))
	{
		return false;
	}

	if(!RenderGraphics())
	{
		return false;
	}

	return true;
}

bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	m_Position->SetFrameTime(frameTime);

	m_Position->UpdateInput(m_Input);

	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	if(m_Input->IsKeyPressed(DIK_F4))
	{
		if(!m_bF3Down)
		{
			m_D3D->ToggleFillMode();
			m_bF3Down = true;
		}
	}
	else if(m_bF3Down)
	{
		m_bF3Down = false;
	}


	return true;
}

bool ApplicationClass::RenderGraphics()
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	bool result;

	m_D3D->BeginScene(0.f, 0.f, 0.0f, 1.0f);

	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_Frustrum->ConstructFrustrum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	result = m_TerrainShader->SetShaderParamaters(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Terrain->GetTexture());
	if(!result)
	{
		return false;
	}

	m_QuadTree->Render(m_Frustrum, m_D3D->GetDeviceContext(), m_TerrainShader);

	m_D3D->TurnZBufferOff();

	m_D3D->TurnOnAlphaBlending();

	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	m_D3D->TurnOffAlphaBlending();

	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;
}