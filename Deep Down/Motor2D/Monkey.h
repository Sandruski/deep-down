#ifndef __Monkey_H__
#define __Monkey_H__

#include "Entity.h"

enum MonkeyState {
	mr_idle, ml_idle,
	mr_hurt, ml_hurt,
	mr_hit,	ml_hit
};

struct MonkeyInfo
{
	Animation r_idle, l_idle;
	Animation r_hurt, l_hurt;
	Animation r_hit, l_hit;

	iPoint coll_size = { 0,0 };
	SDL_Rect coll_offset = { 0,0 };

	int lives = 0;

	SDL_Rect enemy_pathfinding_affect_area = { 0,0,0,0 };
	SDL_Rect player_pathfinding_affect_area = { 0,0,0,0 };
	float pathfinding_slow_speed = 0.0f;
	float pathfinding_normal_speed = 0.0f;
	int min_distance_to_hit = 0;
	int distance_to_player = 0;

	fPoint particle_speed = { 0,0 };

	uint hurt_fx = 0;
};

class Monkey : public Entity
{
public:

	Monkey(float x, float y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move(const float dt);
	void UpdateAnimations(const float dt);
	void LoadAnimationsSpeed();

private:

	void GeneralStatesMachine();
	void UpdateDirection();

	// Pathfinding
	void UpdatePathfinding();

	void UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player);
	bool ResetPathfindingVariables();
	void UpdateMovement(iPoint to_go, float velocity);
	bool CreatePathfinding(const iPoint destination);
	bool Pathfind(float velocity);
	//_pathfinding

	// Normal path
	void UpdatePath();

	bool DoNormalPath(float velocity);
	void RecalculatePath();
	void UpdateNormalPathMovement(const iPoint to_go, float velocity);
	void FlipPath(const PathInfo* path_info);
	//_normal_path

	void Hit();
	void DoHit();
	void Wounded();

private:

	MonkeyInfo monkey;
	MonkeyState monkeyState;

	bool right_hit = false, left_hit = false;
	bool wait = false;
	bool do_hit = true;
	bool left_hurt = false, right_hurt = false;
	bool stop = false;

	// Animations speed
	float r_idle_speed = 0.0f, l_idle_speed = 0.0f;
	float r_hurt_speed = 0.0f, l_hurt_speed = 0.0f;
	float r_hit_speed = 0.0f, l_hit_speed = 0.0f;

	// Pathfinding
	uint pathfinding_index = 0;
	uint pathfinding_size = 0;

	bool create_pathfinding = false;
	bool pathfinding_stop = false;
	bool pathfinding_finished = true;
	bool pathfinding = false;
	bool pathfind = false;
	//_pathfinding

	// Normal path
	uint normal_path_index = 0;
	uint last_normal_path_index = 0;

	bool create_pathfinding_back = false;
	bool going_back_home = false;
	iPoint home = { 0,0 };
	//_normal_path
};

#endif