#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
struct PathInfo;

class Enemy
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	PathInfo* path_info = nullptr;

public:
	iPoint position;
	iPoint start_pos;

	iPoint collider_pos;

	int type;

public:
	Enemy(int x, int y, PathInfo* path);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void UpdatePath() {};
	virtual void UpdatePathfinding() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider, Collider* c2);
};

#endif // __ENEMY_H__