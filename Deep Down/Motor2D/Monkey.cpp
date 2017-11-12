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

void Monkey::Move()
{
	// Update path/pathfinding
	SDL_Rect enemy_pos;
	SDL_Rect player_pos;
	UpdatePathfindingAffectArea(enemy_pos, player_pos);

	if (SDL_HasIntersection(&enemy_pos, &player_pos)) {
		UpdatePathfinding();
	}
	else {
		UpdatePath();
	}

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
	if (path_info->end_pos.x != 0 && path_info->end_pos.y != 0
		&& path_info->start_pos.x != 0 && path_info->start_pos.y != 0) {

		if (create_path) {

			if (goal_is_end) {
				goal = path_info->end_pos;
			}
			else
				goal = path_info->start_pos;

			if (CreatePathfinding(goal)) {
				create_path = false;
				path = true;
			}
		}
	}

	if (path) {
		if (!Pathfind(path_index)) {
			// If the enemy reaches the end of the path, go back to its origin
			goal_is_end = !goal_is_end;
			create_path = true;
			path = false;
			pathfinding_index = 0;
		}
	}
}

void Monkey::UpdatePathfinding()
{
	if (pathfinding_finished) {
		if (ResetPathfindingVariables())
			create_pathfinding = true;
	}

	if (create_pathfinding) {
		iPoint player_pos;
		player_pos.x = (int)App->player->position.x;
		player_pos.y = (int)App->player->position.y;

		const p2DynArray<iPoint>* path = CreatePathfinding(player_pos);
	}

	if (last_pathfinding != nullptr && pathfinding) {
		Pathfind(pathfinding_index);
	}
	else {
		pathfinding = false;
		pathfinding_finished = true;
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
	pathfinding_index = 1;

	return ret;
}

const p2DynArray<iPoint>* Monkey::CreatePathfinding(iPoint end_pos)
{
	App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(end_pos.x, end_pos.y));
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	if (path != nullptr) {
		pathfinding = true;
		return path;
	}
	else
		return nullptr;
}

bool Monkey::Pathfind(uint& index)
{
	bool ret = true;

	iPoint to_go = App->map->MapToWorld(last_pathfinding->At(index)->x, last_pathfinding->At(index)->y);

	if (position.x < to_go.x)
		position.x++;
	else if (position.x > to_go.x)
		position.x--;
	if (position.y < to_go.y)
		position.y++;
	else if (position.y > to_go.y)
		position.y--;

	if (position == to_go) {
		if (index < DEFAULT_PATH_LENGTH - 1)
			index++;
	}

	if (position == goal)
		ret = false;

	return ret;
}

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