#ifndef __IMP_H__
#define __IMP_H__

#include "Enemy.h"

enum ImpState {
	r_shield_idle,
	l_shield_idle,
	r_shield_hurt,
	l_shield_hurt,
	r_jump,
	l_jump,
	r_throw_bomb,
	l_throw_bomb,
	r_shield_walk,
	l_shield_walk
};

class Imp : public Enemy
{
public:

	Imp(int x, int y);

	void OnCollision(Collider* c1, Collider* c2);
	void Move();

private:

	void GeneralStatesMachine();
	void UpdateDirection();

private:

	ImpInfo imp;
	ImpState impState;

	bool up, down, left, right;
	bool stop_x, stop_y;
	bool throw_bomb;

	fPoint speed;
	// Keep track of enemy movement
	iPoint last_pos;

};

#endif