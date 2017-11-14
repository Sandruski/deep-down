#include "j1App.h"
#include "j1Enemies.h"
#include "Monkey.h"

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Collision.h"
#include "j1Player.h"
#include "j1Particles.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

#include "j1Input.h"

#include "SDL/include/SDL_timer.h"

Monkey::Monkey(int x, int y, PathInfo* path) : Enemy(x, y, path)
{
	monkey = new MonkeyInfo(App->enemies->GetMonkeyInfo());

	///
	animation = &monkey->r_idle;
	monkeyState = MonkeyState::r_idle;

	collider = App->collision->AddCollider({ 0, 0, monkey->coll_size.x + monkey->coll_offset.w, monkey->coll_size.y + monkey->coll_offset.h }, COLLIDER_TYPE::COLLIDER_MONKEY, App->enemies);

	follow_pathfinding1 = App->collision->AddCollider({ position.x - 50, position.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);
	follow_pathfinding2 = App->collision->AddCollider({ (int)App->player->position.x - 50, (int)App->player->position.y - 10, 100, 200 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);
}

void Monkey::Move(uint index)
{
	enemy_index = index;

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
	UpdateDirection();

	// Update collider
	collider_pos = { position.x + monkey->coll_offset.x, position.y + monkey->coll_offset.y };
	collider->SetPos(collider_pos.x, collider_pos.y);
}

void Monkey::GeneralStatesMachine() 
{
	switch (monkeyState) {

	case r_idle:
		if (left) {
			monkeyState = MonkeyState::l_idle;
			break;
		}
		animation = &monkey->r_idle;
		break;

	case l_idle:
		if (right) {
			monkeyState = MonkeyState::r_idle;
			break;
		}
		animation = &monkey->l_idle;
		break;

	case r_hit:
		if (left) {
			monkeyState = MonkeyState::l_hit;
			break;
		}
		animation = &monkey->r_hit;
		monkeyState = MonkeyState::r_idle;
		break;

	case l_hit:
		if (right) {
			monkeyState = MonkeyState::r_hit;
			break;
		}
		animation = &monkey->l_hit;
		monkeyState = MonkeyState::l_idle;
		break;

	case r_hurt:
		if (left) {
			monkeyState = MonkeyState::l_hurt;
			break;
		}
		animation = &monkey->r_hurt;
		monkeyState = MonkeyState::r_idle;
		break;

	case l_hurt:
		if (right) {
			monkeyState = MonkeyState::r_hurt;
			break;
		}
		animation = &monkey->l_hurt;
		monkeyState = MonkeyState::l_idle;
		break;
	}
}

void Monkey::UpdateDirection() 
{
	if (position.x == last_pos.x) {
		if (position.x < last_pos.x)
			left = true;
		else if (position.x > last_pos.x)
			right = true;
	}

	if (position.y == last_pos.y) {
		if (position.y < last_pos.y)
			up = true;
		else if (position.y > last_pos.y)
			down = true;
	}

	last_pos = position;
}

void Monkey::OnCollision(Collider* c1, Collider* c2) 
{
}

void Monkey::UpdatePath()
{
	if (path_info->start_pos.x != 0 && path_info->start_pos.y != 0
		&& path_info->end_pos.x != 0 && path_info->end_pos.x != 0) {

		if (pathfinding_stop) {
			//Create new path towards start or end pos
			if (ResetPathfindingBackVariables())
				create_path = true;
		}

		if (create_path) {

			if (goal_is_start)
				goal = path_info->start_pos;
			else
				goal = path_info->end_pos;

			if (CreatePathfinding(goal))
				do_path = true;
		}

		// If player is near the enemy and the enemy hasn't reached the end of the path yet, update the path
		if (last_pathfinding != nullptr && do_path) {
			//LOG("Pathfinding");
			Pathfind();
		}
		else {
			do_path = false;
			//LOG("Update path");
			//LOG("Normal path index: %d", normal_path_index);
			if (!DoNormalPath())
				RecalculatePath();
		}
	}
}

void Monkey::UpdatePathfinding()
{
	SDL_Rect enemy_pos;
	SDL_Rect player_pos;
	UpdatePathfindingAffectArea(enemy_pos, player_pos);

	// If player is near the enemy... Set create path to true
	if (pathfinding_finished && SDL_HasIntersection(&enemy_pos, &player_pos)) {
		//LOG("Reset pathfinding variables");
		if (ResetPathfindingVariables())
			create_pathfinding = true;
	}

	/*
	// Create path
	if (create_pathfinding) {
		//LOG("Create new pathfinding");
		if (CreatePathfinding())
			pathfinding = true;
	}
	*/

	// If player is near the enemy and the enemy hasn't reached the end of the path yet, update the path
	if (last_pathfinding != nullptr && last_pathfinding->At(pathfinding_index) != nullptr && SDL_HasIntersection(&enemy_pos, &player_pos) && pathfinding) {
		//LOG("Pathfinding");
		Pathfind();
	}
	else {
		// If when enemy reaches the end of the path, it doesn't find the player -> Stop pathfinding and go back home
		if (!SDL_HasIntersection(&enemy_pos, &player_pos) && pathfinding) {
			pathfinding = false;
			pathfinding_stop = true;
			pathfinding_finished = true;
			//pathfinding_finished = true;
			//LOG("Pathfinding stop");
		}
		// If when enemy reaches the end of the path, it finds the player -> Create a new pathfinding
		else if (pathfinding) {
			pathfinding = false;
			pathfinding_finished = true;
			//LOG("Pathfinding finished");
		}
	}
}

void Monkey::UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player)
{
	follow_pathfinding1->SetPos(position.x - 30, position.y - 30);
	follow_pathfinding2->SetPos(App->player->position.x - 25, App->player->position.y - 50);

	enemy = { position.x - 30, position.y - 30, 100, 100 };
	player = { (int)App->player->position.x - 25, (int)App->player->position.y - 50, 100, 200 };
}

bool Monkey::ResetPathfindingVariables()
{
	bool ret = true;

	pathfinding_finished = false;
	pathfinding_stop = false;
	pathfinding_index = 1;
	last_normal_path_index = normal_path_index;

	return ret;
}

bool Monkey::CreatePathfinding(iPoint end_pos)
{
	bool ret = false;

	if (App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->player->position.x, App->player->position.y), true, enemy_index))
		last_pathfinding = App->pathfinding->GetLastPath(enemy_index);

	if (last_pathfinding != nullptr)
		ret = true;

	create_pathfinding = false;

	return ret;
}

void Monkey::Pathfind()
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

bool Monkey::DoNormalPath()
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

void Monkey::RecalculatePath()
{
	normal_path_index = 0;
}

bool Monkey::ResetPathfindingBackVariables()
{
	bool ret = true;

	normal_path_index = 1;
	pathfinding_stop = false;
	repeat_normal_path = false;

	return ret;
}

/*
bool Monkey::CreatePathfindingBack()
{
	bool ret = false;

	if (normal_path_index > 0)
		normal_path_index--;
	else
		normal_path_index++;

	iPoint to_go = path_info->path[normal_path_index];

	if (App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(to_go.x, to_go.y), true, enemy_index))
		last_pathfinding = App->pathfinding->GetLastPath(enemy_index);

	if (last_pathfinding != nullptr) {
		pathfinding_index = 1;

		ret = true;
	}

	create_pathfinding_back = false;

	return ret;
}
*/

// -------------------------------------------------------------
// -------------------------------------------------------------

MonkeyInfo::MonkeyInfo() {}

MonkeyInfo::MonkeyInfo(const MonkeyInfo& i) :
	r_idle(i.r_idle), l_idle(i.l_idle),
	r_hurt(i.r_hurt), l_hurt(i.l_hurt),
	r_hit(i.r_hit), l_hit(i.l_hit),
	coll_size(i.coll_size), coll_offset(i.coll_offset)
{}

MonkeyInfo::~MonkeyInfo() {}