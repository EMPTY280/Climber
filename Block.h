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

	// 활성화는 되었지만 충돌, 렌더링 금지
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