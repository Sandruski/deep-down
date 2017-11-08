#ifndef __IMP_H__
#define __IMP_H__

#include "Enemy.h"

class Imp : public Enemy
{
private:
	Animation r_shield_idle, l_shield_idle;
	Animation r_shield_hurt, l_shield_hurt;
	Animation r_jump, l_jump;
	Animation r_throw_bomb, l_throw_bomb;
	Animation r_shield_walk, l_shield_walk;

	Animation r_bomb, l_bomb;

public:

	Imp(int x, int y);

	void OnCollision(Collider* c1, Collider* c2);
	void Move();
};

#endif