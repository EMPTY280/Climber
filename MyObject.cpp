#include "MyObject.h"

MyObject::MyObject(double x, double y) : x(x), y(y) { }

MyObject::MyObject(double x, double y, CMyImage* image) : x(x), y(y), image(image) { }

MyObject::MyObject(double x, double y, CMyImage* image, Gdiplus::Rect rect) : x(x), y(y), image(image), rect(rect) { }

MyObject::~MyObject() { }

void MyObject::SetPos(const double& x, const double& y, bool relative)
{
	if (relative)
	{
		this->x += x;
		this->y += y;
		return;
	}

	this->x = x;
	this->y = y;
}

const double& MyObject::GetX() const
{
	return this->x;
}

const double& MyObject::GetY() const
{
	return this->y;
}

const Gdiplus::Rect& MyObject::GetRect() const
{
	return this->rect;
}

void MyObject::Draw(Gdiplus::Graphics* g)
{
	if (image == nullptr)
		return;
	image->Draw(g, x, y);
}

bool MyObject::Intersect(Gdiplus::Rect& r)
{
	return rect.Intersect(r);
}
