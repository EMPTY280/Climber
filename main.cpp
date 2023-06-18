#include <map>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "MyImage.h"

#include "DataList.h"
#include "MyObject.h"
#include "Player.h"
#include "Block.h"

#pragma region GraphicBuffer
Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;
#pragma endregion

const int WINDOW_W = 480;
const int WINDOW_H = 640;
const int FLOOR_TILE_W = 15;
const int FLOOR_TILE_H = 20;

int scrollSpeed = 110;

double scroll = 32;

DataList<CMyImage*> imageList;
DataList<CMyImage*> scoreImage;
Player* player;
DataList<Block*> blockList; // type 1
DataList<Block*> blockListBouncy; // type 2
DataList<Block*> blockListMagnet; // type 3
DataList<Block*> blockListCollapse; // type 4

enum class CollisionType
{
	Head,
	Foot,
	Side,
	None
};

enum class InputType
{
	Up,			// 누르고 있지 않을 때
	Down,		// 누르고 있을 때 (pressed 확인에서 바꿔줌)
	Pressed		// 누른 직후
};

InputType keys[256] = { InputType::Up, };

bool gameStart = false;
bool gameover = false;

double screenShake = 0;

#pragma region ForwardDeclaration

void OnUpdate(HWND, double);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void inline Initialize();
void LoadImage(char*);
void LoadDigit(char*);
bool OnKey(WPARAM key);
void Input(double);
void Render();
CollisionType CheckCollisionVertical(Block& block);
CollisionType CheckCollisionHorizontal(Block& block, double);
void ScrollBlock(Block& b, double deltaTime);
Block& GetAvailableBlock(int type);
void LoadFloor();
void ResetGame();
#pragma endregion

int nextFloor[20][15] = { 0, };
int floors = 6; // 계층 데이터의 개수, 0번 제외
int readingPosY = FLOOR_TILE_H - 1; // 어디를 읽는 중인지

int score = 0;

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

	HWND hwnd = CreateWindow("myGame", "Climber",
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
			double deltaTime = static_cast<double>(curTick - tick) * 0.001;

			Input(deltaTime);
			OnUpdate(hwnd, deltaTime);
			Render();
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
		keys[wParam] = keys[wParam] == InputType::Pressed ? InputType::Down : InputType::Pressed;
		break;
	case WM_KEYUP:
		keys[wParam] = InputType::Up;
		break;
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void LoadFloor()
{
	score++;

	int xx = 0;
	int yy = 0;

	std::ifstream file;

	std::string directory = "./Data/Floor/floor_type_";
	directory += std::to_string((rand() % floors) + 1);
	directory += ".txt";

	file.open(directory);

	while (!file.eof())
	{
		file >> nextFloor[yy][xx];

		xx++;
		if (xx == FLOOR_TILE_W)
		{
			xx = 0;
			yy++;
		}
	}
	file.close();
	readingPosY = FLOOR_TILE_H - 1;
}

void LoadFirstFloor()
{

	std::ifstream file;
	file.open("./Data/Floor/floor_type_0.txt");

	if (!file.is_open())
		return;

	int x = 0;
	int y = 0;

	while (!file.eof())
	{
		int tile;
		file >> tile;
		if (tile != 0)
		{
			Block& b = GetAvailableBlock(tile);
			b.SetActive(true);
			b.SetPos(x * 32, y * 32, false);
		}
		x++;
		if (x == FLOOR_TILE_W)
		{
			x = 0;
			y++;
		}
	}

	file.close();
}

void inline Initialize()
{
	LoadImage("./Data/Image/Player.png");	// 0 Player
	LoadImage("./Data/Image/Block.png");	// 1 Block
	LoadImage("./Data/Image/Bouncy.png");	// 2 Block_Bouncy
	LoadImage("./Data/Image/Magnet.png");	// 3 Block_Magnet
	LoadImage("./Data/Image/Collapse.png");	// 4 Block_Collapse
	LoadImage("./Data/Image/Border.png");	// 5 Border
	LoadImage("./Data/Image/Player_1.png");	// 6 Player_Walk_1
	LoadImage("./Data/Image/Player_2.png");	// 7 Player_Walk_2
	LoadImage("./Data/Image/Player_3.png");	// 8 Player_Jump
	LoadImage("./Data/Image/Ready.png");	// 9 Ready
	LoadImage("./Data/Image/Over.png");	// 10 Game Over

	LoadDigit("./Data/Image/Num_0.png");
	LoadDigit("./Data/Image/Num_1.png");
	LoadDigit("./Data/Image/Num_2.png");
	LoadDigit("./Data/Image/Num_3.png");
	LoadDigit("./Data/Image/Num_4.png");
	LoadDigit("./Data/Image/Num_5.png");
	LoadDigit("./Data/Image/Num_6.png");
	LoadDigit("./Data/Image/Num_7.png");
	LoadDigit("./Data/Image/Num_8.png");
	LoadDigit("./Data/Image/Num_9.png");
	LoadDigit("./Data/Image/ScoreBoard.png");

	
	player = new Player(224, 288, &imageList[0]);
	player->SetJumpImage(&imageList[8]);
	player->SetWalkImage(&imageList[6], 0);
	player->SetWalkImage(&imageList[0], 1);
	player->SetWalkImage(&imageList[7], 2);
	player->SetWalkImage(&imageList[0], 3);

	std::ifstream file;
	file.open("./Data/Floor/config.txt");

	if (file.is_open())
	{
		std::string temp;
		file >> temp;
		file >> floors;
		file.close();
	}

	// 다음 층 받아오기
	srand((unsigned int)time(NULL));
	LoadFloor();

	score = 0;

	// 1층 받아오기
	LoadFirstFloor();
}

void LoadImage(char* dir)
{
	CMyImage* img = new CMyImage();
	img->Load(dir);
	imageList.AddData(img);
}

void LoadDigit(char* dir)
{
	CMyImage* img = new CMyImage();
	img->Load(dir);
	scoreImage.AddData(img);
}

// 키를 누를 때
bool OnKey(WPARAM key)
{
	if (keys[key] == InputType::Down ||
		keys[key] == InputType::Pressed)
		return true;
	return false;
}

// 키를 누른 직후
bool OnKeyPressed(WPARAM key)
{
	if (keys[key] == InputType::Pressed)
	{
		keys[key] = InputType::Down;
		return true;
	}
	return false;
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

void OnUpdate(HWND hWnd, double deltaTime)
{
	if(hWnd == NULL)
		return;

	int blockSize = blockList.GetSize();
	int bouncySize = blockListBouncy.GetSize();
	int magnetSize = blockListMagnet.GetSize();
	int collapseSize = blockListCollapse.GetSize();

	for (int i = 0; i < blockSize; i++) CheckCollisionHorizontal(blockList[i], deltaTime);
	for (int i = 0; i < bouncySize; i++) CheckCollisionHorizontal(blockListBouncy[i], deltaTime);
	for (int i = 0; i < magnetSize; i++) CheckCollisionHorizontal(blockListMagnet[i], deltaTime);
	for (int i = 0; i < collapseSize; i++) CheckCollisionHorizontal(blockListCollapse[i], deltaTime);

	player->OnUpdate(deltaTime);

	if (player->GetY() <= 0)
	{
		player->SetGrounded(false);
		player->ResetVspeed();
		player->SetPos(0, -player->GetY(), true);
	}

	player->SetGrounded(false);
	for (int i = 0; i < blockSize; i++) CheckCollisionVertical(blockList[i]);
	for (int i = 0; i < bouncySize; i++)
	{
		if (CheckCollisionVertical(blockListBouncy[i]) == CollisionType::Foot)
		{
			blockListBouncy[i].StartBounce();
			player->Jump(1.3);
		}
		blockListBouncy[i].Update(deltaTime);
	}
	for (int i = 0; i < magnetSize; i++)
	{
		if (CheckCollisionVertical(blockListMagnet[i]) == CollisionType::Foot) player->SetGrounded(false);
	}
	for (int i = 0; i < collapseSize; i++)
	{
		if (CheckCollisionVertical(blockListCollapse[i]) == CollisionType::Foot)
		{
			blockListCollapse[i].StartCollapsing();
		}
		blockListCollapse[i].Update(deltaTime);
	}

	
	if (screenShake > 0)
	{
		screenShake -= deltaTime * 10;
		if (screenShake <= 0)
			screenShake = 0;
	}

	// 게임오버 확인
	if (!gameover)
	{
		if (player->GetY() >= 672)
		{
			screenShake = 10;
			gameover = true;
		}
	}

	// 오토 스크롤
	if (gameover || !gameStart)
		return;

	if (player->GetGrounded())
		player->SetPos(0, scrollSpeed * deltaTime, true);
	for (int i = 0; i < blockSize; i++)
		ScrollBlock(blockList[i], deltaTime);
	for (int i = 0; i < bouncySize; i++)
		ScrollBlock(blockListBouncy[i], deltaTime);
	for (int i = 0; i < magnetSize; i++)
		ScrollBlock(blockListMagnet[i], deltaTime);
	for (int i = 0; i < collapseSize; i++)
		ScrollBlock(blockListCollapse[i], deltaTime);

	scroll += scrollSpeed * deltaTime;

	if (scroll >= 32)
	{
		scroll -= 32;
		for (int i = 0; i < FLOOR_TILE_W; i++)
		{

			int type = nextFloor[readingPosY][i];
			if (type == 0)
				continue;

			Block& b = GetAvailableBlock(type);
			b.SetActive(true);
			b.SetPos(i * 32, -32 + scroll, false);
		}
		readingPosY--;
	}

	if (readingPosY < 0)
		LoadFloor();
}

// 수직 방향 충돌 판정
CollisionType CheckCollisionVertical(Block& block)
{
	if (!block.GetActive() || block.GetOff())
		return CollisionType::None;

	if (Rect(player->GetX(), player->GetY() - 1, 32, 34).Intersect(block.GetRect()))
	{
		player->ResetVspeed();
		if (player->GetY() <= block.GetY()) // 착지
		{
			player->SetGrounded(true);
			player->SetPos(player->GetX(), block.GetY() - 32, false);
			return CollisionType::Foot;
		}
		else // 머리
		{
			player->SetPos(player->GetX(), block.GetY() + 40, false);
			return CollisionType::Head;
		}
	}
	return CollisionType::None;
}

// 수평 방향 충돌 판정
CollisionType CheckCollisionHorizontal(Block& block, double deltaTime)
{
	if (!block.GetActive() || block.GetOff())
		return CollisionType::None;

	if (Rect(player->GetX() + player->GetHSpeed() * 100 * deltaTime, player->GetY() + 5, 32, 27).Intersect(block.GetRect()))
	{
		player->ResetHSpeed();

		if (player->GetX() <= block.GetX())
			player->SetPos(block.GetX() - 32, player->GetY(), false);
		else
			player->SetPos(block.GetX() + 32, player->GetY(), false);
		return CollisionType::Side;
	}
}

void ScrollBlock(Block& b, double deltaTime)
{
	if (!b.GetActive())
		return;

	b.SetPos(0, scrollSpeed * deltaTime, true);

	if (b.GetY() > 672)
		b.SetActive(false);
}

void DrawBlock(Block b, Gdiplus::Graphics* g, int xx, int yy, int xShake, int yShake, bool border = false)
{
	if (!b.GetActive() || b.GetOff())
		return;
	if (border)
		b.Draw(g, xx + xShake, yy + yShake, &imageList[5]);
	else
		b.Draw(g, xx + xShake, yy + yShake);
}

void Render()
{
	Color color(0, 0, 0);
	g_BackBuffer->Clear(color);

	player->Draw(g_BackBuffer);

	int xShake = 0;
	int yShake = 0;

	int ShakePower = (int)screenShake;

	if (gameover && ShakePower > 0)
	{
		xShake = rand() % (int)ShakePower * 2 - ShakePower;
		yShake = rand() % (int)ShakePower * 2 - ShakePower;
	}

	int objSize = blockList.GetSize();
	for (int i = 0; i < objSize; i++)
	{
		int xx = blockList[i].GetX();
		bool isBorder = false;
		if (xx == 0 || xx == 448)
			isBorder = true;
		DrawBlock(blockList[i], g_BackBuffer, 0, 0, xShake, yShake, isBorder);
	}
	objSize = blockListBouncy.GetSize();
	for (int i = 0; i < objSize; i++)
	{
		int yy = 0;

		if (blockListBouncy[i].GetBounce())
			yy = rand() % 4;

		DrawBlock(blockListBouncy[i], g_BackBuffer, 0, yy, xShake, yShake);
	}
	objSize = blockListMagnet.GetSize();
	for (int i = 0; i < objSize; i++) DrawBlock(blockListMagnet[i], g_BackBuffer, 0, 0, xShake, yShake);
	objSize = blockListCollapse.GetSize();
	for (int i = 0; i < objSize; i++)
	{
		int xx = 0;
		int yy = 0;

		if (blockListCollapse[i].GetCollapse())
		{
			xx = rand() % 4;
			yy = rand() % 4;
		}

		DrawBlock(blockListCollapse[i], g_BackBuffer, xx, yy, xShake, yShake);
	}

	// UI

	if (!gameStart)
		imageList[9].Draw(g_BackBuffer, 240 - 128, 150 - 96);
	if (gameover && screenShake == 0)
		imageList[10].Draw(g_BackBuffer, 240 - 128, 240 - 96);

	scoreImage[10].Draw(g_BackBuffer, 20, 583);

	std::string scoreStr = std::to_string(score);

	int strSize = scoreStr.size();
	int idxPos = strSize - 1;

	for (int i = 0; i < 3; i++)
	{
		if (idxPos < 0)
		{
			scoreImage[0].Draw(g_BackBuffer, 65 - 13 * i, 591);
			continue;
		}

		int digit = (char)scoreStr.at(idxPos) - 48;
		scoreImage[digit].Draw(g_BackBuffer, 65 - 13 * i, 591);

		idxPos--;
	}
}

void Input(double deltaTime)
{
	if (OnKeyPressed(VK_SPACE))
	{
		if (!gameover && !gameStart)
			gameStart = true;
		else if (gameover && screenShake == 0)
			ResetGame();
	}

	if (!gameStart)
		return;

	if (OnKey(VK_SPACE))
		player->Jump();
	if (OnKey(VK_RIGHT))
		player->Move(deltaTime, false);
	if (OnKey(VK_LEFT))
		player->Move(deltaTime, true);
}

Block& GetAvailableBlock(int type)
{
	DataList<Block*>* dl = &blockList;

	switch (type)
	{
	case 2:
		dl = &blockListBouncy;
		break;
	case 3:
		dl = &blockListMagnet;
		break;
	case 4:
		dl = &blockListCollapse;
		break;
	}

	int size = dl->GetSize();

	for (int i = 0; i < size; i++)
	{
		if (!dl->GetData(i).GetActive())
			return dl->GetData(i);
	}
	Block* b = new Block(0, 0, &imageList[type]);
	dl->AddData(b);
	return *b;
}

void ResetGame()
{
	// 모든 블럭 비활성화
	int blockSize = blockList.GetSize();
	int bouncySize = blockListBouncy.GetSize();
	int magnetSize = blockListMagnet.GetSize();
	int collapseSize = blockListCollapse.GetSize();

	for (int i = 0; i < blockSize; i++) blockList[i].SetActive(false);
	for (int i = 0; i < bouncySize; i++) blockListBouncy[i].SetActive(false);
	for (int i = 0; i < magnetSize; i++) blockListMagnet[i].SetActive(false);
	for (int i = 0; i < collapseSize; i++) blockListCollapse[i].SetActive(false);

	// 게임오버 및 스타트 초기화
	gameover = false;
	gameStart = false;

	// 플레이어 위치 초기화
	player->SetPos(224, 288, false);
	player->ResetVspeed();

	// 맵 로드 변수 초기화
	LoadFloor();
	readingPosY = FLOOR_TILE_H - 1;
	scroll = 32;

	score = 0;

	// 첫 방 다시 생성
	LoadFirstFloor();
}