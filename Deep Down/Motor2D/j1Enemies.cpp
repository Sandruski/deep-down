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
#include "j1Collision.h"

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
	node = node.next_sibling("spritesheet");
	Player_spritesheet = node.attribute("name").as_string();
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

	// MONKEY
	collider_node = config.child("types").child("monkey").child("general").child("coll_offset");
	monkey.coll_offset = { collider_node.attribute("x").as_int(), collider_node.attribute("y").as_int(), collider_node.attribute("w").as_int(), collider_node.attribute("h").as_int() };

	// Load animations
	animations_node = config.child("types").child("monkey").child("animations");

	//r_idle
	node = animations_node.child("r_idle");
	monkey.r_idle.speed = node.attribute("speed").as_float();
	monkey.r_idle.loops = node.attribute("loops").as_bool();
	monkey.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		monkey.r_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_idle
	node = animations_node.child("l_idle");
	monkey.l_idle.speed = node.attribute("speed").as_float();
	monkey.l_idle.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		monkey.l_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_hurt
	node = animations_node.child("r_hurt");
	monkey.r_hurt.speed = node.attribute("speed").as_float();
	monkey.r_hurt.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		monkey.r_hurt.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_hurt
	node = animations_node.child("l_hurt");
	monkey.l_hurt.speed = node.attribute("speed").as_float();
	monkey.l_hurt.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		monkey.l_hurt.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_hit
	node = animations_node.child("r_hit");
	monkey.r_hit.speed = node.attribute("speed").as_float();
	monkey.r_hit.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		monkey.r_hit.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_hit
	node = animations_node.child("l_hit");
	monkey.l_hit.speed = node.attribute("speed").as_float();
	monkey.l_hit.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		monkey.l_hit.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}
	//_load_animations
	//_MONKEY

	//PLAYER
	collider_node = config.child("types").child("player").child("general");
	player.coll_offset = { collider_node.child("collider").attribute("x").as_int(), collider_node.child("collider").attribute("y").as_int(), collider_node.child("collider").attribute("w").as_int(), collider_node.child("collider").attribute("h").as_int() };
	player.gravity = collider_node.child("gravity").attribute("value").as_float();
	player.speed = { collider_node.child("speed").attribute("x").as_float(), collider_node.child("speed").attribute("y").as_float() };

	//idle
	node = animations_node.child("idle");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//idle2
	node = animations_node.child("idle2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//forward
	node = animations_node.child("forward");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//backward
	node = animations_node.child("backward");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouch
	node = animations_node.child("crouch");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouch2
	node = animations_node.child("crouch2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//shot
	node = animations_node.child("shot");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//shot2
	node = animations_node.child("shot2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouchShot
	node = animations_node.child("crouchShot");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouchShot2
	node = animations_node.child("crouchShot2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//jump
	node = animations_node.child("jump");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//jump2
	node = animations_node.child("jump2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//punished
	node = animations_node.child("punished");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//punished2
	node = animations_node.child("punished2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}


	//dash
	node = animations_node.child("dash");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//dash2
	node = animations_node.child("dash2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//firstAttack
	node = animations_node.child("firstAttack");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//firstAttack2
	node = animations_node.child("firstAttack2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//secondAttack
	node = animations_node.child("secondAttack");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//secondAttack2
	node = animations_node.child("secondAttack2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//thirdAttack
	node = animations_node.child("thirdAttack");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//thirdAttack2
	node = animations_node.child("thirdAttack2");
	imp.r_shield_idle.speed = node.attribute("speed").as_float();
	imp.r_shield_idle.loops = node.attribute("loops").as_bool();
	imp.coll_size = { node.child("frame").attribute("w").as_int(), node.child("frame").attribute("h").as_int() };
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		imp.r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}




	return ret;
}

bool j1Enemies::Start()
{
	// Load player textures
	LOG("Loading enemies textures");
	CatPeasantTex = App->tex->Load(CatPeasant_spritesheet.GetString());
	MonkeyPlantTex = App->tex->Load(MonkeyPlant_spritesheet.GetString());
	ImpTex = App->tex->Load(Imp_spritesheet.GetString());
	PlayerTex = App->tex->Load(Player_spritesheet.GetString());

	// Pathfinding collision data
	App->pathfinding->SetMap(App->map->data.width, App->map->data.height, (uchar*)App->map->collisionLayer->data);

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
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] != nullptr) {
			LOG("ENEMY[%d]: ", i);
			enemies[i]->Move(dt);
		}
	}

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) {
			if (enemies[i]->type == ENEMY_TYPES::CAT_PEASANT_)
				enemies[i]->Draw(CatPeasantTex);
			else if (enemies[i]->type == ENEMY_TYPES::IMP_)
				enemies[i]->Draw(ImpTex);
			else if (enemies[i]->type == ENEMY_TYPES::MONKEY_)
				enemies[i]->Draw(MonkeyPlantTex);
			else if (enemies[i]->type == ENEMY_TYPES::PLAYER_)
				enemies[i]->Draw(PlayerTex);
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

	// Remove all paths
	p2List_item<PathInfo*>* item;
	item = paths.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	paths.clear();

	App->tex->UnLoad(CatPeasantTex);
	App->tex->UnLoad(MonkeyPlantTex);
	App->tex->UnLoad(ImpTex);
	App->tex->UnLoad(PlayerTex);

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

	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, uint path)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].path = GetPathByIndex(path);
			queue[i].type = type;

			if (queue[i].path != nullptr) {
				queue[i].x = queue[i].path->start_pos.x;
				queue[i].y = queue[i].path->start_pos.y;
			}
			else {
				queue[i].x = 0;
				queue[i].y = 0;
			}

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
			enemies[i] = new CatPeasant(info.x, info.y, info.path);
			enemies[i]->type = ENEMY_TYPES::CAT_PEASANT_;
			break;

		case ENEMY_TYPES::IMP_:
			enemies[i] = new Imp(info.x, info.y, info.path);
			enemies[i]->type = ENEMY_TYPES::IMP_;
			break;
		
		case ENEMY_TYPES::PLANT_:
			enemies[i] = new Plant(info.x, info.y, info.path);
			enemies[i]->type = ENEMY_TYPES::PLANT_;
			break;

		case ENEMY_TYPES::MONKEY_:
			enemies[i] = new Monkey(info.x, info.y, info.path);
			enemies[i]->type = ENEMY_TYPES::MONKEY_;
			break;

		case ENEMY_TYPES::PLAYER_:
			playerData = new Player(info.x, info.y, info.path);
			enemies[i] = playerData;
			enemies[i]->type = ENEMY_TYPES::PLAYER_;

			break;
		}
	}
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (c1->type == COLLIDER_ARROW || c2->type == COLLIDER_ARROW) {
			if (enemies[i] != nullptr && (enemies[i]->GetCollider() == c1 || enemies[i]->GetCollider() == c2)) {

				enemies[i]->OnCollision(c1, c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			}
		}	
	}
}

bool j1Enemies::LoadPathsInfo()
{
	bool ret = false;

	uint index = 1;

	// Repetitive paths
	if (SaveRepetitivePaths(index))
		ret = true;

	// Start-end paths
	if (SaveStartEndPaths(index))
		ret = true;

	return ret;
}

bool j1Enemies::SaveRepetitivePaths(uint& index) 
{
	bool ret = false;

	p2SString tmp("%s%d", "Enemy", index);
	Object* obj = App->map->data.GetObjectByName("Enemies", tmp);

	while (obj != nullptr) {

		// Save actual path
		PathInfo* path = new PathInfo();

		path->start_pos = { (int)obj->x, (int)obj->y };

		path->path = new iPoint[MAX_POLYLINE_POINTS];
		memset(path->path, 0, sizeof(iPoint) * MAX_POLYLINE_POINTS);

		// Create path
		if (obj->polyline != nullptr) {

			path->path[0].x = obj->polyline[0] + path->start_pos.x;
			path->path[0].y = obj->polyline[1] + path->start_pos.y;

			path->path_size = 1;
			int i = 1;
			while (obj->polyline[i * 2 + 0] != 0 && obj->polyline[i * 2 + 1] != 0 && i < MAX_POLYLINE_POINTS) {

				path->path[i].x = obj->polyline[i * 2 + 0] + path->start_pos.x;
				path->path[i].y = obj->polyline[i * 2 + 1] + path->start_pos.y;

				path->path_size++;
				++i;
			}
		}

		paths.add(path);
		ret = true;

		// Search next path
		index++;
		p2SString tmp("%s%d", "Enemy", index);
		obj = App->map->data.GetObjectByName("Enemies", tmp);
	}

	if (obj != nullptr)
		RELEASE(obj);

	return ret;
}

bool j1Enemies::SaveStartEndPaths(uint& index) 
{
	bool ret = false;

	p2SString tmp("%s%d%s", "Enemy", index, "S");
	Object* obj = App->map->data.GetObjectByName("Enemies", tmp);

	while (obj != nullptr) {

		// Save actual path
		PathInfo* path = new PathInfo();

		path->start_pos = { (int)obj->x, (int)obj->y };

		p2SString tmp("%s%d%s", "Enemy", index, "E");
		obj = App->map->data.GetObjectByName("Enemies", tmp);

		if (obj != nullptr) {
			path->end_pos = { (int)obj->x, (int)obj->y };

			paths.add(path);
			ret = true;
		}

		// Search next path
		index++;
		p2SString tmp1("%s%d%s", "Enemy", index, "S");
		obj = App->map->data.GetObjectByName("Enemies", tmp1);
	}

	if (obj != nullptr)
		RELEASE(obj);

	return ret;
}

bool j1Enemies::AddEnemies()
{
	bool ret = false;

	int index = 1;
	p2SString tmp("%s%d", "Enemy", index);
	Object* obj = App->map->data.GetObjectByName("Enemies", tmp);

	while (obj != nullptr) {
		// Add enemy
		AddEnemy((ENEMY_TYPES)obj->type, index);
		ret = true;

		// Search next enemy
		index++;
		p2SString tmp("%s%d", "Enemy", index);
		obj = App->map->data.GetObjectByName("Enemies", tmp);
	}

	p2SString tmp1("%s%d%s", "Enemy", index, "S");
	obj = App->map->data.GetObjectByName("Enemies", tmp1);

	while (obj != nullptr) {
		// Add enemy
		AddEnemy((ENEMY_TYPES)obj->type, index);
		ret = true;

		// Search next enemy
		index++;
		p2SString tmp1("%s%d%s", "Enemy", index, "S");
		obj = App->map->data.GetObjectByName("Enemies", tmp1);
	}

	AddEnemy(PLAYER_, 0);

	if (obj != nullptr)
		RELEASE(obj);

	return ret;
}

PathInfo* j1Enemies::GetPathByIndex(uint index) const {

	PathInfo* path = nullptr;
	p2List_item<PathInfo*>* iterator = paths.start;

	int i = 0;
	while (iterator != nullptr && i <= index - 1) {

		if (i == index - 1)
			path = iterator->data;

		iterator = iterator->next;
		++i;
	}

	return path;
}

// -------------------------------------------------------------
// -------------------------------------------------------------

PathInfo::PathInfo() {}

PathInfo::PathInfo(const PathInfo& i) :
	start_pos(i.start_pos), path(i.path), path_size(i.path_size)
{}