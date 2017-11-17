#ifndef __Monkey_H__
#define __Monkey_H__

#include "Enemy.h"

enum MonkeyState {
	r_idle,
	l_idle,
	r_hurt,
	l_hurt,
	r_hit,
	l_hit
};

enum StartEndPath {
	start,
	end
};

struct MonkeyInfo
{
	Animation r_idle, l_idle;
	Animation r_hurt, l_hurt;
	Animation r_hit, l_hit;

	iPoint coll_size;
	SDL_Rect coll_offset;
};

class Monkey : public Enemy
{
public:

	Monkey(float x, float y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move();

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

	bool ResetNormalPathVariables();
	void RecalculatePath();
	void FindDestination(iPoint& to_go);
	//_normal_path

private:

	float dt;

	MonkeyInfo monkey;
	MonkeyState monkeyState;

	bool up, down, left, right;

	// Pathfinding
	uint pathfinding_index = 0;
	uint pathfinding_size = 0;

	bool create_pathfinding;
	bool pathfinding_stop;
	bool pathfinding_finished = true;
	bool pathfinding;
	bool pathfind;

	Collider* follow_pathfinding1;
	Collider* follow_pathfinding2;
	//_pathfinding

	// Normal path
	StartEndPath normal_path_index = StartEndPath::start;
	StartEndPath last_normal_path_index = StartEndPath::end;

	bool normal_path_finished = true;
	bool create_normal_path;
	bool do_normal_path;

	bool create_pathfinding_back;
	bool going_back_home;
	//_normal_path
};

#endif