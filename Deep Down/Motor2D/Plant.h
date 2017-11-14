#ifndef __PLANT_H__
#define __PLANT_H__

#include "Enemy.h"

class Plant : public Enemy
{
private:
	Animation idle;

public:
	Plant(int x, int y, PathInfo* path);
	void OnCollision(Collider* c1, Collider* c2);
	void Move(uint index);
};

#endif