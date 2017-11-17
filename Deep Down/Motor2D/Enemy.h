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
	const p2DynArray<iPoint>* last_pathfinding;
	p2DynArray<iPoint> mlast_pathfinding;

public:
	int type;

	// Enemy movement
	fPoint position;
	iPoint i_pos;
	fPoint start_pos;

	fPoint speed = { 1,1 };

	// Keep track of enemy movement
	iPoint last_pos;
	bool up, down, left, right;

	//_enemy_movement

	iPoint collider_pos;
	iPoint collider_size;

public:
	Enemy(float x, float y, PathInfo* path);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move(float dt) {};
	virtual void UpdatePath() {};
	virtual void UpdatePathfinding() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider, Collider* c2);
};

#endif // __ENEMY_H__