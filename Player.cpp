#include "Player.h"

Player::Player(double x, double y, CMyImage* image) : MyObject(x, y, image) { }

void Player::Move(double dt, bool left)
{
	int dir = 1;
	if (isLookingLeft != left)
	{
		image->Flip();
		jumpAnimation->Flip();
		walkAnimation[0]->Flip();
		walkAnimation[1]->Flip();
		walkAnimation[2]->Flip();
		walkAnimation[3]->Flip();
	}
	isLookingLeft = false;
	if (left)
	{
		isLookingLeft = true;
		dir *= -1;
	}
	hSpeed += moveAccel * dir * dt;
}

void Player::Jump(double mult)
{
	if (!isGrounded)
			return;

	vSpeed = -jumpPower * mult;
	isGrounded = false;
}

void Player::OnUpdate(double dt)
{
	// 공중일 경우
	if (!isGrounded) vSpeed += gravityPower * dt;

	if (vSpeed > gravityMax) vSpeed = gravityMax;

	if (hSpeed != 0)
	{
		animation += dt * (hSpeed > 0 ? hSpeed : -hSpeed);
		if (animation >= nextTiming)
		{
			animation = 0;
			animationIndex = (animationIndex + 1) % 4;
		}
	}

	if (hSpeed > 0)
	{
		hSpeed -= hFriction * dt;
		if (hSpeed < 0) hSpeed = 0;
	}
	else if (hSpeed < 0)
	{
		hSpeed += hFriction * dt;
		if (hSpeed > 0) hSpeed = 0;
	}

	if (hSpeed > moveSpeedMax) hSpeed = moveSpeedMax;
	if (hSpeed < -moveSpeedMax) hSpeed = -moveSpeedMax;

	x += hSpeed * dt * 100;
	y += vSpeed * dt * 100;
}

void Player::SetGrounded(bool b)
{
	isGrounded = b;
}

bool Player::GetGrounded()
{
	return isGrounded;
}

Gdiplus::Rect Player::GetRect()
{
	return Gdiplus::Rect((INT)x, (INT)y, 32, 32);
}

double Player::GetHSpeed()
{
	return hSpeed;
}

void Player::ResetHSpeed()
{
	hSpeed = 0;
}

void Player::ResetVspeed()
{
	vSpeed = 0;
	y += 10;
}

void Player::Draw(Gdiplus::Graphics* g)
{
	if (!isGrounded)
	{
		jumpAnimation->Draw(g, x, y); // 공중 이미지
		return;
	}

	if (hSpeed > 0.5 || hSpeed < -0.5)
	{
		walkAnimation[animationIndex]->Draw(g, x, y);
		return;
	}
	image->Draw(g, x, y);
}

void Player::SetWalkImage(CMyImage* img, int idx)
{
	walkAnimation[idx] = img;
}

void Player::SetJumpImage(CMyImage* img)
{
	jumpAnimation = img;
}
