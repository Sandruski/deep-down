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

	iPoint coll_size;
	SDL_Rect coll_offset;
};

class CatPeasant : public Entity
{
public:

	CatPeasant(float x, float y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move(float dt);
	void UpdateAnimations(float dt);

private:

	void GeneralStatesMachine();
	void UpdateDirection();

	// Pathfinding
	bool CreatePathfinding(iPoint destination);
	bool Pathfind();
	void UpdateMovement(iPoint to_go);
	//_pathfinding

	// Normal path
	void UpdatePath();

	bool ResetNormalPathVariables();
	void RecalculatePath();
	void FindDestination(iPoint& to_go);
	//_normal_path

	void Hit();
	void DoHit();
	void CoolDown();

private:

	CatPeasantInfo catPeasant;
	CatPeasantState catPeasantState;

	bool right_hit, left_hit;
	bool do_hit = true;
	bool wait;
	bool cool;
	float cooldown;
	int seconds_to_wait;

	// Pathfinding
	uint pathfinding_index = 0;
	uint pathfinding_size = 0;
	//_pathfinding

	// Normal path
	StartEndPath normal_path_index = StartEndPath::end;

	bool normal_path_finished = true;
	bool create_normal_path;
	bool do_normal_path;

	bool create_pathfinding_back;
	bool going_back_home;
	//_normal_path
};

#endif