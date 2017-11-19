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
};

class Monkey : public Entity
{
public:

	Monkey(float x, float y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move(float dt);
	void UpdateAnimations(float dt);

private:

	void GeneralStatesMachine();
	void UpdateDirection();

	// Pathfinding
	void UpdatePathfinding();

	void UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player);
	bool ResetPathfindingVariables();
	void UpdateMovement(iPoint to_go);
	bool CreatePathfinding(iPoint destination);
	bool Pathfind();
	//_pathfinding

	// Normal path
	void UpdatePath();

	bool DoNormalPath();
	void RecalculatePath();
	void UpdateNormalPathMovement(iPoint to_go);
	void FlipPath(PathInfo* path_info);
	//_normal_path

	void Hit();
	void DoHit();
	void Wounded();

private:

	MonkeyInfo monkey;
	MonkeyState monkeyState;

	bool right_hit = false, left_hit = false;
	bool do_hit = true;
	bool wait = false;

	bool left_hurt = false, right_hurt = false;
	bool stop = false;

	// Pathfinding
	uint pathfinding_index = 0;
	uint pathfinding_size = 0;

	bool create_pathfinding = false;
	bool pathfinding_stop = false;
	bool pathfinding_finished = true;
	bool pathfinding = false;
	bool pathfind = false;

	Collider* follow_pathfinding1 = nullptr;
	Collider* follow_pathfinding2 = nullptr;
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