#include "j1App.h"
#include "j1EntityFactory.h"
#include "Player.h"

#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Particles.h"
#include "UILifeBar.h"
#include "j1BetweenTransitions.h"

#include "Player.h"
#include "Brofiler\Brofiler.h"
#include <math.h>

#include "SDL/include/SDL_timer.h"

Player::Player(float x, float y) : Entity(x, y)
{
	player = App->entities->GetPlayerInfo();

	LoadAnimationsSpeed();

	// Set player animation (state)
	player.SetState(idle_);

	// Create player collider
	position = { 600, 100 };

	collider_pos = { (int)position.x + player.coll_offset.x, (int)position.y + player.coll_offset.y };
	collider = App->collision->AddCollider({ collider_pos.x, collider_pos.y, player.coll_size.x + player.coll_offset.w, player.coll_size.y + player.coll_offset.h }, COLLIDER_PLAYER, App->entities);	
}

void Player::LoadAnimationsSpeed()
{
	idle_speed = player.idle.speed;
	idle2_speed = player.idle2.speed;
	forward_speed = player.forward.speed;
	backward_speed = player.backward.speed;
	jump_speed = player.jump.speed;
	jump2_speed = player.jump2.speed;
	crouch_speed = player.crouch.speed;
	crouch2_speed = player.crouch2.speed;
	dash_speed = player.dash.speed;
	dash2_speed = player.dash2.speed;
	shot_speed = player.shot.speed;
	shot2_speed = player.shot2.speed;
	crouchShot_speed = player.crouchShot.speed;
	crouchShot2_speed = player.crouchShot2.speed;
	punished_speed = player.punished.speed;
	punished2_speed = player.punished2.speed;
	firstAttack_speed = player.firstAttack.speed;
	firstAttack2_speed = player.firstAttack2.speed;
	secondAttack_speed = player.secondAttack.speed;
	secondAttack2_speed = player.secondAttack2.speed;
	thirdAttack_speed = player.thirdAttack.speed;
	thirdAttack2_speed = player.thirdAttack2.speed;
}

void Player::Move(float dt)
{
	this->dt = dt;

	i_pos.x = (int)position.x;
	i_pos.y = (int)position.y;

	GodMode();

	player.gravity = GRAVITY * dt;

	if (App->fade->GetStep() == fade_step::fade_to_black || App->fade->GetStep() == fade_step::fade_from_black)
		player.gravity = 0;

	// Check if dead
	if (player.GetState() == punished_)
		time += 1 * dt;

	if (App->scene->progress_bar->GetProgress() <= 0 && time >= 1.0f)
		player_is_dead = true;

	CheckIfDead();

	// Check for collisions
	up = true;
	down = true;
	left = true;
	right = true;
	CheckCollision(collider_pos, { player.coll_size.x + player.coll_offset.w, player.coll_size.y + player.coll_offset.h }, player.check_collision_offset, up, down, left, right, player.GetState());

	// Update state
	if (!App->scene->pause)
		PlayerStateMachine();

	ApplySpeed();

	// Update animations speed
	UpdateAnimations(dt);

	// Update collider
	collider_pos = { (int)position.x + player.coll_offset.x, (int)position.y + player.coll_offset.y };
	collider->SetPos(collider_pos.x, collider_pos.y);

	animationPlayer = animation;
}

void Player::UpdateAnimations(float dt) 
{
	player.idle.speed = idle_speed * dt;
	player.idle2.speed = idle2_speed * dt;
	player.forward.speed = forward_speed * dt;
	player.backward.speed = backward_speed * dt;
	player.jump.speed = jump_speed * dt;
	player.jump2.speed = jump2_speed * dt;
	player.crouch.speed = crouch_speed * dt;
	player.crouch2.speed = crouch2_speed * dt;
	player.dash.speed = dash_speed * dt;
	player.dash2.speed = dash2_speed * dt;
	player.shot.speed = shot_speed * dt;
	player.shot2.speed = shot2_speed * dt;
	player.crouchShot.speed = crouchShot_speed * dt;
	player.crouchShot2.speed = crouchShot2_speed * dt;
	player.punished.speed = punished_speed * dt;
	player.punished2.speed = punished2_speed * dt;
	player.firstAttack.speed = firstAttack_speed * dt;
	player.firstAttack2.speed = firstAttack2_speed * dt;
	player.secondAttack.speed = secondAttack_speed * dt;
	player.secondAttack2.speed = secondAttack2_speed * dt;
	player.thirdAttack.speed = thirdAttack_speed * dt;
	player.thirdAttack2.speed = thirdAttack2_speed * dt;
}

void Player::GodMode() 
{
	if (respawnGOD == true) {
		App->scene->god = true;
		timeRespawn += dt;
		if (timeRespawn >= 3.0f) {
			respawnGOD = false;
			App->scene->god = false;
			timeRespawn = 0.0f;
		}
	}
}

void Player::OnCollision(Collider* c1, Collider* c2) {
	if (!App->scene->god) {
		if (c2->type == COLLIDER_CATPEASANT_SHOT || c2->type == COLLIDER_CATPEASANT || c2->type == COLLIDER_IMP_BOMB_EXPLOSION || c2->type == COLLIDER_MONKEY_HIT)
			player.SetState(punished_);
	}
}

void Player::MoveForward() {
	if (right) {
		position.x += P_NORMAL_SPEED * dt;
	}
}

void Player::MoveBackward() {
	if (left) {
		position.x -= P_NORMAL_SPEED * dt;
	}
}

void Player::MoveForwardJumping() {
	if (right) {
		position.x += P_JUMP_SPEED * dt;
	}
}
void Player::MoveBackwardJumping() {
	if (left) {
		position.x -= P_JUMP_SPEED * dt;
	}
}

float Player::Jump() const{
	return -P_JUMP;
}

void PlayerInfo::SetState(playerstates state) {
	this->state = state;
}

void Player::DashForward() {
	if (right)
		position.x += P_NORMAL_SPEED * dt;

	speed.y = 0;
}

void Player::DashBackward() {
	if (left)
		position.x -= P_NORMAL_SPEED * dt;

	speed.y = 0;
}

void Player::ApplySpeed() {

	if (player.state == jump_ || player.state == jumping_ || player.state == jump2_ || player.state == jumping2_) {
		if (!up)
			speed.y = 0;
		speed.y += player.gravity;
		if (speed.y >= P_GRAVITY && !gravitySpeed) {
			speed.y = P_GRAVITY * dt;
			gravitySpeed = true;
		}
		position.y += speed.y * dt;
	}
	else {
		//no jump
		if (down) { //down == true, there isn't floor
			speed.y += player.gravity;
			if (speed.y >= P_JUMP && !gravitySpeed) {
				speed.y = P_JUMP * dt;
				gravitySpeed = true;
			}
			if (player.state == forward_ || player.state == idle_)
				animation = &player.jump;
			else if (player.state == backward_ || player.state == idle2_)
				animation = &player.jump2;
			position.y += speed.y * dt;
		}
		else if (down == false) {//down == false, there is floor
			speed.y = 0;
			gravitySpeed = false;
		}
	}
}

void Player::ShotRight() {

	if (player.shot.Finished() || player.crouchShot.Finished()) {
		App->audio->PlayFx(3);
		if (player.state == shot_)
			App->particles->AddParticle(App->particles->arrowRight, position.x + SHOT_X, position.y + SHOT_Y, COLLIDER_ARROW, NULL, { 200,0 });
		else if (player.state == crouchShot_)
			App->particles->AddParticle(App->particles->arrowRight, position.x + CROUCH_SHOT_X, position.y + CROUCH_SHOT_Y, COLLIDER_ARROW, NULL, { 200,0 });
		stopshot = true;
		player.shot.Reset();
		player.crouchShot.Reset();
	}
}

void Player::ShotLeft() {

	if (player.shot2.Finished() || player.crouchShot2.Finished()) {
		App->audio->PlayFx(3);
		if (player.state == shot2_)
			App->particles->AddParticle(App->particles->arrowLeft, position.x + SHOT_X + 10, position.y + SHOT_X, COLLIDER_ARROW, NULL, { -200,0 });
		else if (player.state == crouchShot2_)
			App->particles->AddParticle(App->particles->arrowLeft, position.x + CROUCH_SHOT_X, position.y + CROUCH_SHOT_Y, COLLIDER_ARROW, NULL, { -200,0 });
		stopshot = true;
		player.shot2.Reset();
		player.crouchShot2.Reset();
	}
}

void Player::CheckIfDead() {

	if (time >= 1.0f) {
		respawnGOD = true;
		App->scene->god = true;
		player.SetState(idle_);
		time = 0;
	}
}

void Player::PlayerStateMachine() {

	switch (player.state) {

	case idle_:
		checkDash = false;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			player.state = forward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			player.state = backward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			player.state = crouch_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && down == false) {
			player.state = jump_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && animation == &player.idle) {
			player.state = shot_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && animation == &player.idle && !secondAttackToCheck && !thirdAttackToCheck) {
			player.state = firstAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && animation == &player.idle && secondAttackToCheck && !thirdAttackToCheck) {
			player.state = secondAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && animation == &player.idle && thirdAttackToCheck) {
			player.state = thirdAttack_;
			break;
		}

		if (secondAttackToCheck) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				secondAttackToCheck = false;
				time2 = 0;
			}
		}

		if (thirdAttackToCheck) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				thirdAttackToCheck = false;
				time2 = 0;
			}
		}

		animation = &player.idle;
		break;

	case idle2_:
		checkDash = false;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			player.state = forward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			player.state = backward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			player.state = crouch2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && down == false) {
			player.state = jump2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && animation == &player.idle2) {
			player.state = shot2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && animation == &player.idle2 && !secondAttackToCheck2 && !thirdAttackToCheck2) {
			player.state = firstAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && animation == &player.idle2 && secondAttackToCheck2 && !thirdAttackToCheck2) {
			player.state = secondAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && animation == &player.idle2 && thirdAttackToCheck2) {
			player.state = thirdAttack2_;
			break;
		}

		if (secondAttackToCheck2) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				secondAttackToCheck2 = false;
				time2 = 0;
			}
		}


		if (thirdAttackToCheck2) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				thirdAttackToCheck2 = false;
				time2 = 0;
			}
		}
		animation = &player.idle2;
		break;

	case forward_:
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		MoveForward();
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && down == false) {
			player.state = jump_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && animation == &player.forward && !secondAttackToCheck && !thirdAttackToCheck) {
			player.state = firstAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && animation == &player.forward && secondAttackToCheck && !thirdAttackToCheck) {
			player.state = secondAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && animation == &player.forward && thirdAttackToCheck) {
			player.state = thirdAttack_;
			break;
		}

		if (secondAttackToCheck) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				secondAttackToCheck = false;
				time2 = 0;
			}
		}

		if (thirdAttackToCheck) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				thirdAttackToCheck = false;
				time2 = 0;
			}
		}

		animation = &player.forward;
		player.state = idle_;
		break;

	case backward_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		MoveBackward();
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && down == false) {
			player.state = jump2_;
			break;
		}

		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && animation == &player.backward && !secondAttackToCheck2 && !thirdAttackToCheck2) {
			player.state = firstAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && animation == &player.backward && secondAttackToCheck2 && !thirdAttackToCheck2) {
			player.state = secondAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && animation == &player.backward && thirdAttackToCheck2) {
			player.state = thirdAttack2_;
			break;
		}

		if (secondAttackToCheck2) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				secondAttackToCheck2 = false;
				time2 = 0;
			}
		}

		if (thirdAttackToCheck2) {
			time2 += 1.0f * dt;
			if (time2 >= 0.2f) {
				thirdAttackToCheck2 = false;
				time2 = 0;
			}
		}

		animation = &player.backward;
		player.state = idle2_;
		break;

	case jump_:
		speed.y = Jump();
		animation = &player.jump;
		player.state = jumping_;
		break;

	case jump2_:
		speed.y = Jump();
		animation = &player.jump2;
		player.state = jumping2_;
		break;

	case jumping_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			MoveForwardJumping();
			animation = &player.jump;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && animation == &player.jump && !checkDash) {
				player.state = dashForward_;
				break;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			MoveBackwardJumping();
			animation = &player.jump2;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && animation == &player.jump2 && !checkDash) {
				player.state = dashBackward_;
				break;
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && animation == &player.jump && !checkDash) {
			player.state = dashForward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && animation == &player.jump2 && !checkDash) {
			player.state = dashBackward_;
			break;
		}
		else {
			if (animation == &player.jump)
				animation = &player.jump;
			else if (animation == &player.jump2)
				animation = &player.jump2;
			else
				animation = &player.jump;
		}

		if (down == false) {
			if (animation == &player.jump)
				player.state = idle_;
			else if (animation == &player.jump2)
				player.state = idle2_;
			break;
		}
		break;

	case jumping2_:
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			MoveForwardJumping();
			animation = &player.jump;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && animation == &player.jump && !checkDash) {
				player.state = dashForward_;
				break;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			MoveBackwardJumping();
			animation = &player.jump2;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && animation == &player.jump2 && !checkDash) {
				player.state = dashBackward_;
				break;
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && animation == &player.jump2 && !checkDash) {
			player.state = dashBackward_;
			break;
		}

		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && animation == &player.jump && !checkDash) {
			player.state = dashForward_;
			break;
		}
		else {
			if (animation == &player.jump)
				animation = &player.jump;
			else if (animation == &player.jump2)
				animation = &player.jump2;
			else
				animation = &player.jump2;
		}

		if (down == false) {
			if (animation == &player.jump)
				player.state = idle_;
			else if (animation == &player.jump2)
				player.state = idle2_;
			break;
		}
		break;

	case dashForward_:
		if (time == 0)
			App->audio->PlayFx(1);

		animation = &player.dash;
		time += 1.0f * dt;
		DashForward();
		if (time >= 0.4f) {
			checkDash = true;
			animation = &player.jump;
			player.state = jumping_;
			time = 0;
			break;
		}
		break;

	case dashBackward_:
		if (time == 0)
			App->audio->PlayFx(1);

		animation = &player.dash2;
		time += 1.0f * dt;
		DashBackward();
		if (time >= 0.4f) {
			checkDash = true;
			animation = &player.jump2;
			player.state = jumping2_;
			time = 0;
			break;
		}
		break;

	case crouch_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			player.state = crouchShot_;
			break;
		}
		animation = &player.crouch;
		player.state = idle_;
		break;

	case crouch2_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			player.state = crouchShot2_;
			break;
		}
		animation = &player.crouch2;
		player.state = idle2_;
		break;

	case shot_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		animation = &player.shot;
		ShotRight();
		if (stopshot == true) {
			stopshot = false;
			player.state = idle_;
			break;
		}
		break;

	case shot2_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		animation = &player.shot2;
		ShotLeft();
		if (stopshot == true) {
			stopshot = false;
			player.state = idle2_;
			break;
		}
		break;

	case stop_:
		speed.y = 0;
		if (App->fade->GetStep() == 0) {
			player.state = idle_;
			break;
		}
		break;

	case crouchShot_:
		animation = &player.crouchShot;
		ShotRight();
		if (stopshot == true) {
			stopshot = false;
			player.state = idle_;
			break;
		}
		break;

	case crouchShot2_:
		animation = &player.crouchShot2;
		ShotLeft();
		if (stopshot == true) {
			stopshot = false;
			player.state = idle2_;
			break;
		}
		break;

	case firstAttack_:
		if (animation != &player.firstAttack)
			App->particles->AddParticle(App->particles->firstAttack, position.x + P_ATTACK_OFFSET_X, position.y + P_ATTACK_OFFSET_Y, COLLIDER_ARROW, NULL, { 0,0 });
		animation = &player.firstAttack;
		if (right)
			position.x += P_ATTACK_SPEED * dt;
		if (player.firstAttack.Finished()) {
			player.firstAttack.Reset();
			secondAttackToCheck = true;
			player.state = idle_;
			break;
		}
		break;

	case secondAttack_:
		if (animation != &player.secondAttack)
			App->particles->AddParticle(App->particles->secondAttack, position.x + P_ATTACK_OFFSET_X, position.y + P_ATTACK_OFFSET_Y, COLLIDER_ARROW, NULL, { 0,0 });
		secondAttackToCheck = false;
		animation = &player.secondAttack;
		if (right)
			position.x += P_ATTACK_SPEED * dt;
		if (player.secondAttack.Finished()) {
			thirdAttackToCheck = true;
			player.secondAttack.Reset();
			player.state = idle_;
			break;
		}
		break;

	case thirdAttack_:
		if (animation != &player.thirdAttack)
			App->particles->AddParticle(App->particles->thirdAttack, position.x + P_ATTACK_OFFSET_X, position.y + P_ATTACK_OFFSET_Y, COLLIDER_ARROW, NULL, { 0,0 });
		thirdAttackToCheck = false;
		animation = &player.thirdAttack;
		if (right)
			position.x += P_ATTACK_SPEED * dt;
		if (player.thirdAttack.Finished()) {
			player.thirdAttack.Reset();
			player.state = idle_;
			break;
		}
		break;

	case firstAttack2_:
		if (animation != &player.firstAttack2)
			App->particles->AddParticle(App->particles->firstAttack, position.x, position.y + P_ATTACK_OFFSET_Y, COLLIDER_ARROW, NULL, { 0,0 });
		animation = &player.firstAttack2;
		if (left)
			position.x -= P_ATTACK_SPEED * dt;
		if (player.firstAttack2.Finished()) {
			player.firstAttack2.Reset();
			secondAttackToCheck2 = true;
			player.state = idle2_;
			break;
		}
		break;

	case secondAttack2_:
		if (animation != &player.secondAttack2)
			App->particles->AddParticle(App->particles->secondAttack, position.x, position.y + P_ATTACK_OFFSET_Y, COLLIDER_ARROW, NULL, { 0,0 });
		secondAttackToCheck2 = false;
		animation = &player.secondAttack2;
		if (left)
			position.x -= P_ATTACK_SPEED * dt;
		if (player.secondAttack2.Finished()) {
			thirdAttackToCheck2 = true;
			player.secondAttack2.Reset();
			player.state = idle2_;
			break;
		}
		break;

	case thirdAttack2_:
		if (animation != &player.thirdAttack2)
			App->particles->AddParticle(App->particles->thirdAttack, position.x, position.y + P_ATTACK_OFFSET_Y, COLLIDER_ARROW, NULL, { 0,0 });
		thirdAttackToCheck2 = false;
		animation = &player.thirdAttack2;
		if (left)
			position.x -= P_ATTACK_SPEED * dt;
		if (player.thirdAttack2.Finished()) {
			player.thirdAttack2.Reset();
			player.state = idle2_;
			break;
		}
		break;

	case punished_:
		if (!hurt_fx) {
			App->audio->PlayFx(5);
			App->scene->progress_bar->DecreaseLifeProgress(40);
			if (lava_dead) {
				App->scene->progress_bar->SetLifeProgress(0);
				lava_dead = false;
			}
			hurt_fx = true;
		}

		if (animation == &player.forward || animation == &player.jump || animation == &player.punished || animation == &player.idle || animation == &player.crouch || animation == &player.shot || animation == &player.crouchShot)
			animation = &player.punished;
		else if (animation == &player.backward || animation == &player.jump2 || animation == &player.punished2 || animation == &player.idle2 || animation == &player.crouch2 || animation == &player.shot2 || animation == &player.crouchShot2)
			animation = &player.punished2;

		speed.y = 0;
		speed.x = 0;

		if (player.punished.Finished() && animation == &player.punished) {
			hurt_fx = false;
			player.punished.Reset();
			break;
		}
		else if (player.punished2.Finished() && animation == &player.punished2) {
			hurt_fx = false;
			player.punished2.Reset();
			break;
		}
		break;
	}
}

void Player::CheckCollision(iPoint position, iPoint size, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state) {

	BROFILER_CATEGORY("CheckForCollision", Profiler::Color::Azure);

	App->map->culing_offset = CULING_OFFSET;

	for (int i = position.x - App->map->culing_offset; i < position.x + App->map->culing_offset; i++) {
		for (int j = position.y - App->map->culing_offset; j < position.y + App->map->culing_offset; j++) {

			if (!App->trans->back_to_main_menu) {
				iPoint ij = App->map->WorldToMap(i, j);

				uint id = App->map->collisionLayer->Get(ij.x, ij.y);

				if (id != 0) {
					iPoint world = App->map->MapToWorld(ij.x, ij.y);
					CalculateCollision(position, size, world.x, world.y, id, offset, up, down, left, right, state);
				}
			}
		}
	}
}

void Player::CalculateCollision(iPoint position, iPoint size, uint x, uint y, uint id, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state) {

	if (App->toCap && App->capFrames <= 30) {
		offset = 6;
	}

	if (App->toCap && App->capFrames <= 10) {
		offset = 15;
	}

	SDL_Rect B = { x, y, 16, 16 }; //object rectangle

	iPoint c_up = { 0, -offset };
	iPoint c_down = { 0, offset };
	iPoint c_left = { -offset, 0 };
	iPoint c_right = { offset, 0 };

	if (position.x == (int)this->position.x && position.y == (int)this->position.y) {
		if (state == jumping_ || state == jumping2_)
			c_down = { 0, offset + 3 };
	}

	//UP
	SDL_Rect A_up = { position.x + c_up.x, position.y + c_up.y, size.x, size.y }; //player rectangle
	if (SDL_HasIntersection(&A_up, &B)) {
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			up = false;
		else if (id == 1183 && state != null_) {
			player.SetState(punished_);
			lava_dead = true;
		}
	}

	//DOWN
	SDL_Rect A_down = { position.x + c_down.x, position.y + c_down.y, size.x, size.y }; //player rectangle
	if (SDL_HasIntersection(&A_down, &B))
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			down = false;
		else if (id == 1183 && state != null_) {
			player.SetState(punished_);
			lava_dead = true;
		}

	//LEFT
	SDL_Rect A_left = { position.x + c_left.x, position.y + c_left.y, size.x, size.y }; //player rectangle
	if (SDL_HasIntersection(&A_left, &B))
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			left = false;
		else if (id == 1183 && state != null_) {
			player.SetState(punished_);
			lava_dead = true;
		}

	//RIGHT
	SDL_Rect A_right = { position.x + c_right.x, position.y + c_right.y, size.x, size.y }; //player rectangle
	if (SDL_HasIntersection(&A_right, &B))
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			right = false;
		else if (id == 1183 && state != null_) {
			player.SetState(punished_);
			lava_dead = true;
		}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

PlayerInfo::PlayerInfo() {}

PlayerInfo::PlayerInfo(const PlayerInfo& i) :
	idle(i.idle), idle2(i.idle2),
	forward(i.forward), backward(i.backward),
	crouch(i.crouch), crouch2(i.crouch2),
	shot(i.shot), shot2(i.shot2),
	crouchShot(i.crouchShot), crouchShot2(i.crouchShot2),
	jump(i.jump), jump2(i.jump2),
	punished(i.punished), punished2(i.punished2),
	dash(i.dash), dash2(i.dash2),
	firstAttack(i.firstAttack), firstAttack2(i.firstAttack2),
	secondAttack(i.secondAttack), secondAttack2(i.secondAttack2),
	thirdAttack(i.thirdAttack), thirdAttack2(i.thirdAttack2),
	coll_size(i.coll_size), coll_offset(i.coll_offset),
	gravity(i.gravity), speed(i.speed), state(i.state),
	check_collision_offset(i.check_collision_offset)	
{}

PlayerInfo::~PlayerInfo() {}