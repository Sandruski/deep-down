#include "j1App.h"
#include "j1Enemies.h"
#include "Imp.h"

#include "j1Collision.h"
#include "j1Player.h"
#include "j1Particles.h"

#include "j1Input.h"

#include "SDL/include/SDL_timer.h"

Imp::Imp(int x, int y) : Enemy(x, y)
{
	imp = App->enemies->GetImpInfo();

	animation = &imp.r_shield_idle;
	impState = ImpState::l_throw_bomb;

	collider = App->collision->AddCollider({ 0, 0, imp.coll_size.x + imp.coll_offset.w, imp.coll_size.y + imp.coll_offset.h }, COLLIDER_TYPE::COLLIDER_IMP, App->enemies);

	// Need organization
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
	collider_pos = { position.x + imp.coll_offset.x, position.y + imp.coll_offset.y };
	collider->SetPos(collider_pos.x, collider_pos.y);

	//position.x = App->player->position.x;
	//position.y = App->player->position.y;
	position.y += speed.y;

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
		animation = &imp.r_shield_idle;
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
		animation = &imp.l_shield_idle;
		break;

	case r_shield_walk:
		if (left) {
			impState = ImpState::l_shield_walk;
			break;
		}
		animation = &imp.r_shield_walk;
		break;

	case l_shield_walk:
		if (right) {
			impState = ImpState::r_shield_walk;
			break;
		}
		animation = &imp.l_shield_walk;
		break;

	case r_jump:
		if (left) {
			impState = ImpState::l_jump;
			break;
		}
		animation = &imp.r_jump;
		break;

	case l_jump:
		if (right) {
			impState = ImpState::r_jump;
			break;
		}
		animation = &imp.l_jump;
		break;

	case r_throw_bomb:
		if (left) {
			impState = ImpState::l_throw_bomb;
			break;
		}
		animation = &imp.r_throw_bomb;
		break;

	case l_throw_bomb:
		if (right) {
			impState = ImpState::r_throw_bomb;
			break;
		}
		animation = &imp.l_throw_bomb;
		break;

	case r_shield_hurt:
		if (left) {
			impState = ImpState::l_shield_hurt;
			break;
		}
		animation = &imp.r_shield_hurt;
		break;

	case l_shield_hurt:
		if (right) {
			impState = ImpState::r_shield_hurt;
			break;
		}
		animation = &imp.l_shield_hurt;
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