#ifndef __CATPEASANT_H__
#define __CATPEASANT_H__

#include "Entity.h"

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

class CatPeasant : public Entity
{
private:

	Animation idle, idle2, idleNoStuff, idleNoStuff2, hurt, hurt2, hurtNoStuff, hurtNoStuff2, death, death2, deathNoStuff, deathNoStuff2, throwStuff, throwStuff2;

	stateEnemies catPeasantState;

	fPoint lastPosition;

	bool toBackward, toForward, toUp, toDown;

	const p2DynArray<iPoint>* last_path;

	uint index = 0;

private:

	void ActualDirection();
	void SetDirectionBoolsToFalse();
	void GeneralStatesMachine();

public:

	CatPeasant(float x, float y, PathInfo* path);
	void OnCollision(Collider* c1, Collider* c2);
	void Move();
};

#endif