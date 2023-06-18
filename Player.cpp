#include "Player.h"

Player::Player(double x, double y, CMyImage* image) : MyObject(x, y, image) { }

void Player::Move(double dt, bool left)
{
	int dir = 1;
	if (left) dir *= -1;

	hSpeed += moveAccel * dir * dt;
}

void Player::Jump()
{
	if (!isGrounded)
			return;

	vSpeed = -jumpPower;
	isGrounded = false;
}

void Player::OnUpdate(double dt)
{
	// 공중일 경우
	if (!isGrounded) vSpeed += gravityPower * dt;

	if (vSpeed > gravityMax) vSpeed = gravityMax;

	if (hSpeed > 0)
	{
		hSpeed -= hFriction * dt;
		if (hSpeed < 0) hSpeed = 0;
	}
	if (hSpeed < 0)
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
}
