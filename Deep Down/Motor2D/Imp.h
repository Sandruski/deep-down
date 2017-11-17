#ifndef __IMP_H__
#define __IMP_H__

#include "Enemy.h"

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

class Imp : public Enemy
{
public:

	Imp(float x, float y, PathInfo* path);

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

	bool DoNormalPath();
	void RecalculatePath();
	void FlipPath(PathInfo* path_info);
	//_normal_path

private:

	float dt;

	ImpInfo imp;
	ImpState impState;

	bool up, down, left, right;
	bool stop_x, stop_y;
	bool throw_bomb;



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
	uint normal_path_index = 0;
	uint last_normal_path_index = 0;

	bool create_pathfinding_back;
	bool going_back_home;
	iPoint home;
	//_normal_path
};

#endif