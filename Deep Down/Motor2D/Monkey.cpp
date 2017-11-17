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
	monkey = new MonkeyInfo(App->enemies->GetMonkeyInfo());

	///
	animation = &monkey->r_idle;
	monkeyState = MonkeyState::r_idle;

	collider = App->collision->AddCollider({ 0, 0, monkey->coll_size.x + monkey->coll_offset.w, monkey->coll_size.y + monkey->coll_offset.h }, COLLIDER_TYPE::COLLIDER_MONKEY, App->enemies);

	follow_pathfinding1 = App->collision->AddCollider({ i_pos.x - 50, i_pos.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);
	follow_pathfinding2 = App->collision->AddCollider({ (int)App->player->position.x - 50, (int)App->player->position.y - 10, 100, 200 }, COLLIDER_TYPE::COLLIDER_NONE, App->enemies);
}

void Monkey::Move()
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
	up = false;
	down = false;
	left = false;
	right = false;
	UpdateDirection();

	// Update collider
	collider_pos = { i_pos.x + monkey->coll_offset.x, i_pos.y + monkey->coll_offset.y };
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
	if (i_pos.x == last_pos.x) {
		if (i_pos.x < last_pos.x)
			left = true;
		else if (i_pos.x > last_pos.x)
			right = true;
	}

	if (i_pos.y == last_pos.y) {
		if (i_pos.y < last_pos.y)
			up = true;
		else if (i_pos.y > last_pos.y)
			down = true;
	}


	last_pos.x = (int)position.x;
	last_pos.y = (int)position.y;
}

void Monkey::OnCollision(Collider* c1, Collider* c2) 
{
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