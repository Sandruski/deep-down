#ifndef __CATPEASANT_H__
#define __CATPEASANT_H__

#include "Enemy.h"

enum stateEnemies {
	enemyIdle_,
	enemyIdle2_,
	enemyForward_,
	enemyBackward_,
	enemyAttack_,
	enemyPunished_,
	enemyPunished2_,
	enemyDeath_,
	enemyDeath2_,
};

class CatPeasant : public Enemy
{
private:

	Animation idle, idle2, idleNoStuff, idleNoStuff2, hurt, hurt2, hurtNoStuff, hurtNoStuff2, death, death2, deathNoStuff, deathNoStuff2, throwStuff, throwStuff2;

	stateEnemies catPeasantState;

	iPoint lastPosition;

	bool toBackward, toForward, toUp, toDown;

	const p2DynArray<iPoint>* last_path;

	uint index = 0;

private:

	void ActualDirection();
	void SetDirectionBoolsToFalse();
	void GeneralStatesMachine();

public:

	CatPeasant(int x, int y, PathInfo* path);
	void OnCollision(Collider* c1, Collider* c2);
	void Move();
};

#endif