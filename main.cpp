#include <map>

#include "MyImage.h"
#include "DSpriteManager.h"

#include "DataList.h"

Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;

void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void inline Initialize();

DataList<CMyImage> imageList;

double g_y = 10;
double g_x = 0;
double g_vspeed = 0;
double g_gravity = 9.8;

std::map<int, bool> keys;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,     int nCmdShow)
{
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = "myGame";
	
	if(RegisterClass(&wndclass) == 0)
	{
		return 0;		
	}

	HWND hwnd = CreateWindow("myGame", "Game Window",
		WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX,
		100, 100, 800, 600, NULL, NULL, hInstance, NULL);	

	if(hwnd == NULL)
	{
		return 0;
	}

	HDC hDC = GetDC(hwnd);

	CreateBuffer(hwnd, hDC);

	ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

	Initialize();

	MSG msg;
	DWORD tick = GetTickCount();
	while(1)
	{	
		//윈도우 메세지가 있을경우 메세지를 처리한다.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //메세지가 없을 경우 게임 루프를 실행한다.
		{		
			DWORD curTick = GetTickCount();
			OnUpdate(hwnd, curTick - tick);
			tick = curTick;

			g_MainBuffer->DrawImage(g_Bitmap, 0, 0);
		}
	}

	ReleaseBuffer(hwnd, hDC);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
		keys[wParam] = true;
		break;
	case WM_KEYUP:
		keys[wParam] = false;
		break;
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void inline Initialize()
{
	CMyImage* img = new CMyImage();
	img->Load("./Data/Image/Player.png");
	imageList.AddData(img);
}

void CreateBuffer(HWND hWnd, HDC hDC)
{
	GdiplusStartupInput			gdiplusStartupInput;
	ULONG_PTR					gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	RECT rc;
	GetClientRect(hWnd, &rc);

	g_Bitmap = new Bitmap(rc.right - rc.left, rc.bottom - rc.top);
	g_BackBuffer = new Graphics(g_Bitmap);
	g_BackBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);
	
	g_MainBuffer = new Graphics(hDC);
	g_MainBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);
}

void ReleaseBuffer(HWND hWnd, HDC hDC)
{
	ReleaseDC(hWnd, hDC);

	delete g_Bitmap;
	delete g_BackBuffer;
	delete g_MainBuffer;
}

void OnUpdate(HWND hWnd, DWORD tick)
{
	if(hWnd == NULL)
		return;

	double deltaTime = static_cast<double>(tick) * 0.001;

	Color color(255, 255, 255);
	g_BackBuffer->Clear(color);


	imageList.GetData(0).Draw(g_BackBuffer, g_x, g_y);

	if (keys[VK_SPACE] && g_y == 400)
		g_vspeed = -5;
	if (keys[VK_LEFT])
		g_x -= 300 * deltaTime;
	if (keys[VK_RIGHT])
		g_x += 300 * deltaTime;

	g_vspeed += g_gravity * deltaTime;
	g_y += g_vspeed;

	if (g_y >= 400)
	{
		g_vspeed = 0;
		g_y = 400;
	}
}