#include "j1App.h"
#include "j1EntityFactory.h"
#include "Imp.h"

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Audio.h"

#include "j1Input.h"

#include "SDL/include/SDL_timer.h"

/*
IMP (enemy):
· Type: ground.
· Lives: 2.

- Follows a path A-B (set in Tiled) using pathfinding.
- Skips her path when she sees the player and pathfinds him.
- When she is near the player, she throws a bomb towards him.
- If she stops seeing the player, she goes back to her initial path.
- If she reaches the end B of her path, she disappears (and respawns at the start A of her path).
*/

Imp::Imp(float x, float y, PathInfo* path) : Entity(x, y, path)
{
	imp = App->entities->GetImpInfo();

	///
	animation = &imp.r_shield_idle;
	impState = ImpState::r_shield_idle;

	lives = 2;
	dead = false;

	collider = App->collision->AddCollider({ 0, 0, imp.coll_size.x + imp.coll_offset.w, imp.coll_size.y + imp.coll_offset.h }, COLLIDER_TYPE::COLLIDER_IMP, App->entities);
	collider_size = imp.coll_size;

	speed = { 60.0f, 2 };
	particle_speed = { 50.0f, 50.0f };
	seconds_to_wait = 1.0f;
	distance_to = 200.0f;
}

void Imp::Move(float dt)
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

	// Update path/pathfinding	
	if (!stop) {
		DoHit();
		CoolDown();

		if (!pathfinding)
			UpdatePath();

		
			UpdatePathfinding();
	}
	// Update state
	GeneralStatesMachine();

	// Update collider
	//collider_pos = { i_pos.x + imp.coll_offset.x, i_pos.y + imp.coll_offset.y };
	//collider->SetPos(collider_pos.x, collider_pos.y);
	collider->SetPos(i_pos.x, i_pos.y);
}

void Imp::Wounded() 
{
	if (right_hurt) {
		if (imp.r_shield_hurt.Finished()) {
			if (lives == 0)
				dead = true;

			imp.r_shield_hurt.Reset();
			stop = false;
			right_hurt = false;
		}
	}
	else if (left_hurt) {
		if (imp.l_shield_hurt.Finished()) {
			if (lives == 0)
				dead = true;

			imp.l_shield_hurt.Reset();
			stop = false;
			left_hurt = false;
		}
	}
}

void Imp::GeneralStatesMachine() 
{
	switch (impState) {

	case ImpState::r_shield_idle:
		if (back) {
			impState = ImpState::back_to_start;
			break;
		}
		if (right_hurt) {
			impState = ImpState::r_shield_hurt;
			break;
		}
		if (left_hurt) {
			impState = ImpState::l_shield_hurt;
			break;
		}
		if (right_hit) {
			impState = ImpState::r_throw_bomb;
			break;
		}
		if (right) {
			impState = ImpState::r_shield_walk;
			break;
		}
		if (left) {
			impState = ImpState::l_shield_walk;
			break;
		}
		animation = &imp.r_shield_idle;
		break;

	case ImpState::l_shield_idle:
		if (back) {
			impState = ImpState::back_to_start;
			break;
		}
		if (left_hurt) {
			impState = ImpState::l_shield_hurt;
			break;
		}
		if (right_hurt) {
			impState = ImpState::r_shield_hurt;
			break;
		}
		if (left_hit) {
			impState = ImpState::l_throw_bomb;
			break;
		}
		if (right) {
			impState = ImpState::r_shield_walk;
			break;
		}
		if (left) {
			impState = ImpState::l_shield_walk;
			break;
		}
		animation = &imp.l_shield_idle;
		break;

	case ImpState::r_shield_walk:
		if (down) {
			impState = ImpState::r_jump;
			break;
		}
		if (left) {
			impState = ImpState::l_shield_walk;
			break;
		}
		animation = &imp.r_shield_walk;
		if (!right && !left)
			impState = ImpState::r_shield_idle;
		break;

	case ImpState::l_shield_walk:
		if (down) {
			impState = ImpState::l_jump;
			break;
		}
		if (right) {
			impState = ImpState::r_shield_walk;
			break;
		}
		animation = &imp.l_shield_walk;
		if (!right && !left)
			impState = ImpState::l_shield_idle;
		break;

	case ImpState::r_jump:
		if (left) {
			impState = ImpState::l_jump;
			break;
		}
		animation = &imp.r_jump;
		if (!right && !left && !up && !down)
			impState = ImpState::r_shield_idle;
		break;

	case ImpState::l_jump:
		if (right) {
			impState = ImpState::r_jump;
			break;
		}
		animation = &imp.l_jump;
		if (!right && !left && !up && !down)
			impState = ImpState::l_shield_idle;
		break;

	case ImpState::r_throw_bomb:
		if (left) {
			impState = ImpState::l_throw_bomb;
			break;
		}
		animation = &imp.r_throw_bomb;

		impState = ImpState::r_shield_idle;
		break;

	case ImpState::l_throw_bomb:
		if (right) {
			impState = ImpState::r_throw_bomb;
			break;
		}
		animation = &imp.l_throw_bomb;

		impState = ImpState::l_shield_idle;
		break;

	case ImpState::r_shield_hurt:
		if (left) {
			impState = ImpState::l_shield_hurt;
			break;
		}
		animation = &imp.r_shield_hurt;
		impState = ImpState::r_shield_idle;
		break;

	case ImpState::l_shield_hurt:
		if (right) {
			impState = ImpState::r_shield_hurt;
			break;
		}
		animation = &imp.l_shield_hurt;
		impState = ImpState::l_shield_idle;
		break;

	case ImpState::back_to_start:
		animation = &imp.invisible;
		impState = ImpState::l_shield_idle;
		break;
	}
}

void Imp::UpdateDirection() {

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

void Imp::UpdateAnimations(float dt)
{
	float speed = 10.0f;

	imp.r_shield_idle.speed = speed * dt;
	imp.l_shield_idle.speed = speed * dt;
	imp.r_shield_hurt.speed = 5.0f * dt;
	imp.l_shield_hurt.speed = 5.0f * dt;
	imp.r_jump.speed = speed * dt;
	imp.l_jump.speed = speed * dt;
	imp.r_throw_bomb.speed = speed * dt;
	imp.l_throw_bomb.speed = speed * dt;
	imp.r_shield_walk.speed = speed * dt;
	imp.l_shield_walk.speed = speed * dt;
}

void Imp::OnCollision(Collider* c1, Collider* c2) 
{
	if (c2->type == COLLIDER_ARROW && !back) {
		lives--;
		App->audio->PlayFx(6);
		stop = true;

		if (App->entities->playerData->position.x < position.x)
			left_hurt = true;
		else
			right_hurt = true;
	}
}

void Imp::UpdatePath()
{
	if (path_info->end_pos.x != NULL && path_info->end_pos.y != NULL) {

		// If enemy was pathfinding towards the player and doesn't see it anymore, create a pathfinding back home
		if (pathfinding_stop) {
			if (ResetNormalPathVariables()) {
				create_pathfinding_back = true;
				pathfinding_stop = false;
			}
		}

		// Create pathfinding back
		if (create_pathfinding_back) {
			iPoint to_go;

			FindDestination(to_go);

			if (CreatePathfinding(to_go)) {
				going_back_home = true;
				do_normal_path = true;
				create_pathfinding_back = false;
			}
		}

		// If the enemy doesn't see the player, create its normal path
		if (normal_path_finished && !pathfinding) {
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

		// If it has to go back home and hasn't reached home yet, update the pathfinding back
		if (going_back_home) {
			if (!Pathfind(20.0f)) {
				RecalculatePath();
				going_back_home = false;
				pathfinding_finished = true;
			}
		}
		// If it is home, update the normal path
		else if (do_normal_path) {
			if (!Pathfind(20.0f)) {
				RecalculatePath();
				normal_path_finished = true;
			}
		}
	}
}

void Imp::FindDestination(iPoint& to_go)
{

	switch (normal_path_index) {
	case StartEndPath::start:
		to_go = path_info->start_pos;
		to_go.y -= 17;
		break;
	case StartEndPath::end:
		to_go = path_info->end_pos;
		break;
	default:
		to_go = { 0,0 };
	}
}

void Imp::UpdatePathfinding()
{
	SDL_Rect enemy_pos;
	SDL_Rect player_pos;
	UpdatePathfindingAffectArea(enemy_pos, player_pos);

	// If player is near the enemy... Create a pathfinding towards it
	if (pathfinding_finished && SDL_HasIntersection(&enemy_pos, &player_pos) && cooldown <= 0 && !back
		&& position.DistanceTo(App->entities->playerData->position) < 100.0f && App->entities->playerData->speed.y == 0) {
		if (ResetPathfindingVariables()) {
			create_pathfinding = true;
		}
	}

	// Create pathfinding
	if (create_pathfinding) {

		if (App->entities->playerData->position.x < position.x)
			dest.x = (int)App->entities->playerData->position.x + 50;
		else
			dest.x = (int)App->entities->playerData->position.x - 50;

		dest.y = (int)App->entities->playerData->position.y;

		if (CreatePathfinding(dest)) {
			pathfinding = true;
			create_pathfinding = false;
			go = true;
		}
		else {
			pathfinding_stop = true;
			create_pathfinding = false;
		}
	}

	if (go && SDL_HasIntersection(&enemy_pos, &player_pos)) {
		pathfind = true;
		go = false;
	}

	if (pathfinding) {
		// If player is near the enemy and the enemy hasn't reached the end of the path yet, update the path
		if (pathfind) {
			if (!Pathfind(60.0f)) {
				fPoint i_dest;
				i_dest.x = dest.x;
				i_dest.y = dest.y;

				if (position.DistanceTo(i_dest) < distance_to && cooldown <= 0 && !back && speed.y == 0) {
					Hit();
					wait = true;
					cool = true;
					cooldown = seconds_to_wait;
				}
				else if (!wait)
					pathfind = false;
			}
		}
		else if (!pathfind) {
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

void Imp::Hit()
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

void Imp::DoHit()
{
	if (right_hit) {
		if (imp.r_throw_bomb.Finished()) {
			// Add particle
			App->particles->AddParticle(App->particles->Imp_r_bomb, i_pos.x + 50, i_pos.y, COLLIDER_IMP_BOMB, NULL, particle_speed);

			// Reset variables
			right_hit = false;
			imp.r_throw_bomb.Reset();
			do_hit = true;
			wait = false;

			pathfind = false;
			pathfinding = false;
			pathfinding_stop = true;
		}
	}
	else if (left_hit) {
		if (imp.l_throw_bomb.Finished()) {
			// Add particle
			App->particles->AddParticle(App->particles->Imp_l_bomb, i_pos.x - 50, i_pos.y, COLLIDER_IMP_BOMB, NULL, particle_speed);

			// Reset variables
			left_hit = false;
			imp.l_throw_bomb.Reset();
			do_hit = true;
			wait = false;

			pathfind = false;
			pathfinding = false;
			pathfinding_stop = true;
		}
	}
}

void Imp::CoolDown()
{
	if (cool) {
		cooldown -= deltaTime;

		if (cooldown <= 0)
			cool = false;
	}
}

void Imp::UpdatePathfindingAffectArea(SDL_Rect& enemy, SDL_Rect& player)
{
	enemy = { i_pos.x - 30, i_pos.y - 30, 100, 100 };
	player = { (int)App->entities->playerData->position.x - 50, (int)App->entities->playerData->position.y - 50, 200, 100 };
}

bool Imp::ResetPathfindingVariables()
{
	bool ret = true;

	pathfinding_finished = false;
	pathfinding_stop = false;
	pathfinding = false;
	pathfind = true;
	pathfinding_size = 0;

	return ret;
}

bool Imp::ResetNormalPathVariables()
{
	bool ret = true;

	do_normal_path = false;
	normal_path_finished = false;
	pathfinding_size = 0;

	return ret;
}

bool Imp::CreatePathfinding(iPoint destination)
{
	bool ret = false;

	IsGround(destination);

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

void Imp::IsGround(iPoint& pos) 
{
	iPoint check = App->map->WorldToMap(pos.x, pos.y);

	while (App->map->collisionLayer->Get(check.x, check.y) != 1180 && App->map->collisionLayer->Get(check.x, check.y) != 1181
		&& App->map->collisionLayer->Get(check.x, check.y) != 1182 && App->map->collisionLayer->Get(check.x, check.y) != 1183) {

		pos.y += App->map->data.tile_height;
		check = App->map->WorldToMap(pos.x, pos.y);

		grounded = true;
	}

	if (grounded) {
		pos.y -= App->map->data.tile_height;
		grounded = false;
	}
}

void Imp::UpdateMovement(iPoint to_go, float velocity)
{	
	speed.x = mlast_pathfinding[pathfinding_index].x - App->map->WorldToMap(position.x, position.y).x;
	speed.y = mlast_pathfinding[pathfinding_index].y - App->map->WorldToMap(position.x, position.y).y;

	speed.x *= velocity * deltaTime;
	speed.y *= velocity * deltaTime;

	position.x += speed.x;
	position.y += speed.y;
}

bool Imp::Pathfind(float velocity)
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

void Imp::RecalculatePath()
{
	switch (normal_path_index) {
	case StartEndPath::start:
		back = false;
		normal_path_index = StartEndPath::end;
		break;
	case StartEndPath::end:
		back = true;
		normal_path_index = StartEndPath::start;
		break;
	default:
		break;
	}
}