#ifndef __Cat_H__
#define __Cat_H__

#include "Entity.h"

struct CatInfo
{
	Animation r_idle, l_idle;
	Animation r_going_ZZZ, l_going_ZZZ;
	Animation r_waking_up, l_waking_up;
	Animation r_to_crouch, l_to_crouch;
	Animation r_crouch, l_crouch;
	Animation r_rise, l_rise;
	Animation r_jump, l_jump;
	Animation r_fall, l_fall;
	Animation r_land_soft, l_land_soft;
	Animation r_roll, l_roll;
	Animation r_to_run, l_to_run;
	Animation r_run, l_run;
	Animation r_turn, l_turn;
	Animation r_brake, l_brake;
	Animation attack;

	iPoint coll_size = { 0,0 };
	SDL_Rect coll_offset = { 0,0 };
};

class Cat : public Entity
{
public:

	Cat(float x, float y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move(const float dt);
	void UpdateAnimations(const float dt);
	
private:

	CatInfo cat;
};

#endif