#ifndef __Monkey_H__
#define __Monkey_H__

#include "Enemy.h"

class Monkey : public Enemy
{
private:

public:

	Monkey(int x, int y, PathInfo* path);
	void OnCollision(Collider* c1, Collider* c2);
	void Move();
};

#endif