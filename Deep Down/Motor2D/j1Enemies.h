#ifndef __j1Enemies_H__
#define __j1Enemies_H__

#include "j1Module.h"
#include "Animation.h"

#include "p2Point.h"
#include "Imp.h"
#include "Monkey.h"
#include "Player.h"

#define MAX_ENEMIES 10

struct SDL_Texture;

enum ENEMY_TYPES
{
	NO_TYPE,
	IMP_,
	CAT_PEASANT_,
	MONKEY_,
	PLANT_,
	PLAYER_,
	
};

class Enemy;

struct PathInfo
{
	iPoint start_pos = { 0,0 };
	iPoint end_pos = { 0,0 };

	iPoint* path = nullptr;
	int path_size = 0;

	PathInfo();
	PathInfo(const PathInfo& i);

	~PathInfo() {
		RELEASE_ARRAY(path);
	}
};

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
	PathInfo* path = nullptr;

	~EnemyInfo() {
		RELEASE_ARRAY(path);
	}
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

	bool LoadPathsInfo();
	bool SaveRepetitivePaths(uint& index);
	bool SaveStartEndPaths(uint& index);

	bool AddEnemies();
	bool AddEnemy(ENEMY_TYPES type, uint path);

	// Get enemies info
	ImpInfo& GetImpInfo() { return imp; }
	MonkeyInfo& GetMonkeyInfo() { return monkey; }
	PlayerInfo& GetPlayerInfo() { return player; }

	// Get paths info
	PathInfo* GetPathByIndex(uint index) const;

private:

	void SpawnEnemy(const EnemyInfo& info);

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];

	p2SString CatPeasant_spritesheet;
	p2SString MonkeyPlant_spritesheet;
	p2SString Imp_spritesheet;
	p2SString Player_spritesheet;

	// Enemies info
	ImpInfo imp;
	MonkeyInfo monkey;
	PlayerInfo player;

	// Paths info list
	p2List<PathInfo*> paths;

public:
	SDL_Texture* CatPeasantTex = nullptr;
	SDL_Texture* MonkeyPlantTex = nullptr;
	SDL_Texture* ImpTex = nullptr;
	SDL_Texture* PlayerTex = nullptr;

	Player* playerData;

};

#endif // __j1Enemies_H__