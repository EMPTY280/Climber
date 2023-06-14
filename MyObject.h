#pragma once
#include "MyImage.h"

class MyObject
{
protected:
	double x, y;
	CMyImage* image = nullptr;
	Gdiplus::Rect rect;

public:
	MyObject(double x, double y);
	MyObject(double x, double y, CMyImage* image);
	MyObject(double x, double y, CMyImage* image, Gdiplus::Rect rect);
	~MyObject();

	// 위치 변경, relative가 true이면, 현재 위치에서 매개변수만큼 이동
	virtual void SetPos(const double& x, const double& y, bool relative);

	const double& GetX() const;
	const double& GetY() const;
	const Gdiplus::Rect& GetRect() const;

	void Draw(Gdiplus::Graphics* g);
	virtual void OnUpdate(double deltaTime) { };

	bool Intersect(Gdiplus::Rect& r);
};