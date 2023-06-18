#pragma once
#include "DataList.h"
#include "MyObject.h"

class Player : public MyObject
{
private:
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

public:
	Player(double x, double y, CMyImage* image);
	
	void Move(double dt, bool left);
	void Jump();

	void OnUpdate(double dt);

	void SetGrounded(bool b);
	bool GetGrounded();

	Gdiplus::Rect GetRect();
	double GetHSpeed();
	void ResetHSpeed();
	void ResetVspeed();
};

