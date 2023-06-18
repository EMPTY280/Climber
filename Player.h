#pragma once
#include "DataList.h"
#include "MyObject.h"

class Player : public MyObject
{
private:

	CMyImage* walkAnimation[4];
	// 현재 이미지의 진행도
	double animation = 0;
	double nextTiming = 0.3;
	int animationIndex = 0;

	CMyImage* jumpAnimation;

	double gravityPower = 15;
	double gravityMax = 32;
	double jumpPower = 6;

	double moveAccel = 30;
	double moveSpeedMax = 30;

	double vSpeed = 0;
	double hSpeed = 0;
	double hFriction = 20;

	bool isGrounded = false;
	bool isLeftBlocked = false;
	bool IsRightBlocked = false;

	bool isLookingLeft = false;

public:
	Player(double x, double y, CMyImage* image);
	
	void Move(double dt, bool left);
	void Jump(double mult = 1.0);

	void OnUpdate(double dt);

	void SetGrounded(bool b);
	bool GetGrounded();

	Gdiplus::Rect GetRect();
	double GetHSpeed();
	void ResetHSpeed();
	void ResetVspeed();

	void Draw(Gdiplus::Graphics* g);

	void SetWalkImage(CMyImage* img, int idx);
	void SetJumpImage(CMyImage* img);
};

