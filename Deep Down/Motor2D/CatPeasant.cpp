#include "j1App.h"
#include "j1EntityFactory.h"
#include "CatPeasant.h"

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Input.h"
#include "j1Particles.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

#include "SDL/include/SDL_timer.h"

/*
CAT PEASANT (enemy):
· Type: air.
· Lives: 2.

- Follows a continuous path A-B (set in Tiled) using pathfinding.
- If she sees the player, she throws her magic cane towards him.
*/

CatPeasant::CatPeasant(float x, float y, PathInfo* path) : Entity(x, y, path)
{
	catPeasant = App->entities->GetCatPeasantInfo();

	///
	animation = &catPeasant.r_idle;
	catPeasantState = CatPeasantState::r_idle;

	lives = 2;

	collider = App->collision->AddCollider({ 0, 0, catPeasant.coll_size.x + catPeasant.coll_offset.w, catPeasant.coll_size.y + catPeasant.coll_offset.h }, COLLIDER_TYPE::COLLIDER_CATPEASANT, App->entities);
	collider_size = catPeasant.coll_size;

	speed = { 60.0f, 0 };
	particle_speed = { 80.0f, 80.0f };
	seconds_to_wait = 6.0f;
}

void CatPeasant::Move(float dt)
{
	deltaTime = dt;

	i_pos.x = (int)position.x;
	i_pos.y = (int)position.y;

	// Update animations speed
	UpdateAnimations(dt);

	// Update movement
	up = false;
	down = false;
	left = false;
	right = false;
	UpdateDirection();

	// Update hurt and death
	Wounded();

	// Update path (which is a pathfinding)
	if (!stop) {
		DoHit();
		CoolDown();

		UpdatePath();
	}
	//_update_path

	// Update state
	GeneralStatesMachine();

	if (dead)
		LOG("DEAD");

	// Update collider
	//collider_pos = { i_pos.x + imp.coll_offset.x, i_pos.y + imp.coll_offset.y };
	//collider->SetPos(collider_pos.x, collider_pos.y);
	collider->SetPos(i_pos.x, i_pos.y);
}

void CatPeasant::Wounded()
{
	if (right_hurt) {
		if (catPeasant.r_hurt.Finished()) {
			catPeasant.r_hurt.Reset();
			stop = false;
			right_hurt = false;
		}
	}
	else if (left_hurt) {
		if (catPeasant.l_hurt.Finished()) {
			catPeasant.l_hurt.Reset();
			stop = false;
			left_hurt = false;
		}
	}
	else if (right_die) {
		if (catPeasant.r_dead.GetCurrentFrame().x == 464) {
			dead = true;
			catPeasant.r_dead.Stop();
		}
	}
	else if (left_die) {
		if (catPeasant.l_dead.GetCurrentFrame().x == 312) {
			dead = true;
			catPeasant.l_dead.Stop();
		}
	}
}

void CatPeasant::GeneralStatesMachine() {

	switch (catPeasantState) {

	case CatPeasantState::r_idle:
		if (right_hurt) {
			catPeasantState = CatPeasantState::r_hurt;
			break;
		}
		if (right_die) {
			catPeasantState = CatPeasantState::r_dead;
			break;
		}
		if (left_hurt) {
			catPeasantState = CatPeasantState::l_hurt;
			break;
		}
		if (left_die) {
			catPeasantState = CatPeasantState::l_dead;
			break;
		}
		if (right_hit) {
			catPeasantState = CatPeasantState::r_attack;
			break;
		}
		if (left) {
			catPeasantState = CatPeasantState::l_idle;
			break;
		}
		animation = &catPeasant.r_idle;
		break;

	case CatPeasantState::l_idle:
		if (left_hurt) {
			catPeasantState = CatPeasantState::l_hurt;
			break;
		}
		if (left_die) {
			catPeasantState = CatPeasantState::l_dead;
			break;
		}
		if (right_hurt) {
			catPeasantState = CatPeasantState::r_hurt;
			break;
		}
		if (right_die) {
			catPeasantState = CatPeasantState::r_dead;
			break;
		}
		if (left_hit) {
			catPeasantState = CatPeasantState::l_attack;
			break;
		}
		if (right) {
			catPeasantState = CatPeasantState::r_idle;
			break;
		}
		animation = &catPeasant.l_idle;
		break;

	case CatPeasantState::r_attack:
		if (left) {
			catPeasantState = CatPeasantState::l_attack;
			break;
		}
		animation = &catPeasant.r_throw_staff;

		catPeasantState = CatPeasantState::r_idle;
		break;

	case CatPeasantState::l_attack:
		if (right) {
			catPeasantState = CatPeasantState::r_attack;
			break;
		}
		animation = &catPeasant.l_throw_staff;

		catPeasantState = CatPeasantState::l_idle;
		break;

	case CatPeasantState::r_hurt:
		if (left) {
			catPeasantState = CatPeasantState::l_hurt;
			break;
		}
		animation = &catPeasant.r_hurt;

		catPeasantState = CatPeasantState::r_idle;
		break;

	case CatPeasantState::l_hurt:
		if (right) {
			catPeasantState = CatPeasantState::r_hurt;
			break;
		}
		animation = &catPeasant.l_hurt;

		catPeasantState = CatPeasantState::l_idle;
		break;

	case CatPeasantState::r_dead:
		if (left) {
			catPeasantState = CatPeasantState::l_dead;
			break;
		}
		animation = &catPeasant.r_dead;

		catPeasantState = CatPeasantState::r_idle;
		break;

	case CatPeasantState::l_dead:
		if (right) {
			catPeasantState = CatPeasantState::r_dead;
			break;
		}
		animation = &catPeasant.l_dead;

		catPeasantState = CatPeasantState::l_idle;
		break;
	}
}

void CatPeasant::UpdateDirection() {

	if (position.x < last_pos.x) {
		left = true;
	}
	else if (position.x > last_pos.x) {
		right = true;
	}

	if (position.y < last_pos.y) {
		up = true;
	}
	else if (position.y > last_pos.y) {
		down = true;
	}

	last_pos = position;
}

void CatPeasant::UpdateAnimations(float dt)
{
	float speed = 10.0f;

	catPeasant.r_idle.speed = speed * dt;
	catPeasant.l_idle.speed = speed * dt;
	catPeasant.r_idle_no_staff.speed = speed * dt;
	catPeasant.l_idle_no_staff.speed = speed * dt;
	catPeasant.r_hurt.speed = 5.0f * dt;
	catPeasant.l_hurt.speed = 5.0f * dt;
	catPeasant.r_hurt_no_staff.speed = speed * dt;
	catPeasant.l_hurt_no_staff.speed = speed * dt;
	catPeasant.r_dead.speed = 5.0f * dt;
	catPeasant.l_dead.speed = 5.0f * dt;
	catPeasant.r_dead_no_staff.speed = speed * dt;
	catPeasant.l_dead_no_staff.speed = speed * dt;
	catPeasant.r_throw_staff.speed = speed * dt;
	catPeasant.l_throw_staff.speed = speed * dt;
}

void CatPeasant::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_ARROW) {
		lives--;
		stop = true;
		LOG("Lifes: %i", lives);

		if (lives == 1) {
			if (App->entities->playerData->position.x < position.x)
				left_hurt = true;
			else
				right_hurt = true;
		}
		else if (lives == 0) {
			if (App->entities->playerData->position.x < position.x)
				left_die = true;
			else
				right_die = true;
		}
	}
}

void CatPeasant::UpdatePath()
{
	if (path_info->end_pos.x != NULL && path_info->end_pos.y != NULL) {

		// If the enemy doesn't see the player, create its normal path
		if (normal_path_finished) {
			if (ResetNormalPathVariables()) {
				create_normal_path = true;
			}
		}

		// Create normal path
		if (create_normal_path) {
			iPoint to_go;

			FindDestination(to_go);

			if (CreatePathfinding(to_go)) {
				do_normal_path = true;
				create_normal_path = false;
			}
		}

		// If it is home, update the normal path
		if (do_normal_path) {

			// If she sees the player, she throws her staff towards him
			if (position.DistanceTo(App->entities->playerData->position) < 100.0f && cooldown <= 0) {
				Hit();
				wait = true;
				cool = true;
				cooldown = seconds_to_wait;
			}

			if (!wait) {
				if (!Pathfind()) {
					RecalculatePath();
					normal_path_finished = true;
				}
			}
		}
	}
}

void CatPeasant::FindDestination(iPoint& to_go)
{
	switch (normal_path_index) {
	case StartEndPath::start:
		to_go = path_info->start_pos;
		break;
	case StartEndPath::end:
		to_go = path_info->end_pos;
		break;
	default:
		to_go = { 0,0 };
	}
}

void CatPeasant::Hit()
{
	if (do_hit) {
		if (right)
			right_hit = true;
		else if (left)
			left_hit = true;
		else
			right_hit = true;

		do_hit = false;
	}
}

void CatPeasant::DoHit()
{
	if (right_hit) {
		if (catPeasant.r_throw_staff.Finished()) {
			// Add particle
			App->particles->AddParticle(App->particles->CatPeasantSinus, i_pos.x, i_pos.y + 20, COLLIDER_PEASANT_SHOT, NULL, particle_speed);

			// Reset variables
			right_hit = false;
			catPeasant.r_throw_staff.Reset();
			do_hit = true;
			wait = false;
		}
	}
	else if (left_hit) {
		if (catPeasant.l_throw_staff.Finished()) {
			// Add particle
			App->particles->AddParticle(App->particles->CatPeasantSinus, i_pos.x, i_pos.y + 20, COLLIDER_PEASANT_SHOT, NULL, particle_speed);

			// Reset variables
			left_hit = false;
			catPeasant.l_throw_staff.Reset();
			do_hit = true;
			wait = false;
		}
	}
}

void CatPeasant::CoolDown()
{
	if (cool) {
		cooldown -= deltaTime;

		if (cooldown <= 0)
			cool = false;
	}
}

bool CatPeasant::ResetNormalPathVariables()
{
	bool ret = true;

	do_normal_path = false;
	normal_path_finished = false;
	pathfinding_size = 0;

	return ret;
}

bool CatPeasant::CreatePathfinding(iPoint destination)
{
	bool ret = false;

	if (App->pathfinding->CreatePath(App->map->WorldToMap(i_pos.x, i_pos.y), App->map->WorldToMap(destination.x, destination.y), Distance::MANHATTAN) > -1) {
		last_pathfinding = App->pathfinding->GetLastPath();

		pathfinding_size = last_pathfinding->Count();

		if (pathfinding_size > 1)
			pathfinding_index = 1;
		else
			pathfinding_index = 0;

		mlast_pathfinding.Clear();

		for (int i = 0; i < pathfinding_size; ++i) {
			mlast_pathfinding.PushBack(*last_pathfinding->At(i));
			ret = true;
		}
	}

	return ret;
}

void CatPeasant::UpdateMovement(iPoint to_go)
{
	if (i_pos.x < to_go.x)
		position.x += speed.x * deltaTime;
	else if (i_pos.x > to_go.x)
		position.x -= speed.x * deltaTime;
	if (i_pos.y < to_go.y)
		position.y += speed.x * deltaTime;
	else if (i_pos.y > to_go.y)
		position.y -= speed.x * deltaTime;
}

bool CatPeasant::Pathfind()
{
	bool ret = true;

	iPoint to_go = App->map->MapToWorld(mlast_pathfinding[pathfinding_index].x, mlast_pathfinding[pathfinding_index].y);

	UpdateMovement(to_go);

	if (i_pos == to_go) {
		if (pathfinding_index < pathfinding_size - 1)
			pathfinding_index++;
	}

	if (i_pos == App->map->MapToWorld(mlast_pathfinding[pathfinding_size - 1].x, mlast_pathfinding[pathfinding_size - 1].y))
		ret = false;

	return ret;
}

void CatPeasant::RecalculatePath()
{
	switch (normal_path_index) {
	case StartEndPath::start:
		normal_path_index = StartEndPath::end;
		break;
	case StartEndPath::end:
		normal_path_index = StartEndPath::start;
		break;
	default:
		break;
	}
}

