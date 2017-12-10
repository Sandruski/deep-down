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
#include "j1Audio.h"

#include "SDL/include/SDL_timer.h"

/*
CAT PEASANT (enemy):
· Type: air.
· Lives: 2.

- Follows a continuous path A-B (set in Tiled) using pathfinding.
- If she sees the player, she throws her magic cane towards him.
*/

CatPeasant::CatPeasant(float x, float y, PathInfo* path) : Entity(x, y), path_info(path), mlast_pathfinding(DEFAULT_PATH_LENGTH)
{
	last_pathfinding = nullptr;
	mlast_pathfinding.Clear();

	catPeasant = App->entities->GetCatPeasantInfo();

	///
	LoadAnimationsSpeed();
	animation = &catPeasant.r_idle;
	catPeasantState = CatPeasantState::r_idle;

	lives = catPeasant.lives;
	dead = false;

	collider = App->collision->AddCollider({ 0, 0, catPeasant.coll_size.x + catPeasant.coll_offset.w, catPeasant.coll_size.y + catPeasant.coll_offset.h }, COLLIDER_TYPE::COLLIDER_CATPEASANT, App->entities);
	collider_size = catPeasant.coll_size;
}

void CatPeasant::LoadAnimationsSpeed() 
{
	r_idle_speed = catPeasant.r_idle.speed;
	l_idle_speed = catPeasant.l_idle.speed;
	r_idle_no_staff_speed = catPeasant.r_idle_no_staff.speed;
	l_idle_no_staff_speed = catPeasant.r_idle_no_staff.speed;
	r_hurt_speed = catPeasant.r_hurt.speed;
	l_hurt_speed = catPeasant.l_hurt.speed;
	r_hurt_no_staff_speed = catPeasant.r_hurt_no_staff.speed;
	l_hurt_no_staff_speed = catPeasant.l_hurt_no_staff.speed;
	r_dead_speed = catPeasant.r_dead.speed;
	l_dead_speed = catPeasant.l_dead.speed;
	r_dead_no_staff_speed = catPeasant.r_dead_no_staff.speed;
	l_dead_no_staff_speed = catPeasant.l_dead_no_staff.speed;
	r_throw_staff_speed = catPeasant.r_throw_staff.speed;
	l_throw_staff_speed = catPeasant.l_throw_staff.speed;
}

void CatPeasant::Move(const float dt)
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

	// Update collider
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
		if (catPeasant.r_dead.Finished()) {
			dead = true;
		}
	}
	else if (left_die) {
		if (catPeasant.l_dead.Finished()) {
			dead = true;
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

void CatPeasant::UpdateAnimations(const float dt)
{
	catPeasant.r_idle.speed = r_idle_speed * dt;
	catPeasant.l_idle.speed = l_idle_speed * dt;
	catPeasant.r_idle_no_staff.speed = r_idle_no_staff_speed * dt;
	catPeasant.l_idle_no_staff.speed = l_idle_no_staff_speed * dt;
	catPeasant.r_hurt.speed = r_hurt_speed * dt;
	catPeasant.l_hurt.speed = l_hurt_speed * dt;
	catPeasant.r_hurt_no_staff.speed = r_hurt_no_staff_speed * dt;
	catPeasant.l_hurt_no_staff.speed = l_hurt_no_staff_speed * dt;
	catPeasant.r_dead.speed = r_dead_speed * dt;
	catPeasant.l_dead.speed = l_dead_speed * dt;
	catPeasant.r_dead_no_staff.speed = r_dead_no_staff_speed * dt;
	catPeasant.l_dead_no_staff.speed = l_dead_no_staff_speed * dt;
	catPeasant.r_throw_staff.speed = r_throw_staff_speed * dt;
	catPeasant.l_throw_staff.speed = l_throw_staff_speed * dt;
}

void CatPeasant::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_ARROW) {
		lives--;
		stop = true;
		App->audio->PlayFx(catPeasant.hurt_fx);

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
	if (path_info->end_pos.x != 0 && path_info->end_pos.y != 0) {

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
			if (position.DistanceTo(App->entities->playerData->position) < catPeasant.min_distance_to_shoot && cooldown <= 0) {
				Hit();
				wait = true;
				cool = true;
				cooldown = catPeasant.seconds_to_wait;
			}

			if (!wait) {
				if (!Pathfind(catPeasant.pathfinding_normal_speed)) {
					RecalculatePath();
					normal_path_finished = true;
				}
			}
		}
	}
}

void CatPeasant::FindDestination(iPoint& to_go) const
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
			App->particles->AddParticle(App->particles->CatPeasantSinus, i_pos.x, i_pos.y + catPeasant.distance_to_player, COLLIDER_CATPEASANT_SHOT, NULL, catPeasant.particle_speed);

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
			App->particles->AddParticle(App->particles->CatPeasantSinus, i_pos.x, i_pos.y + catPeasant.distance_to_player, COLLIDER_CATPEASANT_SHOT, NULL, catPeasant.particle_speed);

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

bool CatPeasant::CreatePathfinding(const iPoint destination)
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

void CatPeasant::UpdateMovement(const iPoint to_go, float velocity)
{
	speed.x = mlast_pathfinding[pathfinding_index].x - App->map->WorldToMap(position.x, position.y).x;
	speed.y = mlast_pathfinding[pathfinding_index].y - App->map->WorldToMap(position.x, position.y).y;

	speed.x *= velocity * deltaTime;
	speed.y *= velocity * deltaTime;

	position.x += speed.x;
	position.y += speed.y;
}

bool CatPeasant::Pathfind(float velocity)
{
	bool ret = true;

	iPoint to_go = App->map->MapToWorld(mlast_pathfinding[pathfinding_index].x, mlast_pathfinding[pathfinding_index].y);

	UpdateMovement(to_go, velocity);

	if (App->map->WorldToMap(i_pos.x, i_pos.y) == App->map->WorldToMap(to_go.x, to_go.y)) {
		if (pathfinding_index < pathfinding_size - 1)
			pathfinding_index++;
	}

	if (App->map->WorldToMap(i_pos.x, i_pos.y) == mlast_pathfinding[pathfinding_size - 1])
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

