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

	p2DynArray<iPoint> path1;

	ImpInfo();
	ImpInfo(const ImpInfo& i);
	~ImpInfo();

};

class Imp : public Enemy
{
public:

	Imp(int x, int y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move();

private:

	void GeneralStatesMachine();
	void UpdateDirection();

	// Pathfinding
	void UpdatePathfinding();

	void UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player);
	bool ResetPathfindingVariables();
	bool CreatePathfinding();
	void Pathfind();
	//_pathfinding

	// Normal path
	void UpdatePath();

	bool ResetPathfindingBackVariables();
	bool CreatePathfindingBack();

	bool DoNormalPath();
	void RecalculatePath();
	void FlipPath(PathInfo* path_info);
	//_normal_path

private:

	ImpInfo* imp;
	ImpState impState;

	bool up, down, left, right;
	bool stop_x, stop_y;
	bool throw_bomb;

	// Gravity
	fPoint speed;

	// Keep track of enemy movement
	iPoint last_pos;

	// Pathfinding
	uint pathfinding_index = 0;

	bool create_pathfinding;
	bool pathfinding_finished = true;
	bool pathfinding;
	bool pathfinding_stop;
	
	Collider* follow_pathfinding1;
	Collider* follow_pathfinding2;
	//_pathfinding

	// Normal path
	uint normal_path_index = 0;

	bool repeat_normal_path;
	int last_normal_path_index;

	bool create_pathfinding_back;
	bool going_back_home;
	//_normal_path
};

#endif