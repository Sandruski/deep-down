#ifndef __IMP_H__
#define __IMP_H__

#include "Enemy.h"
#include "Path.h"

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

	Imp(int x, int y);

	void OnCollision(Collider* c1, Collider* c2);
	void Move();

private:

	void GeneralStatesMachine();
	void UpdateDirection();

private:

	ImpInfo* imp;
	ImpState impState;

	bool up, down, left, right;
	bool stop_x, stop_y;
	bool throw_bomb;

	fPoint speed;
	// Keep track of enemy movement
	iPoint last_pos;

	// Pathfinding
	bool create_path;
	p2DynArray<iPoint> last_path;
	uint index = 0;
	bool path_finished;

	// Normal path
	Path normal_path;

};

#endif