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

	// ��ġ ����, relative�� true�̸�, ���� ��ġ���� �Ű�������ŭ �̵�
	virtual void SetPos(const double& x, const double& y, bool relative);

	const double& GetX() const;
	const double& GetY() const;
	const Gdiplus::Rect& GetRect() const;

	void Draw(Gdiplus::Graphics* g);
	virtual void OnUpdate(double deltaTime) { };

	bool Intersect(Gdiplus::Rect& r);
};