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
	void LoadAnimationsSpeed();
	
private:

	CatInfo cat;

	// Animations speed
	float r_idle_speed = 0.0f, l_idle_speed = 0.0f;
	float r_going_ZZZ_speed = 0.0f, l_going_ZZZ_speed = 0.0f;
	float r_waking_up_speed = 0.0f, l_waking_up_speed = 0.0f;
	float r_to_crouch_speed = 0.0f, l_to_crouch_speed = 0.0f;
	float r_crouch_speed = 0.0f, l_crouch_speed = 0.0f;
	float r_rise_speed = 0.0f, l_rise_speed = 0.0f;
	float r_jump_speed = 0.0f, l_jump_speed = 0.0f;
	float r_fall_speed = 0.0f, l_fall_speed = 0.0f;
	float r_land_soft_speed = 0.0f, l_land_soft_speed = 0.0f;
	float r_roll_speed = 0.0f, l_roll_speed = 0.0f;
	float r_to_run_speed = 0.0f, l_to_run_speed = 0.0f;
	float r_run_speed = 0.0f, l_run_speed = 0.0f;
	float r_turn_speed = 0.0f, l_turn_speed = 0.0f;
	float r_brake_speed = 0.0f, l_brake_speed = 0.0f;
	float attack_speed = 0.0f;
};

#endif