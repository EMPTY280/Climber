#include "MyObject.h"

MyObject::MyObject(double x, double y) : x(x), y(y) { }

MyObject::MyObject(double x, double y, CMyImage* image) : x(x), y(y), image(image), rect(Rect(x, y, 32, 32)) { }

MyObject::~MyObject() { }

void MyObject::SetPos(const double& x, const double& y, bool relative)
{
	if (!active)
		return;

	if (relative)
	{
		this->x += x;
		this->y += y;
		UpdateRect();
		return;
	}

	this->x = x;
	this->y = y;
	UpdateRect();
}

double MyObject::GetX() const
{
	return this->x;
}

double MyObject::GetY() const
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

void MyObject::Draw(Gdiplus::Graphics* g, int xx, int yy)
{
	if (image == nullptr)
		return;
	image->Draw(g, x + xx, y + yy);
}

void MyObject::Draw(Gdiplus::Graphics* g, int xx, int yy, CMyImage* img)
{
	img->Draw(g, x + xx, y + yy);
}

bool MyObject::Intersect(Gdiplus::Rect& r)
{
	return rect.Intersect(r);
}

void MyObject::UpdateRect()
{
	rect.X = x;
	rect.Y = y;
}

Gdiplus::Rect MyObject::GetRect()
{
	return rect;
}

bool MyObject::GetActive()
{
	return active;
}

void MyObject::SetActive(bool b)
{
	active = b;
}