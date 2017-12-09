#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "p2DynArray.h"
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
	Animation* animation = nullptr;

public:
	float deltaTime = 0.0f;

	int type = 0;
	int lives = 0;
	bool dead = false;

	// Entity movement
	fPoint position = { 0,0 };
	iPoint i_pos = { 0,0 };
	fPoint start_pos = { 0,0 };

	fPoint speed = { 0,0 };

	fPoint last_pos = { 0,0 };
	bool up = false, down = false, left = false, right = false;
	//_enemy_movement

	// Particles
	iPoint collider_pos = { 0,0 };
	iPoint collider_size = { 0,0 };

public:
	Entity(float x, float y);
	virtual ~Entity();

	const Collider* GetCollider() const;

	virtual void Move(float dt) {};
	virtual void LoadAnimationsSpeed() {};
	virtual void UpdateAnimations(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider, Collider* c2);
};

#endif // __ENTITY_H__