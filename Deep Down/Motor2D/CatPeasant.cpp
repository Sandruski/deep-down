#include "j1App.h"
#include "CatPeasant.h"
#include "j1Collision.h"
#include "j1Player.h"

#include "SDL/include/SDL_timer.h"

CatPeasant::CatPeasant(int x, int y) : Enemy(x, y)
{
	idle.PushBack({ 2, 21, 64, 64 });
	idle.PushBack({ 68, 21, 64, 64 });
	idle.PushBack({ 134, 21, 64, 64 });
	idle.PushBack({ 200, 21, 64, 64 });
	idle.PushBack({ 266, 21, 64, 64 });
	idle.speed = 0.25f;

}

void CatPeasant::Move()
{
	position.x = App->player->position.x;
	position.y = App->player->position.y;
	animation = &idle;
}

void CatPeasant::OnCollision(Collider* c1, Collider* c2) {


}