#include "j1App.h"
#include "j1Enemies.h"
#include "Imp.h"

#include "j1Collision.h"
#include "j1Player.h"

#include "SDL/include/SDL_timer.h"

Imp::Imp(int x, int y) : Enemy(x, y)
<<<<<<< HEAD
{
	imp = App->enemies->GetImpInfo();
=======
{/*
	// Load animations
	pugi::xml_node animations_node = config.child("types").child("cat").child("animations");
	pugi::xml_node node;
>>>>>>> 58796027d7212c9bc0cc282d230c818d2ad7312f

	animation = &imp.r_shield_idle;
	collider = App->collision->AddCollider({ 0, 0, 18, 18 }, COLLIDER_TYPE::COLLIDER_IMP, App->enemies);
	*/
}

void Imp::Move()
{
	position.x = App->player->position.x;
	position.y = App->player->position.y;
}

void Imp::OnCollision(Collider* c1, Collider* c2) 
{


}