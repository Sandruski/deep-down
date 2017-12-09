#ifndef __IMP_H__
#define __IMP_H__

#include "Entity.h"

enum ImpState {
	r_shield_idle,
	l_shield_idle,
	r_shield_hurt,
	l_shield_hurt,
	r_jump,
	l_jump,
	r_throw_bomb,
	l_throw_bomb,
	r_shield_walk,
	l_shield_walk,
	back_to_start
};

struct ImpInfo
{
	Animation r_shield_idle, l_shield_idle;
	Animation r_shield_hurt, l_shield_hurt;
	Animation r_jump, l_jump;
	Animation r_throw_bomb, l_throw_bomb;
	Animation r_shield_walk, l_shield_walk;
	Animation invisible;

	iPoint coll_size = { 0,0 };
	SDL_Rect coll_offset = { 0,0 };

	int lives = 0;

	SDL_Rect enemy_pathfinding_affect_area = { 0,0,0,0 };
	SDL_Rect player_pathfinding_affect_area = { 0,0,0,0 };
	float pathfinding_slow_speed = 0.0f;
	float pathfinding_normal_speed = 0.0f;
	float pathfinding_fast_speed = 0.0f;
	int min_distance_to_pathfind = 0;
	int min_distance_to_shoot = 0;
	int seconds_to_wait = 0;
	int scene1_pathfinding_start = 0;
	int distance_to_player = 0;

	fPoint particle_speed = { 0,0 };

	uint hurt_fx = 0;
};

class Imp : public Entity
{
public:

	Imp(float x, float y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move(float dt);
	void UpdateAnimations(float dt);
	void LoadAnimationsSpeed();

private:

	void GeneralStatesMachine();
	void UpdateDirection();

	// Pathfinding
	void UpdatePathfinding();

	void UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player);
	bool ResetPathfindingVariables();
	void UpdateMovement(iPoint to_go, float velocity);
	bool CreatePathfinding(iPoint destination);
	bool Pathfind(float velocity);
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
	void IsGround(iPoint& pos);
	void Wounded();

private:

	ImpInfo imp;
	ImpState impState;

	bool right_hit = false, left_hit = false;
	bool do_hit = true;
	bool wait = false;
	bool cool = false;
	float cooldown = 0;

	bool back = false;
	bool left_hurt = false, right_hurt = false;
	bool stop = false;

	// Animations speed
	float r_shield_idle_speed = 0.0f, l_shield_idle_speed = 0.0f;
	float r_shield_hurt_speed = 0.0f, l_shield_hurt_speed = 0.0f;
	float r_jump_speed = 0.0f, l_jump_speed = 0.0f;
	float r_throw_bomb_speed = 0.0f, l_throw_bomb_speed = 0.0f;
	float r_shield_walk_speed = 0.0f, l_shield_walk_speed = 0.0f;
	float invisible_speed = 0.0f;

	// Pathfinding
	uint pathfinding_index = 0;
	uint pathfinding_size = 0;

	bool create_pathfinding = false;
	bool pathfinding_stop = false;
	bool pathfinding_finished = true;
	bool pathfinding = false;
	bool pathfind = false;
	bool go = false;

	bool grounded = false;

	iPoint dest = { 0,0 };
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