#include "j1App.h"
#include "j1EntityFactory.h"
#include "Cat.h"

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Collision.h"
#include "j1Particles.h"

#include "j1Audio.h"

#include "SDL/include/SDL_timer.h"

Cat::Cat(float x, float y, p2DynArray<uint>* cat_states, bool right_death) : Entity(x, y)
{
	cat = App->entities->GetCatInfo();

	this->right_death = right_death;
	this->cat_states = cat_states;

	if (this->cat_states != nullptr)
		catState = *(CatState*)this->cat_states->At(cat_states_index);
	else
		catState = CatState::rc_idle;

	///
	LoadAnimationsSpeed();
	animation = &cat.r_waking_up;

	dead = false;

	collider = App->collision->AddCollider({ 0, 0, cat.coll_size.x + cat.coll_offset.w, cat.coll_size.y + cat.coll_offset.h }, COLLIDER_TYPE::COLLIDER_CAT, App->entities);
	collider_size = cat.coll_size;
}

void Cat::LoadAnimationsSpeed() 
{
	r_idle_speed = cat.r_idle.speed;
	l_idle_speed = cat.l_idle.speed;
	r_going_ZZZ_speed = cat.r_going_ZZZ.speed;
	l_going_ZZZ_speed = cat.l_going_ZZZ.speed;
	r_ZZZ_speed = cat.r_ZZZ.speed;
	l_ZZZ_speed = cat.l_ZZZ.speed;
	r_waking_up_speed = cat.r_waking_up.speed;
	l_waking_up_speed = cat.l_waking_up.speed;
	r_to_crouch_speed = cat.r_to_crouch.speed;
	l_to_crouch_speed = cat.l_to_crouch.speed;
	r_crouch_speed = cat.r_crouch.speed;
	l_crouch_speed = cat.l_crouch.speed;
	r_rise_speed = cat.r_rise.speed;
	l_rise_speed = cat.l_rise.speed;
	r_jump_speed = cat.r_jump.speed;
	l_jump_speed = cat.l_jump.speed;
	r_fall_speed = cat.r_fall.speed;
	l_fall_speed = cat.l_fall.speed;
	r_land_soft_speed = cat.r_land_soft.speed;
	l_land_soft_speed = cat.l_land_soft.speed;
	r_roll_speed = cat.r_roll.speed;
	l_roll_speed = cat.l_roll.speed;
	r_to_run_speed = cat.r_to_run.speed;
	l_to_run_speed = cat.l_to_run.speed;
	r_run_speed = cat.r_run.speed;
	l_run_speed = cat.l_run.speed;
	r_turn_speed = cat.r_turn.speed;
	l_turn_speed = cat.l_turn.speed;
	r_brake_speed = cat.r_brake.speed;
	l_brake_speed = cat.l_brake.speed;
	r_dead_speed = cat.r_dead.speed;
	l_dead_speed = cat.l_dead.speed;
	attack_speed = cat.attack.speed;
}

void Cat::Move(const float dt)
{
	deltaTime = dt;

	i_pos.x = (int)position.x;
	i_pos.y = (int)position.y;

	// Reset all animations
	if (reset_animations) {
		ResetAllAnimations();
		reset_animations = false;
	}

	// Update animations speed
	UpdateAnimations(dt);

	// Update state
	GeneralStatesMachine();

	// Update collider
	collider_pos = { i_pos.x + cat.coll_offset.x, i_pos.y + cat.coll_offset.y };
	collider->SetPos(collider_pos.x, collider_pos.y);
}

void Cat::UpdateAnimations(const float dt)
{
	cat.r_idle.speed = r_idle_speed * dt;
	cat.l_idle.speed = l_idle_speed * dt;
	cat.r_going_ZZZ.speed = r_going_ZZZ_speed * dt;
	cat.l_going_ZZZ.speed = l_going_ZZZ_speed * dt;
	cat.r_ZZZ.speed = r_ZZZ_speed * dt;
	cat.l_ZZZ.speed = l_ZZZ_speed * dt;
	cat.r_waking_up.speed = r_waking_up_speed * dt;
	cat.l_waking_up.speed = l_waking_up_speed * dt;
	cat.r_to_crouch.speed = r_to_crouch_speed * dt;
	cat.l_to_crouch.speed = l_to_crouch_speed * dt;
	cat.r_crouch.speed = r_crouch_speed * dt;
	cat.l_crouch.speed = l_crouch_speed * dt;
	cat.r_rise.speed = r_rise_speed * dt;
	cat.l_rise.speed = l_rise_speed * dt;
	cat.r_jump.speed = r_jump_speed * dt;
	cat.l_jump.speed = l_jump_speed * dt;
	cat.r_fall.speed = r_fall_speed * dt;
	cat.l_fall.speed = l_fall_speed * dt;
	cat.r_land_soft.speed = r_land_soft_speed * dt;
	cat.l_land_soft.speed = l_land_soft_speed * dt;
	cat.r_roll.speed = r_roll_speed * dt;
	cat.l_roll.speed = l_roll_speed * dt;
	cat.r_to_run.speed = r_to_run_speed * dt;
	cat.l_to_run.speed = l_to_run_speed * dt;
	cat.r_run.speed = r_run_speed * dt;
	cat.l_run.speed = l_run_speed * dt;
	cat.r_turn.speed = r_turn_speed * dt;
	cat.l_turn.speed = l_turn_speed * dt;
	cat.r_brake.speed = r_brake_speed * dt;
	cat.l_brake.speed = l_brake_speed * dt;
	cat.r_dead.speed = r_dead_speed * dt;
	cat.l_dead.speed = l_dead_speed * dt;
	cat.attack.speed = attack_speed * dt;
}

void Cat::UpdateCatState()
{
	if (cat_states != nullptr) {
		if (cat_states_index == cat_states->Count() - 1)
			cat_states_index = 0;
		else
			cat_states_index++;

		catState = *(CatState*)cat_states->At(cat_states_index);
	}

	reset_animations = true;
}

void Cat::ResetAllAnimations()
{
	cat.r_idle.Reset();
	cat.l_idle.Reset();
	cat.r_going_ZZZ.Reset();
	cat.l_going_ZZZ.Reset();
	cat.r_ZZZ.Reset();
	cat.l_ZZZ.Reset();
	cat.r_waking_up.Reset();
	cat.l_waking_up.Reset();
	cat.r_to_crouch.Reset();
	cat.l_to_crouch.Reset();
	cat.r_crouch.Reset();
	cat.l_crouch.Reset();
	cat.r_rise.Reset();
	cat.l_rise.Reset();
	cat.r_jump.Reset();
	cat.l_jump.Reset();
	cat.r_fall.Reset();
	cat.l_fall.Reset();
	cat.r_land_soft.Reset();
	cat.l_land_soft.Reset();
	cat.r_roll.Reset();
	cat.l_roll.Reset();
	cat.r_to_run.Reset();
	cat.l_to_run.Reset();
	cat.r_run.Reset();
	cat.l_run.Reset();
	cat.r_turn.Reset();
	cat.l_turn.Reset();
	cat.r_brake.Reset();
	cat.l_brake.Reset();
	cat.r_dead.Reset();
	cat.l_dead.Reset();
	cat.attack.Reset();
}

void Cat::GeneralStatesMachine() {

	switch (catState) {

	case CatState::rc_idle:

		if (cat.r_idle.Finished()) {
			UpdateCatState();
			break;
		}
		animation = &cat.r_idle;
		break;

	case CatState::lc_idle:

		if (cat.l_idle.Finished()) {
			UpdateCatState();
			break;
		}
		animation = &cat.l_idle;
		break;

	case CatState::rc_ZZZ:

		if (cat.r_going_ZZZ.Finished()) {

			if (cat.r_ZZZ.Finished()) {

				if (cat.r_waking_up.Finished()) {
					UpdateCatState();
					break;
				}

				animation = &cat.r_waking_up;
				break;
			}
			animation = &cat.r_ZZZ;
			break;
		}
		animation = &cat.r_going_ZZZ;
		break;

	case CatState::lc_ZZZ:

		if (cat.l_going_ZZZ.Finished()) {

				if (cat.l_ZZZ.Finished()) {

					if (cat.l_waking_up.Finished()) {
						UpdateCatState();
						break;
					}
				animation = &cat.l_waking_up;
				break;
			}
			animation = &cat.l_ZZZ;
			break;
		}
		animation = &cat.l_going_ZZZ;
		break;

	case CatState::rc_crouch:

		if (cat.r_to_crouch.Finished()) {

			if (cat.r_crouch.Finished()) {

				if (cat.r_rise.Finished()) {
					UpdateCatState();
					break;
				}
				animation = &cat.r_rise;
				break;
			}
			animation = &cat.r_crouch;
			break;
		}
		animation = &cat.r_to_crouch;
		break;

	case CatState::lc_crouch:

		if (cat.l_to_crouch.Finished()) {

			if (cat.l_crouch.Finished()) {

				if (cat.l_rise.Finished()) {
					UpdateCatState();
					break;
				}
				animation = &cat.l_rise;
				break;
			}
			animation = &cat.l_crouch;
			break;
		}
		animation = &cat.l_to_crouch;
		break;

	case CatState::rc_jump:

		if (cat.r_jump.Finished()) {

			if (cat.r_land_soft.Finished()) {
				UpdateCatState();
				break;
			}
			animation = &cat.r_land_soft;
			break;
		}
		animation = &cat.r_jump;
		break;

	case CatState::lc_jump:

		if (cat.l_jump.Finished()) {

			if (cat.l_land_soft.Finished()) {
				UpdateCatState();
				break;
			}
			animation = &cat.l_land_soft;
			break;
		}
		animation = &cat.l_jump;
		break;

	case CatState::rc_jump_no_fall:

		if (cat.r_jump.Finished()) {

			UpdateCatState();
			break;
		}
		animation = &cat.r_jump;
		break;

	case CatState::lc_jump_no_fall:

		if (cat.l_jump.Finished()) {

			UpdateCatState();
			break;
		}
		animation = &cat.l_jump;
		break;

	case CatState::rc_land_soft:

		if (cat.r_land_soft.Finished()) {

			UpdateCatState();
			break;
		}
		animation = &cat.r_land_soft;
		break;

	case CatState::lc_land_soft:

		if (cat.l_land_soft.Finished()) {

			UpdateCatState();
			break;
		}
		animation = &cat.l_land_soft;
		break;

	case CatState::rc_roll:

		if (cat.r_roll.Finished()) {
			UpdateCatState();
			break;
		}
		animation = &cat.r_roll;
		break;

	case CatState::lc_roll:

		if (cat.l_roll.Finished()) {
			UpdateCatState();
			break;
		}
		animation = &cat.l_roll;
		break;

	case CatState::rc_dead_run:

		if (cat.r_to_run.Finished()) {

			position.x += 50 * deltaTime;

			if (cat.r_run.Finished()) {
				App->particles->AddParticle(App->particles->sparkle, i_pos.x, i_pos.y, COLLIDER_TYPE::COLLIDER_CAT_SPARKLE, NULL, { 0.0f,0.0f });
				dead = true;
				break;
			}
			animation = &cat.r_run;
			break;
		}
		animation = &cat.r_to_run;
		break;

	case CatState::lc_dead_run:

		if (cat.l_to_run.Finished()) {

			position.x -= 50 * deltaTime;

			if (cat.l_run.Finished()) {
				App->particles->AddParticle(App->particles->sparkle, i_pos.x, i_pos.y, COLLIDER_TYPE::COLLIDER_CAT_SPARKLE, NULL, { 0.0f,0.0f });
				dead = true;
				break;
			}
			animation = &cat.l_run;
			break;
		}
		animation = &cat.l_to_run;
		break;

	case CatState::rc_dead:

		if (cat.r_dead.Finished()) {
			App->particles->AddParticle(App->particles->sparkle, i_pos.x, i_pos.y, COLLIDER_TYPE::COLLIDER_CAT_SPARKLE, NULL, { 0.0f,0.0f });
			dead = true;
			break;
		}
		animation = &cat.r_dead;
		break;

	case CatState::lc_dead:

		if (cat.l_dead.Finished()) {
			App->particles->AddParticle(App->particles->sparkle, i_pos.x, i_pos.y, COLLIDER_TYPE::COLLIDER_CAT_SPARKLE, NULL, { 0.0f,0.0f });
			dead = true;
			break;
		}
		animation = &cat.l_dead;
		break;

	case CatState::rc_run:

			if (cat.r_run.Finished()) {
				UpdateCatState();
				break;
			}
			animation = &cat.r_run;
			break;

	case CatState::lc_run:

		if (cat.l_run.Finished()) {
			UpdateCatState();
			break;
		}
		animation = &cat.l_run;
		break;

	default:
		break;
	}
}

void Cat::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_ARROW) {
		if (right_death)
			catState = CatState::rc_dead;
		else
			catState = CatState::lc_dead;
	}
	else if (c2->type == COLLIDER_PLAYER) {
		if (right_death)
			catState = CatState::rc_run;
		else
			catState = CatState::lc_run;
	}
}

void Cat::SetCatState(uint state) 
{
	catState = (CatState)state;
}