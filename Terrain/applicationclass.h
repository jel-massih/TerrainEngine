#ifndef APPLICATIONCLASS_H
#define APPLICATIONCLASS_H

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "InputClass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "camerapositionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "lightclass.h"
#include "frustrumclass.h"
#include "quadtreeclass.h"
#include "debugtreeclass.h"
#include "shadermanagerclass.h"
#include "terrainshaderclass.h"

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass& other);
	~ApplicationClass();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();

private:
	InputClass* m_Input;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	CameraPositionClass* m_Position;
	FPSClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	LightClass* m_Light;
	FrustrumClass* m_Frustrum;
	QuadTreeClass* m_QuadTree;
	DebugTreeClass* m_DebugTree;
	ShaderManagerClass* m_ShaderManager;
	TerrainShaderClass* m_TerrainShader;

	bool m_bF3Down;
};

#endif