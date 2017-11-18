#ifndef __j1EntityFactory_H__
#define __j1EntityFactory_H__

#include "j1Module.h"
#include "Animation.h"

#include "p2Point.h"
#include "CatPeasant.h"
#include "Imp.h"
#include "Monkey.h"
#include "Player.h"

#define MAX_ENTITIES 10

struct SDL_Texture;

// Utility: use ENTITY_TYPES values to assign a type to enemies in Tiled 
enum ENTITY_TYPES
{
	NO_TYPE,
	IMP_,
	CAT_PEASANT_,
	MONKEY_,
	PLANT_,
	PLAYER_,
};

class Entity;

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

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y;
	PathInfo* path = nullptr;

	~EntityInfo() {
		RELEASE_ARRAY(path);
	}
};

class j1EntityFactory : public j1Module
{
public:

	j1EntityFactory();
	virtual ~j1EntityFactory();
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

	bool AddEntities();
	bool AddEntity(ENTITY_TYPES type, uint path);

	// Get entities info
	CatPeasantInfo& GetCatPeasantInfo() { return catPeasant; }
	ImpInfo& GetImpInfo() { return imp; }
	MonkeyInfo& GetMonkeyInfo() { return monkey; }
	PlayerInfo& GetPlayerInfo() { return player; }

	// Get paths info
	PathInfo* GetPathByIndex(uint index) const;

	bool Save(pugi::xml_node& save) const;

	bool Load(pugi::xml_node& save);

private:

	void SpawnEntity(const EntityInfo& info);

private:

	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];

	p2SString CatPeasant_spritesheet;
	p2SString Monkey_spritesheet;
	p2SString Imp_spritesheet;
	p2SString Player_spritesheet;

	// Enemies info
	ImpInfo imp;
	MonkeyInfo monkey;
	PlayerInfo player;
	CatPeasantInfo catPeasant;

	// Paths info list
	p2List<PathInfo*> paths;

public:
	SDL_Texture* CatPeasantTex = nullptr;
	SDL_Texture* MonkeyTex = nullptr;
	SDL_Texture* ImpTex = nullptr;
	SDL_Texture* PlayerTex = nullptr;

	Player* playerData;

};

#endif // __j1EntityFactory_H__