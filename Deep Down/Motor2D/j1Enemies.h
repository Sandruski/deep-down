#ifndef __j1Enemies_H__
#define __j1Enemies_H__

#include "j1Module.h"
#include "Animation.h"

#include "p2Point.h"
#include "Imp.h"

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

enum PATH_TYPES {
	NO_TYPE,
	PATH_1,
	PATH_2,
	PATH_3

};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};

struct PathInfo
{
	PATH_TYPES type = PATH_TYPES::NO_TYPE;

	iPoint start_pos = { 0,0 };
	iPoint* path = nullptr;

	PathInfo();
	PathInfo(const PathInfo& i);
	~PathInfo();
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
	bool LoadPaths();

	// Get enemies info
	ImpInfo& GetImpInfo() { return imp; }

private:

	void SpawnEnemy(const EnemyInfo& info);

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];

	p2SString CatPeasant_spritesheet;
	p2SString MonkeyPlant_spritesheet;
	p2SString Imp_spritesheet;

	// Enemies info
	ImpInfo imp;

	// Paths info
	PathInfo path_1;
	PathInfo path_2;
	PathInfo path_3;

public:
	SDL_Texture* CatPeasantTex = nullptr;
	SDL_Texture* MonkeyPlantTex = nullptr;
	SDL_Texture* ImpTex = nullptr;

};

#endif // __j1Enemies_H__