#include "j1Module.h"

#include "j1Enemies.h"
#include "j1Render.h"

#include "Enemy.h"
#include "Imp.h"
#include "CatPeasant.h"
#include "Monkey.h"
#include "Plant.h"


#define SPAWN_MARGIN 50

j1Enemies::j1Enemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Start()
{
	return true;
}

bool j1Enemies::PreUpdate()
{
	// check camera position to decide what to spawn

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			/*if (App->fade->on == App->scene_1) {
				if (abs(queue[i].y * SCREEN_SIZE) < App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN)
				{
					SpawnEnemy(queue[i]);
					queue[i].type = ENEMY_TYPES::NO_TYPE;
					LOG("Spawning enemy at %d", queue[i].y * SCREEN_SIZE);
				}
			}
			else {
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].y * SCREEN_SIZE);
			}
			*/
		}
	}

	return true;
}

// Called before render is available
bool j1Enemies::Update()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Draw(nullptr /*App->TEX->sprites*/);

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
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	
	return true;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	//LOG("Freeing all enemies");

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
		case ENEMY_TYPES::IMP_:
			enemies[i] = new Imp(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::IMP_;
			break;
		
		case ENEMY_TYPES::CAT_PEASANT_:
			enemies[i] = new Imp(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::CAT_PEASANT_;
			break;
		
		case ENEMY_TYPES::MONKEY_:
			enemies[i] = new Imp(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::MONKEY_;
			break;
		
		case ENEMY_TYPES::PLANT_:
			enemies[i] = new Imp(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::PLANT_;
			break;
		}
	}
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		/*if (c1->type == COLLIDER_ITEM) { //GRENADES
			if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1) {

				enemies[i]->OnCollision(c1, c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			}
		}
		*/
		

	}

}