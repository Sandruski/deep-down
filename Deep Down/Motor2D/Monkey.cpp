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

Monkey::Monkey(float x, float y, PathInfo* path) : Enemy(x, y, path)
{
	monkey = App->enemies->GetMonkeyInfo();

	///
	animation = &monkey.r_idle;
	monkeyState = MonkeyState::r_idle;

	collider = App->collision->AddCollider({ 0, 0, monkey.coll_size.x + monkey.coll_offset.w, monkey.coll_size.y + monkey.coll_offset.h }, COLLIDER_TYPE::COLLIDER_MONKEY, App->enemies);
	collider_size = monkey.coll_size;

	follow_pathfinding1 = App->collision->AddCollider({ i_pos.x - 50, i_pos.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);
	follow_pathfinding2 = App->collision->AddCollider({ (int)App->player->position.x - 50, (int)App->player->position.y - 10, 100, 200 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);

	speed = { 0.8f,0 };
}

void Monkey::Move(float dt)
{
	i_pos.x = (int)position.x;
	i_pos.y = (int)position.y;

	// Update path/pathfinding
	if (!pathfinding)
		UpdatePath();

	UpdatePathfinding();

	// Update state
	GeneralStatesMachine();

	// Update movement
	UpdateDirection();

	// Update collider
	collider_pos = { i_pos.x + monkey.coll_offset.x, i_pos.y + monkey.coll_offset.y };
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
		animation = &monkey.r_idle;
		break;

	case l_idle:
		if (right) {
			monkeyState = MonkeyState::r_idle;
			break;
		}
		animation = &monkey.l_idle;
		break;

	case r_hit:
		if (left) {
			monkeyState = MonkeyState::l_hit;
			break;
		}
		animation = &monkey.r_hit;
		monkeyState = MonkeyState::r_idle;
		break;

	case l_hit:
		if (right) {
			monkeyState = MonkeyState::r_hit;
			break;
		}
		animation = &monkey.l_hit;
		monkeyState = MonkeyState::l_idle;
		break;

	case r_hurt:
		if (left) {
			monkeyState = MonkeyState::l_hurt;
			break;
		}
		animation = &monkey.r_hurt;
		monkeyState = MonkeyState::r_idle;
		break;

	case l_hurt:
		if (right) {
			monkeyState = MonkeyState::r_hurt;
			break;
		}
		animation = &monkey.l_hurt;
		monkeyState = MonkeyState::l_idle;
		break;
	}
}

void Monkey::UpdateDirection() 
{
	if (i_pos.x == last_pos.x) {
		if (i_pos.x < last_pos.x) {
			left = true;
			right = false;
		}
		else if (i_pos.x > last_pos.x) {
			right = true;
			left = false;
		}
	}

	if (i_pos.y == last_pos.y) {
		if (i_pos.y < last_pos.y) {
			up = true;
			down = false;
		}
		else if (i_pos.y > last_pos.y) {
			down = true;
			up = false;
		}
	}

	last_pos.x = (int)position.x;
	last_pos.y = (int)position.y;
}

void Monkey::OnCollision(Collider* c1, Collider* c2) 
{
}

void Monkey::UpdatePath()
{
	if (path_info->path != nullptr) {

		// If enemy was pathfinding towards the player and doesn't see it anymore, create a pathfinding back home
		if (pathfinding_stop) {
			if (ResetPathfindingVariables()) {
				normal_path_index = last_normal_path_index;
				create_pathfinding_back = true;
			}
		}

		// Create pathfinding back
		if (create_pathfinding_back) {
			home = path_info->path[normal_path_index];

			if (CreatePathfinding(home)) {
				going_back_home = true;
				pathfinding_finished = true;
				create_pathfinding_back = false;
			}
		}

		// If it has to go back home and hasn't reached home yet, update the pathfinding back
		if (going_back_home) {
			if (!Pathfind())
				going_back_home = false;
		}
		// If it is home, update the normal path
		else {
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

	// If player is near the enemy... Create a pathfinding towards it
	if (pathfinding_finished && SDL_HasIntersection(&enemy_pos, &player_pos)) {
		if (ResetPathfindingVariables()) {
			last_normal_path_index = normal_path_index;
			create_pathfinding = true;
		}
	}

	// Create pathfinding
	if (create_pathfinding) {
		iPoint dest;
		dest.x = (int)App->player->position.x;
		dest.y = (int)App->player->position.y;

		if (CreatePathfinding(dest)) {
			pathfinding = true;
			create_pathfinding = false;
		}
	}

	if (pathfinding) {
		// If player is near the enemy and the enemy hasn't reached the end of the path yet, update the path
		if (SDL_HasIntersection(&enemy_pos, &player_pos) && pathfind) {
			if (!Pathfind())
				pathfind = false;
		}
		else {
			// If the enemy reaches the end of the path and doesn't see the player anymore, stop pathfinding and go back home
			if (!SDL_HasIntersection(&enemy_pos, &player_pos)) {
				pathfinding = false;
				pathfinding_stop = true;
			}
			// If the enemy reaches the end of the path and continues seeing the player, create a new pathfinding towards him
			else {
				pathfinding = false;
				pathfinding_finished = true;
			}
		}
	}
}

void Monkey::UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player)
{
	follow_pathfinding1->SetPos(i_pos.x - 30, i_pos.y - 30);
	follow_pathfinding2->SetPos((int)App->player->position.x - 25, (int)App->player->position.y - 50);

	enemy = { i_pos.x - 30, i_pos.y - 30, 100, 100 };
	player = { (int)App->player->position.x - 25, (int)App->player->position.y - 50, 100, 200 };
}

bool Monkey::ResetPathfindingVariables()
{
	bool ret = true;

	pathfinding_finished = false;
	pathfinding_stop = false;
	pathfinding = false;
	pathfind = true;
	pathfinding_size = 0;

	return ret;
}

bool Monkey::CreatePathfinding(iPoint destination)
{
	bool ret = false;

	if (App->pathfinding->CreatePath(App->map->WorldToMap(i_pos.x, i_pos.y), App->map->WorldToMap(destination.x, destination.y), Distance::DISTANCE_TO)) {
		last_pathfinding = App->pathfinding->GetLastPath();

		pathfinding_size = last_pathfinding->Count();

		pathfinding_index = 1;

		mlast_pathfinding.Clear();

		for (int i = 0; i < pathfinding_size; ++i) {
			mlast_pathfinding.PushBack(*last_pathfinding->At(i));
			ret = true;
		}
	}

	return ret;
}

void Monkey::UpdateMovement(iPoint to_go)
{
	if (i_pos.x < to_go.x)
		position.x += speed.x;
	else if (i_pos.x > to_go.x)
		position.x -= speed.x;
	if (i_pos.y < to_go.y)
		position.y += speed.x;
	else if (i_pos.y > to_go.y)
		position.y -= speed.x;
}

bool Monkey::Pathfind()
{
	bool ret = true;

	if (pathfinding_size > 1) {
		iPoint to_go = App->map->MapToWorld(mlast_pathfinding[pathfinding_index].x, mlast_pathfinding[pathfinding_index].y);

		UpdateMovement(to_go);

		if (i_pos == to_go) {
			if (pathfinding_index < pathfinding_size - 1)
				pathfinding_index++;
		}

		if (i_pos == App->map->MapToWorld(mlast_pathfinding[pathfinding_size - 1].x, mlast_pathfinding[pathfinding_size - 1].y))
			ret = false;
	}
	else
		ret = false;

	return ret;
}

bool Monkey::DoNormalPath()
{
	bool ret = true;

	iPoint to_go = path_info->path[normal_path_index];

	if (to_go.x != NULL && to_go.y != NULL) {

		UpdateMovement(to_go);

		ret = true;

		if (i_pos == to_go) {
			if (normal_path_index < path_info->path_size - 1)
				normal_path_index++;
			else
				ret = false;
		}
	}

	return ret;
}

void Monkey::RecalculatePath()
{
	normal_path_index = 0;

	// Flip path
	FlipPath(path_info);
}

void Monkey::FlipPath(PathInfo* path_info) {
	iPoint* start = &path_info->path[0];
	iPoint* end = &path_info->path[path_info->path_size - 1];

	while (start < end)
		SWAP(*start++, *end--);
}