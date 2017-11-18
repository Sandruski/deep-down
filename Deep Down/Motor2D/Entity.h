#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
struct PathInfo;

enum StartEndPath {
	start,
	end
};

class Entity
{
protected:
	
	Collider* collider = nullptr;
	PathInfo* path_info = nullptr;
	Animation* animation = nullptr;

	const p2DynArray<iPoint>* last_pathfinding;
	p2DynArray<iPoint> mlast_pathfinding;

public:
	
	float deltaTime;

	int type;
	int lives;

	// Enemy movement
	fPoint position;
	iPoint i_pos;
	fPoint start_pos;

	fPoint speed;

	// Keep track of enemy movement
	fPoint last_pos;
	bool up, down, left, right;
	//_enemy_movement

	// Particles
	fPoint particle_speed;
	float distance_to;

	iPoint collider_pos;
	iPoint collider_size;

public:
	Entity(float x, float y, PathInfo* path);
	virtual ~Entity();

	const Collider* GetCollider() const;

	virtual void Move(float dt) {};
	virtual void UpdatePath() {};
	virtual void UpdatePathfinding() {};
	virtual void UpdateAnimations(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider, Collider* c2);
};

#endif // __ENTITY_H__