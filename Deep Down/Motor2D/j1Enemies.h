#ifndef __j1Enemies_H__
#define __j1Enemies_H__

#include "j1Module.h"

#define MAX_ENEMIES 4000

struct SDL_Texture;

enum ENEMY_TYPES
{
	NO_TYPE,
	IMP_,
	CAT_PEASANT_,
	MONKEY_,
	PLANT_,
	
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};

class j1Enemies : public j1Module
{
public:

	j1Enemies();
	virtual ~j1Enemies();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	bool AddEnemy(ENEMY_TYPES type, int x, int y);
	int dieE;
	int cont;

	bool follow = false;

private:

	void SpawnEnemy(const EnemyInfo& info);
	SDL_Texture* CatPeasantTxt = nullptr;

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];

};

#endif // __j1Enemies_H__