#ifndef __CATPEASANT_H__
#define __CATPEASANT_H__

#include "Entity.h"

enum CatPeasantState {
	r_idle,	l_idle,
	r_walk,	l_walk,
	r_attack, l_attack,
	r_hurt,	l_hurt,
	r_dead,	l_dead,
};

struct CatPeasantInfo
{
	Animation r_idle, l_idle;
	Animation r_idle_no_staff, l_idle_no_staff;
	Animation r_hurt, l_hurt;
	Animation r_hurt_no_staff, l_hurt_no_staff;
	Animation r_dead, l_dead;
	Animation r_dead_no_staff, l_dead_no_staff;
	Animation r_throw_staff, l_throw_staff;

	iPoint coll_size = { 0,0 };
	SDL_Rect coll_offset = { 0,0,0,0 };

	int lives = 0;

	float pathfinding_normal_speed = 0;
	int min_distance_to_shoot = 0;
	int seconds_to_wait = 0;
	int distance_to_player = 0;

	fPoint particle_speed = { 0,0 };

	uint hurt_fx = 0;
};

class CatPeasant : public Entity
{
public:

	CatPeasant(float x, float y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move(const float dt);
	void UpdateAnimations(const float dt);
	void LoadAnimationsSpeed();

private:

	void GeneralStatesMachine();
	void UpdateDirection();

	// Pathfinding
	bool CreatePathfinding(const iPoint destination);
	bool Pathfind(float speed);
	void UpdateMovement(const iPoint to_go, float speed);
	//_pathfinding

	// Normal path
	void UpdatePath();

	bool ResetNormalPathVariables();
	void RecalculatePath();
	void FindDestination(iPoint& to_go) const;
	//_normal_path

	void Hit();
	void DoHit();
	void CoolDown();
	void Wounded();

private:

	CatPeasantInfo catPeasant;
	CatPeasantState catPeasantState;

	bool right_hit = false, left_hit = false;
	bool do_hit = true;
	bool wait = false;
	bool cool = false;
	float cooldown = 0;

	bool left_hurt = false, right_hurt = false;
	bool left_die = false, right_die = false;
	bool stop = false;

	// Animations speed
	float r_idle_speed = 0.0f, l_idle_speed = 0.0f;
	float r_idle_no_staff_speed = 0.0f, l_idle_no_staff_speed = 0.0f;
	float r_hurt_speed = 0.0f, l_hurt_speed = 0.0f;
	float r_hurt_no_staff_speed = 0.0f, l_hurt_no_staff_speed = 0.0f;
	float r_dead_speed = 0.0f, l_dead_speed = 0.0f;
	float r_dead_no_staff_speed = 0.0f, l_dead_no_staff_speed = 0.0f;
	float r_throw_staff_speed = 0.0f, l_throw_staff_speed = 0.0f;

	// Pathfinding
	uint pathfinding_index = 0;
	uint pathfinding_size = 0;
	//_pathfinding

	// Normal path
	StartEndPath normal_path_index = StartEndPath::end;

	bool normal_path_finished = true;
	bool create_normal_path = false;
	bool do_normal_path = false;

	bool create_pathfinding_back = false;
	bool going_back_home = false;
	//_normal_path
};

#endif