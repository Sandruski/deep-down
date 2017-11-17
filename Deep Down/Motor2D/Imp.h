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
	l_shield_walk
};

enum StartEndPath {
	start,
	end
};

struct ImpInfo
{
	Animation r_shield_idle, l_shield_idle;
	Animation r_shield_hurt, l_shield_hurt;
	Animation r_jump, l_jump;
	Animation r_throw_bomb, l_throw_bomb;
	Animation r_shield_walk, l_shield_walk;

	iPoint coll_size;
	SDL_Rect coll_offset;
};

class Imp : public Entity
{
public:

	Imp(float x, float y, PathInfo* path);

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

	bool ResetNormalPathVariables();
	void RecalculatePath();
	void FindDestination(iPoint& to_go);
	//_normal_path

private:

	ImpInfo imp;
	ImpState impState;

	bool throw_bomb;

	// Pathfinding
	uint pathfinding_index = 0;
	uint pathfinding_size = 0;

	bool create_pathfinding;
	bool pathfinding_stop;
	bool pathfinding_finished = true;
	bool pathfinding;
	bool pathfind;
	bool go;

	Collider* follow_pathfinding1;
	Collider* follow_pathfinding2;
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