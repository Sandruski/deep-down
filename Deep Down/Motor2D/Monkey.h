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

struct MonkeyInfo
{
	Animation r_idle, l_idle;
	Animation r_hurt, l_hurt;
	Animation r_hit, l_hit;

	iPoint coll_size;
	SDL_Rect coll_offset;

	MonkeyInfo();
	MonkeyInfo(const MonkeyInfo& i);
	~MonkeyInfo();

};

class Monkey : public Enemy
{
public:

	Monkey(int x, int y, PathInfo* path);

	void OnCollision(Collider* c1, Collider* c2);
	void Move();

private:

	void GeneralStatesMachine();
	void UpdateDirection();

private:

	const p2DynArray<iPoint>* last_pathfinding;

	MonkeyInfo* monkey;
	MonkeyState monkeyState;

	bool up, down, left, right;

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

	bool create_path = true;

	bool do_path;

	bool goal_is_start;
	iPoint goal;

	//_normal_path
};

#endif