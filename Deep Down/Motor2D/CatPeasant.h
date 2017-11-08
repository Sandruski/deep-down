#ifndef __CATPEASANT_H__
#define __CATPEASANT_H__

#include "Enemy.h"



class CatPeasant : public Enemy
{
private:

	Animation idle, idle2, idleNoStuff, idleNoStuff2, hurt, hurt2, hurtNoStuff, hurtNoStuff2, death, death2, deathNoStuff, deathNoStuff2, throwStuff, throwStuff2;

	stateEnemies catPeasantState;

	iPoint lastPosition;

	bool toBackward, toForward, toUp, toDown;

private:

	void ActualDirection();
	void SetDirectionBoolsToFalse();
	void GeneralStatesMachine();

public:

	CatPeasant(int x, int y);
	void OnCollision(Collider* c1, Collider* c2);
	void Move();
};

#endif