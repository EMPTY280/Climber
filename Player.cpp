#include "Player.h"

Player::Player(double x, double y, CMyImage* image) : MyObject(x, y, image) { }

void Player::SetPos(const double& x, const double& y, bool relative)
{
	if (relative)
	{
		this->x += x;

		double dist = 0;

		while (dist < y)
		{
			int size = collisionTable->GetSize();
			for (int i = 0; i < size; i++)
			{
				if (rect.Intersect(collisionTable->GetData(i).GetRect()))
				{
					break;
				}
			}

			if (y >= 1)
			{
				this->y +
			}
			else
			{

			}
		}

		//this->y += y;
		return;
	}

	this->x = x;
	this->y = y;
}

void Player::OnUpdate(double deltaTime)
{
	vSpeed += gravityPower * deltaTime;

	if (vSpeed > gravityPower)
		vSpeed = gravityPower;

	SetPos(0, vSpeed, true);
}

void Player::Jump()
{
	vSpeed = -jumpPower;
}

void Player::SetCollisionTable(DataList<MyObject>* t)
{
	this->collisionTable = t;
}
