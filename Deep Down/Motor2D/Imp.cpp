#include "j1App.h"
#include "j1Enemies.h"
#include "Imp.h"

#include "p2Defs.h"
#include "p2Log.h"

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

	follow_pathfinding1 = App->collision->AddCollider({ position.x - 50, position.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);
	follow_pathfinding2 = App->collision->AddCollider({ (int)App->player->position.x - 50, (int)App->player->position.y - 10, 100, 200 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);

	// Needs organization
	speed = { 0, 1 };
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

	// Update path/pathfinding
	if (!pathfinding)
		UpdatePath();

	UpdatePathfinding();

	// Update state
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

		if (pathfinding_stop) {
			LOG("Reset pathfinding back variables");
			if (ResetPathfindingBackVariables())
				create_pathfinding_back = true;
		}

		// Create pathfinding back
		if (create_pathfinding_back) {
			LOG("Create new pathfinding back");
			if (CreatePathfindingBack())
				going_back_home = true;
		}

		// If player is near the enemy and the enemy hasn't reached the end of the path yet, update the path
		if (last_pathfinding != nullptr && last_pathfinding->At(pathfinding_index) != nullptr && going_back_home) {
			LOG("Pathfinding");
			Pathfind();
		}
		else {
			going_back_home = false;

			LOG("Update path");
			LOG("Normal path index: %d", normal_path_index);
			if (!DoNormalPath())
				RecalculatePath();
		}
	}
}

void Imp::UpdatePathfinding()
{
	SDL_Rect enemy_pos;
	SDL_Rect player_pos;
	UpdatePathfindingAffectArea(enemy_pos, player_pos);

	// If player is near the enemy... Set create path to true
	if (pathfinding_finished && SDL_HasIntersection(&enemy_pos, &player_pos)) {
		LOG("Reset pathfinding variables");
		if (ResetPathfindingVariables())
			create_pathfinding = true;
	}

	// Create path
	if (create_pathfinding) {
		LOG("Create new pathfinding");
		if (CreatePathfinding())
			pathfinding = true;
	}

	// If player is near the enemy and the enemy hasn't reached the end of the path yet, update the path
	if (last_pathfinding != nullptr && last_pathfinding->At(pathfinding_index) != nullptr && pathfinding) {
		LOG("Pathfinding");
		Pathfind();
	}
	else {
		// If when enemy reaches the end of the path, it doesn't find the player -> Stop pathfinding and go back home
		if (!SDL_HasIntersection(&enemy_pos, &player_pos) && pathfinding) {
			pathfinding = false;
			pathfinding_stop = true;
			pathfinding_finished = true;
			LOG("Pathfinding stop");
		}
		// If when enemy reaches the end of the path, it finds the player -> Create a new pathfinding
		else {
			pathfinding = false;
			pathfinding_finished = true;
			LOG("Pathfinding finished");
		}
	}
}

void Imp::UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player)
{
	follow_pathfinding1->SetPos(position.x - 30, position.y - 30);
	follow_pathfinding2->SetPos(App->player->position.x - 25, App->player->position.y - 50);

	enemy = { position.x - 30, position.y - 30, 100, 100 };
	player = { (int)App->player->position.x - 25, (int)App->player->position.y - 50, 100, 200 };
}

bool Imp::ResetPathfindingVariables() 
{
	bool ret = true;

	pathfinding_finished = false;
	pathfinding_stop = false;
	pathfinding_index = 1;
	last_normal_path_index = normal_path_index;

	return ret;
}

bool Imp::CreatePathfinding() 
{
	bool ret = false;

	App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->player->position.x, App->player->position.y));
	last_pathfinding = App->pathfinding->GetLastPath();

	if (last_pathfinding != nullptr)
		ret = true;

	create_pathfinding = false;

	return ret;
}

void Imp::Pathfind() 
{
	iPoint to_go = App->map->MapToWorld(last_pathfinding->At(pathfinding_index)->x, last_pathfinding->At(pathfinding_index)->y);

	if (position.x < to_go.x)
		position.x++;
	else if (position.x > to_go.x)
		position.x--;
	if (position.y < to_go.y)
		position.y++;
	else if (position.y > to_go.y)
		position.y--;

	if (position == to_go) {
		if (pathfinding_index < DEFAULT_PATH_LENGTH)
			pathfinding_index++;
	}
}

bool Imp::DoNormalPath() 
{
	bool ret = false;

	iPoint to_go = path_info->path[normal_path_index];

	if (to_go.x != 0 && to_go.y != 0) {
		if (position.x < to_go.x)
			position.x++;
		else if (position.x > to_go.x)
			position.x--;
		if (position.y < to_go.y)
			position.y++;
		else if (position.y > to_go.y)
			position.y--;

		if (position == to_go) {
			if (normal_path_index < path_info->path_size)
				normal_path_index++;
		}

		ret = true;
	}

	return ret;
}

void Imp::RecalculatePath()
{
	normal_path_index = 0;

	// Flip path
	LOG("Flip path");
	FlipPath(path_info);
}

bool Imp::ResetPathfindingBackVariables() 
{
	bool ret = true;

	normal_path_index = last_normal_path_index;
	pathfinding_stop = false;
	repeat_normal_path = false;

	return ret;
}

bool Imp::CreatePathfindingBack() 
{
	bool ret = false;

	iPoint to_go = path_info->path[normal_path_index];
	App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(to_go.x, to_go.y));
	last_pathfinding = App->pathfinding->GetLastPath();

	if (last_pathfinding != nullptr) {
		pathfinding_index = 1;

		if (normal_path_index > 0)
			normal_path_index--;
		else
			normal_path_index++;

		ret = true;
	}

	create_pathfinding_back = false;

	return ret;
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

void Imp::FlipPath(PathInfo* path_info) {
	iPoint* start = &path_info->path[0];
	iPoint* end = &path_info->path[path_info->path_size - 1];

	while (start < end)
		SWAP(*start++, *end--);
}