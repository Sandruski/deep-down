#ifndef __j1Enemies_H__
#define __j1Enemies_H__

#include "j1Module.h"
#include "Animation.h"

#include "p2Point.h"

#define MAX_ENEMIES 300

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
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	bool AddEnemy(ENEMY_TYPES type, int x, int y);

	ImpInfo GetImpInfo() const { return imp; }

private:

	void SpawnEnemy(const EnemyInfo& info);

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];

	ImpInfo imp;

	p2SString CatPeasant_spritesheet;
	p2SString MonkeyPlant_spritesheet;
	p2SString Imp_spritesheet;

public:
	SDL_Texture* CatPeasantTex = nullptr;
	SDL_Texture* MonkeyPlantTex = nullptr;
	SDL_Texture* ImpTex = nullptr;

};

#endif // __j1Enemies_H__