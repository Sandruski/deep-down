#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Enemy
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;

public:
	iPoint position;
	iPoint start_pos;

	iPoint collider_pos;

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