#include "j1App.h"
#include "j1Enemies.h"
#include "Plant.h"

#include "j1Collision.h"
#include "j1Player.h"

#include "SDL/include/SDL_timer.h"

Plant::Plant(int x, int y) : Enemy(x, y)
{
	idle.PushBack({ 2, 21, 64, 64 });
	idle.PushBack({ 68, 21, 64, 64 });
	idle.PushBack({ 134, 21, 64, 64 });
	idle.PushBack({ 200, 21, 64, 64 });
	idle.PushBack({ 266, 21, 64, 64 });
	idle.speed = 0.25f;

	animation = &idle;
	collider = App->collision->AddCollider({ 0, 0, 18, 18 }, COLLIDER_TYPE::COLLIDER_CATPEASANT, App->enemies);
}

void Plant::Move()
{
	// use 'start_pos' to know the start position of the enemy
	// update their position by using 'position'

	position.x = App->player->position.x;
	position.y = start_pos.y;

}

void Plant::OnCollision(Collider* c1, Collider* c2) {


}