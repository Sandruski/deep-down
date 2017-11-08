#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

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

class Enemy
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;

public:
	iPoint position;

	fPoint start_pos;
	int type;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider, Collider* c2);
};

#endif // __ENEMY_H__