#include "p2Defs.h"
#include "p2Log.h"

#include "j1Module.h"
#include "j1App.h"

#include "j1Enemies.h"
#include "j1Render.h"

#include "Enemy.h"
#include "Imp.h"
#include "CatPeasant.h"
#include "Monkey.h"
#include "Plant.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

#define SPAWN_MARGIN 50

j1Enemies::j1Enemies()
{
	name.create("enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Awake(pugi::xml_node& config) {

	bool ret = true;
	
	pugi::xml_node node = config.child("spritesheets").child("spritesheet");

	// Load texture paths
	CatPeasant_spritesheet = node.attribute("name").as_string();
	node = node.next_sibling("spritesheet");
	MonkeyPlant_spritesheet = node.attribute("name").as_string();
	node = node.next_sibling("spritesheet");
	Imp_spritesheet = node.attribute("name").as_string();
	//_load_texture_paths

	// IMP
	pugi::xml_node collider_node = config.child("types").child("imp").child("general").child("coll_offset");
	imp.coll_offset = { collider_node.attribute("x").as_int(), collider_node.attribute("y").as_int(), collider_node.attribute("w").as_int(), collider_node.attribute("h").as_int() };
	
	// Load animations
	pugi::xml_node animations_node = config.child("types").child("imp").child("animations");

	//r_shield_idle
	node = animations_node.child("r_shield_idle");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };

	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_shield_idle
	node = animations_node.child("l_shield_idle");
	imp.l_shield_idle.speed = node.attribute("speed").as_float();
	imp.l_shield_idle.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.l_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_shield_hurt
	node = animations_node.child("r_shield_hurt");
	imp.r_shield_hurt.speed = node.attribute("speed").as_float();
	imp.r_shield_hurt.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_hurt.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_shield_hurt
	node = animations_node.child("l_shield_hurt");
	imp.l_shield_hurt.speed = node.attribute("speed").as_float();
	imp.l_shield_hurt.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.l_shield_hurt.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_jump
	node = animations_node.child("r_jump");
	imp.r_jump.speed = node.attribute("speed").as_float();
	imp.r_jump.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_jump.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_jump
	node = animations_node.child("l_jump");
	imp.l_jump.speed = node.attribute("speed").as_float();
	imp.l_jump.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.l_jump.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_throw_bomb
	node = animations_node.child("r_throw_bomb");
	imp.r_throw_bomb.speed = node.attribute("speed").as_float();
	imp.r_throw_bomb.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_throw_bomb.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_throw_bomb
	node = animations_node.child("l_throw_bomb");
	imp.l_throw_bomb.speed = node.attribute("speed").as_float();
	imp.l_throw_bomb.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.l_throw_bomb.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_shield_walk
	node = animations_node.child("r_shield_walk");
	imp.r_shield_walk.speed = node.attribute("speed").as_float();
	imp.r_shield_walk.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_walk.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_shield_walk
	node = animations_node.child("l_shield_walk");
	imp.l_shield_walk.speed = node.attribute("speed").as_float();
	imp.l_shield_walk.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.l_shield_walk.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}
	//_load_animations
	//_IMP

	return ret;
}

bool j1Enemies::Start()
{
	// Load player textures
	LOG("Loading enemies textures");
	CatPeasantTex = App->tex->Load(CatPeasant_spritesheet.GetString());
	MonkeyPlantTex = App->tex->Load(MonkeyPlant_spritesheet.GetString());
	ImpTex = App->tex->Load(Imp_spritesheet.GetString());

	// Pathfinding collision data
	App->pathfinding->SetMap(App->map->data.width, App->map->data.height, (int*)App->map->collisionLayer->data);

	return true;
}

bool j1Enemies::PreUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			SpawnEnemy(queue[i]);
			queue[i].type = ENEMY_TYPES::NO_TYPE;
			LOG("Spawning enemy at %d", queue[i].y * App->scene->scale);
		}
	}
	
	return true;
}

// Called before render is available
bool j1Enemies::Update(float dt)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) {
			if (enemies[i]->type == ENEMY_TYPES::CAT_PEASANT_)
				enemies[i]->Draw(CatPeasantTex);
			else if (enemies[i]->type == ENEMY_TYPES::IMP_)
				enemies[i]->Draw(ImpTex);
		}

	return true;
}

bool j1Enemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
		//	if ((enemies[i]->position.y >((App->scene_1->current_start_pos + 114) - App->scene_1->cont + 88) + SPAWN_MARGIN) || enemies[i]->Esperanza == false)
		//	{
		//		LOG("DeSpawning enemy at %d", enemies[i]->position.y * SCREEN_SIZE);
				//delete enemies[i];
				//enemies[i] = nullptr;
			}
		}
	
	return true;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = ENEMY_TYPES::NO_TYPE;
			queue[i].x = NULL;
			queue[i].y = NULL;
		}

		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	App->tex->UnLoad(CatPeasantTex);
	App->tex->UnLoad(MonkeyPlantTex);

	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{

		switch (info.type)
		{	
		case ENEMY_TYPES::CAT_PEASANT_:
			enemies[i] = new CatPeasant(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::CAT_PEASANT_;
			break;

		case ENEMY_TYPES::IMP_:
			enemies[i] = new Imp(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::IMP_;
			break;
		
		case ENEMY_TYPES::PLANT_:
			enemies[i] = new Plant(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::PLANT_;
			break;

		case ENEMY_TYPES::MONKEY_:
			enemies[i] = new Monkey(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::MONKEY_;
			break;
		}
	}
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		//if (c1->type == COLLIDER_ITEM) { //GRENADES
			if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1) {

				enemies[i]->OnCollision(c1, c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			}
		//}
		
	}
}