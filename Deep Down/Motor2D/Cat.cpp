#include "j1App.h"
#include "j1EntityFactory.h"
#include "Cat.h"

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Collision.h"
#include "j1Particles.h"

#include "j1Audio.h"

#include "SDL/include/SDL_timer.h"

Cat::Cat(float x, float y, PathInfo* path) : Entity(x, y, path)
{
	cat = App->entities->GetCatInfo();

	///
	animation = &cat.r_idle;

	collider = App->collision->AddCollider({ 0, 0, cat.coll_size.x + cat.coll_offset.w, cat.coll_size.y + cat.coll_offset.h }, COLLIDER_TYPE::COLLIDER_MONKEY, App->entities);
	collider_size = cat.coll_size;
}

void Cat::Move(const float dt)
{
	deltaTime = dt;

	i_pos.x = (int)position.x;
	i_pos.y = (int)position.y;

	// Update animations speed
	UpdateAnimations(dt);

	// Update collider
	collider_pos = { i_pos.x + cat.coll_offset.x, i_pos.y + cat.coll_offset.y };
	collider->SetPos(collider_pos.x, collider_pos.y);
}

void Cat::UpdateAnimations(const float dt)
{
	float speed = 10.0f;

	cat.r_idle.speed = speed * dt;
	cat.l_idle.speed = speed * dt;
	cat.r_going_ZZZ.speed = speed * dt;
	cat.l_going_ZZZ.speed = speed * dt;
	cat.r_waking_up.speed = speed * dt;
	cat.l_waking_up.speed = speed * dt;
	cat.r_to_crouch.speed = speed * dt;
	cat.l_to_crouch.speed = speed * dt;
	cat.r_crouch.speed = speed * dt;
	cat.l_crouch.speed = speed * dt;
	cat.r_rise.speed = speed * dt;
	cat.l_rise.speed = speed * dt;
	cat.r_jump.speed = speed * dt;
	cat.l_jump.speed = speed * dt;
	cat.r_fall.speed = speed * dt;
	cat.l_fall.speed = speed * dt;
	cat.r_land_soft.speed = speed * dt;
	cat.l_land_soft.speed = speed * dt;
	cat.r_roll.speed = speed * dt;
	cat.l_roll.speed = speed * dt;
	cat.r_to_run.speed = speed * dt;
	cat.l_to_run.speed = speed * dt;
	cat.r_run.speed = speed * dt;
	cat.l_run.speed = speed * dt;
	cat.r_turn.speed = speed * dt;
	cat.l_turn.speed = speed * dt;
	cat.r_brake.speed = speed * dt;
	cat.l_brake.speed = speed * dt;
	cat.attack.speed = speed * dt;
}

void Cat::OnCollision(Collider* c1, Collider* c2)
{
	
}