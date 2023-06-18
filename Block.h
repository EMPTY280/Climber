#pragma once

#include "MyObject.h"

class Block : public MyObject
{
private:
	double life = 2;
	double lifeMax = 2;
	double collapseSpeed = 2;
	double recoverSpeed = 1;

	bool collapse = false;
	bool recover = false;

	bool bounce = false;
	double bounceSpeed = 4;

	// Ȱ��ȭ�� �Ǿ����� �浹, ������ ����
	bool off = false;

public:
	Block(double x, double y, CMyImage* image);
	void Update(double deltaTime);
	void StartCollapsing();
	void Reset();
	bool GetCollapse();

	void StartBounce();
	bool GetBounce();

	bool GetOff();

	virtual void SetActive(bool b) override;
};