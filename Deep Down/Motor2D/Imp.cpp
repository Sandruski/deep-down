#include "j1App.h"
#include "j1Enemies.h"
#include "Imp.h"

#include "j1Collision.h"
#include "j1Player.h"
#include "j1Particles.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

#include "j1Input.h"

#include "SDL/include/SDL_timer.h"

Imp::Imp(int x, int y, PathInfo* path) : Enemy(x, y, path)
{
	imp = new ImpInfo(App->enemies->GetImpInfo());

	///
	animation = &imp->r_shield_walk;
	impState = ImpState::r_shield_walk;

	collider = App->collision->AddCollider({ 0, 0, imp->coll_size.x + imp->coll_offset.w, imp->coll_size.y + imp->coll_offset.h }, COLLIDER_TYPE::COLLIDER_IMP, App->enemies);

	// Needs organization
	speed = { 0, 1 };

	// Create normal path (idle path)

}

void Imp::Move()
{
	// Lab
	/*
	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT) {

		position.x--;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT) {

		position.x++;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT) {

		position.y--;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT) {

		position.y++;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_REPEAT) {

		impState = r_throw_bomb;

	}
	*/
	//

	UpdatePath();

	//UpdatePathfinding();

	GeneralStatesMachine();

	// Update movement
	up = false;
	down = false;
	left = false;
	right = false;
	stop_x = false;
	stop_y = false;
	throw_bomb = false;
	UpdateDirection();

	// Update collider
	collider_pos = { position.x + imp->coll_offset.x, position.y + imp->coll_offset.y };
	collider->SetPos(collider_pos.x, collider_pos.y);

	//position.x = App->player->position.x;
	//position.y = App->player->position.y;
	//position.y += speed.y;

}

void Imp::GeneralStatesMachine() {

	switch (impState) {

	case r_shield_idle:
		if (throw_bomb) {
			App->particles->AddParticle(App->particles->Imp_r_bomb, position.x, position.y, COLLIDER_IMP_BOMB, NULL, { 0,0 });
			impState = ImpState::r_throw_bomb;
		}
		if (stop_x && left) {
			impState = ImpState::l_shield_idle;
			break;
		}
		animation = &imp->r_shield_idle;
		break;

	case l_shield_idle:
		if (throw_bomb) {
			App->particles->AddParticle(App->particles->Imp_l_bomb, position.x, position.y, COLLIDER_IMP_BOMB, NULL, { 0,0 });
			impState = ImpState::l_throw_bomb;
		}
		if (stop_x && right) {
			impState = ImpState::r_shield_idle;
			break;
		}
		animation = &imp->l_shield_idle;
		break;

	case r_shield_walk:
		if (left) {
			impState = ImpState::l_shield_walk;
			break;
		}
		animation = &imp->r_shield_walk;
		impState = ImpState::r_shield_idle;
		break;

	case l_shield_walk:
		if (right) {
			impState = ImpState::r_shield_walk;
			break;
		}
		animation = &imp->l_shield_walk;
		impState = ImpState::l_shield_idle;
		break;

	case r_jump:
		if (left) {
			impState = ImpState::l_jump;
			break;
		}
		animation = &imp->r_jump;
		impState = ImpState::r_shield_idle;
		break;

	case l_jump:
		if (right) {
			impState = ImpState::r_jump;
			break;
		}
		animation = &imp->l_jump;
		impState = ImpState::l_shield_idle;
		break;

	case r_throw_bomb:
		if (left) {
			impState = ImpState::l_throw_bomb;
			break;
		}
		animation = &imp->r_throw_bomb;
		impState = ImpState::r_shield_idle;
		break;

	case l_throw_bomb:
		if (right) {
			impState = ImpState::r_throw_bomb;
			break;
		}
		animation = &imp->l_throw_bomb;
		impState = ImpState::l_shield_idle;
		break;

	case r_shield_hurt:
		if (left) {
			impState = ImpState::l_shield_hurt;
			break;
		}
		animation = &imp->r_shield_hurt;
		impState = ImpState::r_shield_idle;
		break;

	case l_shield_hurt:
		if (right) {
			impState = ImpState::r_shield_hurt;
			break;
		}
		animation = &imp->l_shield_hurt;
		impState = ImpState::l_shield_idle;
		break;
	}
}

void Imp::UpdateDirection() {

	if (position.x == last_pos.x) {
		if (position.x < last_pos.x)
			left = true;
		else if (position.x > last_pos.x)
			right = true;
	}
	else
		stop_x = true;

	if (position.y == last_pos.y) {
		if (position.y < last_pos.y)
			up = true;
		else if (position.y > last_pos.y)
			down = true;
	}
	else
		stop_y = true;

	last_pos = position;
}

void Imp::OnCollision(Collider* c1, Collider* c2) 
{
}

void Imp::UpdatePath() 
{
	if (path_info->path != nullptr) {

		iPoint to_go = path_info->path[index];

		if (position.x < to_go.x)
			position.x++;
		else if (position.x > to_go.x)
			position.x--;
		if (position.y < to_go.y)
			position.y++;
		else if (position.y > to_go.y)
			position.y--;

		if (position == to_go)
			index++;
	}
	else
		repeat_path = true;

	if (repeat_path) {
		index = 0;
	}

}

void Imp::UpdatePathfinding() 
{
	if (create_path) {
		App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->player->position.x, App->player->position.y));
		last_path = *App->pathfinding->GetLastPath();
		create_path = false;
	}

	if (last_path.At(index) != nullptr) {

		iPoint to_go = App->map->MapToWorld(last_path.At(index)->x, last_path.At(index)->y);

		if (position.x < to_go.x)
			position.x++;
		else if (position.x > to_go.x)
			position.x--;
		if (position.y < to_go.y)
			position.y++;
		else if (position.y > to_go.y)
			position.y--;

		if (position == to_go)
			index++;
	}
	else
		path_finished = true;

	SDL_Rect enemy_pos = { position.x - 50, position.y, 100, 100 };
	SDL_Rect player_pos = { App->player->position.x - 50, App->player->position.y - 10, 100, 200 };

	// If player is near the enemy... Create path
	if (path_finished && SDL_HasIntersection(&enemy_pos, &player_pos) && App->input->GetKey(SDL_SCANCODE_0) == KEY_REPEAT) {
		create_path = true;
		path_finished = false;
		index = 0;
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

ImpInfo::ImpInfo() {}

ImpInfo::ImpInfo(const ImpInfo& i) :
	r_shield_idle(i.r_shield_idle), l_shield_idle(i.l_shield_idle),
	r_shield_hurt(i.r_shield_hurt), l_shield_hurt(i.l_shield_hurt),
	r_jump(i.r_jump), l_jump(i.l_jump),
	r_throw_bomb(i.r_throw_bomb), l_throw_bomb(i.l_throw_bomb),
	r_shield_walk(i.r_shield_walk), l_shield_walk(i.l_shield_walk),
	coll_size(i.coll_size), coll_offset(i.coll_offset),
	path1(i.path1)
{}

ImpInfo::~ImpInfo() {

// Delete dyn array
}