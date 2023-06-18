#include <map>

#include "MyImage.h"
#include "DSpriteManager.h"

#include "DataList.h"
#include "MyObject.h"
#include "Player.h"

#pragma region GraphicBuffer
Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;
#pragma endregion

#pragma region ForwardDeclaration
void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void inline Initialize();
void LoadImage(char*);
#pragma endregion

const int WINDOW_W = 480;
const int WINDOW_H = 640;

DataList<CMyImage> imageList;
Player* player;
DataList<MyObject> blockList;

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

	//					테두리	 제목표시줄    최소화 상자     시스템 메뉴   보이기
	DWORD wndStyle = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE | WS_POPUP;

	RECT wndRect = { 0, 0, WINDOW_W, WINDOW_H };
	AdjustWindowRect(&wndRect, wndStyle, false);

	int width = wndRect.right - wndRect.left;
	int height = wndRect.bottom - wndRect.top;

	HWND hwnd = CreateWindow("myGame", "Game Window",
		wndStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

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
	DWORD tick = GetTickCount64();
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
			DWORD curTick = GetTickCount64();
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
	LoadImage("./Data/Image/Player.png");	// 0 Player
	LoadImage("./Data/Image/block.png");	// 1 Block
	
	player = new Player(30, 30, &imageList[0]);

	blockList.AddData(new MyObject(0, 68, &imageList[1]));
	blockList.AddData(new MyObject(32, 10, &imageList[1]));
	blockList.AddData(new MyObject(64, 100, &imageList[1]));
	blockList.AddData(new MyObject(96, 100, &imageList[1]));
	blockList.AddData(new MyObject(128, 100, &imageList[1]));
	blockList.AddData(new MyObject(160, 100, &imageList[1]));
	blockList.AddData(new MyObject(192, 68, &imageList[1]));

	for (int i = 0; i < 10; i++)
	{
		blockList.AddData(new MyObject(32 * i, 150, &imageList[1]));
	}
}

void LoadImage(char* dir)
{
	CMyImage* img = new CMyImage();
	img->Load(dir);
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
	/*
	
		!! INPUT !!

	*/

	if (keys[VK_SPACE] == true)
		player->Jump();
	if (keys[VK_RIGHT] == true)
		player->Move(deltaTime, false);
	if (keys[VK_LEFT] == true)
		player->Move(deltaTime, true);
	if (keys[VK_SHIFT] == true)
		player->SetPos(0, 0, false);

	/*

	!! UPDATE !!

	*/

	int size = blockList.GetSize();

	for (int i = 0; i < size; i++)
	{
		MyObject& block = blockList[i];

		// 블록 좌우 판정
		if (Rect(player->GetX() + player->GetHSpeed() * 100 * deltaTime, player->GetY(), 32, 32).Intersect(block.GetRect()))
		{
			player->ResetHSpeed();

			if (player->GetX() <= block.GetX())
				player->SetPos(block.GetX() - 32, player->GetY(), false);
			else
				player->SetPos(block.GetX() + 32, player->GetY(), false);


			break;
		}
	}

	player->OnUpdate(deltaTime);
	player->SetGrounded(false);

	for (int i = 0; i < size; i++)
	{
		MyObject& block = blockList[i];

		// 블록 상하 판정
		if (Rect(player->GetX(), player->GetY(), 32, 33).Intersect(block.GetRect()))
		{
			player->ResetVspeed();

			if (player->GetY() <= block.GetY()) // 착지
			{
				player->SetGrounded(true);
				player->SetPos(player->GetX(), block.GetY() - 32, false);
			}
			else // 머리
				player->SetPos(player->GetX(), block.GetY() + 32, false);

			break;
		}
	}


	if (player->GetGrounded())
		player->SetPos(0, 100 * deltaTime, true);
	for (int i = 0; i < size; i++)
	{
		MyObject& block = blockList[i];
		block.SetPos(0, 100 * deltaTime, true);
	}

	/*

	!! RENDER !!

	*/
	Color color(128, 128, 128);
	g_BackBuffer->Clear(color);

	player->Draw(g_BackBuffer);

	int objSize = blockList.GetSize();
	for (int i = 0; i < objSize; i++)
	{
		blockList[i].Draw(g_BackBuffer);
	}
}