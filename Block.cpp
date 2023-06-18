#include "Block.h"

Block::Block(double x, double y, CMyImage* image) : MyObject(x, y, image) { }

void Block::Update(double deltaTime)
{
	if (collapse)
	{
		life -= deltaTime * collapseSpeed;

		if (life <= 0)
		{
			life = 0;
			off = true;
			recover = true;
			collapse = false;
			return;
		}
	}
	if (recover)
	{
		life += deltaTime * recoverSpeed;

		if (life >= lifeMax)
		{
			life = lifeMax;
			off = false;
			recover = false;
			return;
		}
	}
	if (bounce)
	{
		life += deltaTime * bounceSpeed;

		if (life >= lifeMax)
		{
			life = lifeMax;
			bounce = false;
		}
	}
}

void Block::StartCollapsing()
{
	if (!recover)
	collapse = true;
}

void Block::Reset()
{
	recover = false;
	collapse = false;
	bounce = false;
	off = false;
	life = lifeMax;
}

bool Block::GetCollapse()
{
	return collapse;
}

void Block::StartBounce()
{
	bounce = true;
	life = 0;
}

bool Block::GetBounce()
{
	return bounce;
}

bool Block::GetOff()
{
	return off;
}

void Block::SetActive(bool b)
{
	MyObject::SetActive(b);
	if (!b)
		Reset();
}
