#ifndef __IMP_H__
#define __IMP_H__

#include "Enemy.h"

class Imp : public Enemy
{
private:

	ImpInfo imp;

public:

	Imp(int x, int y);

	void OnCollision(Collider* c1, Collider* c2);
	void Move();
};

#endif