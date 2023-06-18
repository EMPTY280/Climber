#pragma once
#include "MyImage.h"

class MyObject
{
protected:
	double x, y;
	CMyImage* image = nullptr;
	Gdiplus::Rect rect;

	// ��Ȱ��ȭ ����
	bool active = true;

public:
	MyObject(double x, double y);
	MyObject(double x, double y, CMyImage* image);
	~MyObject();

	// ��ġ ����, relative�� true�̸�, ���� ��ġ���� �Ű�������ŭ �̵�
	void SetPos(const double& x, const double& y, bool relative);

	double GetX() const;
	double GetY() const;
	const Gdiplus::Rect& GetRect() const;

	void Draw(Gdiplus::Graphics* g);
	void Draw(Gdiplus::Graphics* g, int xx, int yy);
	void Draw(Gdiplus::Graphics* g, int xx, int yy, CMyImage* img);
	bool Intersect(Gdiplus::Rect& r);

	void UpdateRect();
	Gdiplus::Rect GetRect();

	bool GetActive();
	virtual void SetActive(bool b);
};