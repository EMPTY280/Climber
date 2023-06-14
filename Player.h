#pragma once
#include "DataList.h"
#include "MyObject.h"

class Player : public MyObject
{
private:
	double gravityPower = 9.8;
	double vSpeed = 0;
	double jumpPower = 5;

	bool isGrounded = false;

	DataList<MyObject>* collisionTable;

public:
	Player(double x, double y, CMyImage* image);

	virtual void SetPos(const double& x, const double& y, bool relative) override;

	void OnUpdate(double deltaTime) override;
	void Jump();

	void SetCollisionTable(DataList<MyObject>* t);
};

