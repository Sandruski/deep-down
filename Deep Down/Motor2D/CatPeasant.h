#ifndef __CATPEASANT_H__
#define __CATPEASANT_H__

#include "Enemy.h"

class CatPeasant : public Enemy
{
private:

	Animation idle, idle2, forward, backward;

public:

	CatPeasant(int x, int y);
	void OnCollision(Collider* c1, Collider* c2);
	void Move();
};

#endif