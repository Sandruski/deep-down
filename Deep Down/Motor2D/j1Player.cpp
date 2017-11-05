#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"

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
#include "j1Player.h"

#include <math.h>

j1Player::j1Player()
{
	name.create("player");

	idle.PushBack({ 1, 454, 48, 48 });
	idle.PushBack({ 50, 454, 48, 48 });
	idle.PushBack({ 99, 454, 48, 48 });
	idle.PushBack({ 148, 454, 48, 48 });
	idle.PushBack({ 197, 454, 48, 48 });
	idle.PushBack({ 246, 454, 48, 48 });
	idle.speed = 0.2f;

	idle2.PushBack({ 686, 454, 48, 48 });
	idle2.PushBack({ 735, 454, 48, 48 });
	idle2.PushBack({ 784, 454, 48, 48 });
	idle2.PushBack({ 833, 454, 48, 48 });
	idle2.PushBack({ 882, 454, 48, 48 });
	idle2.PushBack({ 931, 454, 48, 48 });
	idle2.speed = 0.2f;

	//forward.PushBack({ 1, 206, 48, 48 });
	//forward.PushBack({ 50, 206, 48, 48 });
	forward.PushBack({ 99, 206, 48, 48 });
	forward.PushBack({ 148, 206, 48, 48 });
	forward.PushBack({ 197, 206, 48, 48 });
	forward.PushBack({ 246, 206, 48, 48 });
	forward.PushBack({ 295, 206, 48, 48 });
	forward.PushBack({ 344, 206, 48, 48 });
	forward.PushBack({ 393, 206, 48, 48 });
	forward.PushBack({ 442, 206, 48, 48 });
	forward.speed = 0.2f;

	//backward.PushBack({ 931, 206, 48, 48 });
	//backward.PushBack({ 882, 206, 48, 48 });
	backward.PushBack({ 833, 206, 48, 48 });
	backward.PushBack({ 784, 206, 48, 48 });
	backward.PushBack({ 735, 206, 48, 48 });
	backward.PushBack({ 686, 206, 48, 48 });
	backward.PushBack({ 637, 206, 48, 48 });
	backward.PushBack({ 588, 206, 48, 48 });
	backward.PushBack({ 539, 206, 48, 48 });
	backward.PushBack({ 490, 206, 48, 48 });
	backward.speed = 0.2f;

	crouch.PushBack({ 99, 268, 48, 48 });
	crouch.PushBack({ 148, 268, 48, 48 });
	crouch.speed = 0.2f;

	crouch2.PushBack({ 784, 268, 48, 48 });
	crouch2.PushBack({ 833, 268, 48, 48 });
	crouch2.speed = 0.2f;

	shot.PushBack({ 1, 13, 48, 48 });
	shot.PushBack({ 50, 13, 48, 48 });
	shot.PushBack({ 99, 13, 48, 48 });
	shot.PushBack({ 148, 13, 48, 48 });
	shot.PushBack({ 197, 13, 48, 48 });
	shot.PushBack({ 246, 13, 48, 48 });
	shot.speed = 0.2f;

	shot2.PushBack({ 931, 13, 48, 48 });
	shot2.PushBack({ 882, 13, 48, 48 });
	shot2.PushBack({ 833, 13, 48, 48 });
	shot2.PushBack({ 784, 13, 48, 48 });
	shot2.PushBack({ 735, 13, 48, 48 });
	shot2.PushBack({ 686, 13, 48, 48 });
	shot2.speed = 0.2f;

	crouchShot.PushBack({ 1, 140, 48, 48 });
	crouchShot.PushBack({ 50, 140, 48, 48 });
	crouchShot.PushBack({ 99, 140, 48, 48 });
	crouchShot.PushBack({ 148, 140, 48, 48 });
	crouchShot.PushBack({ 197, 140, 48, 48 });
	crouchShot.PushBack({ 246, 140, 48, 48 });
	crouchShot.speed = 0.2f;

	crouchShot2.PushBack({ 931, 140, 48, 48 });
	crouchShot2.PushBack({ 882, 140, 48, 48 });
	crouchShot2.PushBack({ 833, 140, 48, 48 });
	crouchShot2.PushBack({ 784, 140, 48, 48 });
	crouchShot2.PushBack({ 735, 140, 48, 48 });
	crouchShot2.PushBack({ 686, 140, 48, 48 });
	crouchShot2.speed = 0.2f;

	jump.PushBack({ 1, 390, 48, 48 });
	jump.PushBack({ 50, 390, 48, 48 });
	jump.PushBack({ 99, 390, 48, 48 });
	jump.speed = 0.2f;

	jump2.PushBack({ 833, 390, 48, 48 });
	jump2.PushBack({ 882, 390, 48, 48 });
	jump2.PushBack({ 931, 390, 48, 48 });
	jump2.speed = 0.2f;

	punished.PushBack({ 51, 332, 48, 44 });
	punished.PushBack({ 101, 332, 48, 44 });
	punished.PushBack({ 151, 332, 48, 44 });
	punished.speed = 0.1f;

	punished2.PushBack({ 780, 332, 48, 44 });
	punished2.PushBack({ 830, 332, 48, 44 });
	punished2.PushBack({ 880, 332, 48, 44 });
	punished2.speed = 0.1f;

	dash.PushBack({ 1, 518, 48, 48 });
	dash.speed = 0.2f;

	dash2.PushBack({ 931, 518, 48, 48 });
	dash.speed = 0.2f;
}

j1Player::~j1Player()
{
}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config) {
	bool ret = true;

	return ret;
}

// Load assets
bool j1Player::Start()
{
	// Load FX
	App->audio->LoadFx("dash.wav"); ///id == 1
	App->audio->LoadFx("earthquake.wav"); ///id == 2
	App->audio->LoadFx("bow.wav"); ///id == 3
	App->audio->LoadFx("gate.wav"); ///id == 4
	App->audio->LoadFx("scream.wav"); ///id == 5
	//

	SetState(idle_);
	gravity = 0.1f;

	size = { 20, 38 };
	colliderPos = { (int)position.x + 14, (int)position.y + 10 };

	coll = App->collision->AddCollider({ colliderPos.x, colliderPos.y, size.x, size.y }, COLLIDER_PLAYER, this);

	speed = { 0,0 };

	LOG("Loading player textures");
	player = App->tex->Load("Assets/Sprites/Textures/Kaho.png");

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(player);

	// Delete coll
	if (coll != nullptr)
		coll->to_delete = true;

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	// Check for collisions
	if (App->fade->GetStep() == 0) {
		App->map->CheckNextTile(colliderPos, size, 0, 2, left, right, up, down);
		App->map->CheckTile(colliderPos, size, 10, left, right, up, down);
	}

	CheckIfDead();

	PlayerStateMachine();

	ApplySpeed();

	r = &current_animation->GetCurrentFrame();

	// Update collider
	colliderPos = { (int)position.x + 14, (int)position.y + 10 };
	coll->SetPos(colliderPos.x, colliderPos.y);

	return true;
}

void j1Player::MoveForward() {
	if (right) {
		position.x += 2.0f;
	}
}

void j1Player::MoveBackward() {
	if (left) {
		position.x -= 2.0f;
	}
}

void j1Player::MoveForwardJumping() {
	if (right) {
		position.x += 1.0f;
	}
}
void j1Player::MoveBackwardJumping() {
	if (left) {
		position.x -= 1.0f;
	}
}

float j1Player::Jump() {
	return -3;
}

void j1Player::SetState(playerstates state) {
	this->state = state;
}

void j1Player::DashForward() {
	if (right)
		position.x += 2.0f;
	
	speed.y = 0;
}

void j1Player::DashBackward() {
	if (left)
		position.x -= 2.0f;

	speed.y = 0;
}

void j1Player::ApplySpeed() {

	if (state == jump_ || state == jumping_ || state == jump2_ || state == jumping2_) {
		if (!up)
			speed.y = 0;
			speed.y += gravity;
		if (speed.y >= 2.0f)
			speed.y = 2.0f;	
		position.y += speed.y;
	}
	else {
		//no jump
		if (down) { //down == true, there isn't floor
			speed.y += gravity;
			if (speed.y >= 2.0f)
				speed.y = 2.0f;
			if (state == forward_ || state == idle_)
				current_animation = &jump;
			else if (state == backward_ || state == idle2_)
				current_animation = &jump2;
			position.y += speed.y;
		}
		else if (down == false) //down == false, there is floor
			speed.y = 0;
	}
}

void j1Player::ShotRight() {
	time += 0.1f;

	if (time >= 1.2f) {
		App->audio->PlayFx(3);
		if (state == shot_)
			App->particles->AddParticle(App->particles->arrowRight, position.x + 5, position.y + 22, COLLIDER_ARROW, NULL, { 2,0 });
		else if (state == crouchShot_)
			App->particles->AddParticle(App->particles->arrowRight, position.x + 5, position.y + 38, COLLIDER_ARROW, NULL, { 2,0 });
		stopshot = true;
		time = 0;
	}
}

void j1Player::ShotLeft() {
	time += 0.1f;

	if (time >= 1.2f) {
		App->audio->PlayFx(3);
		if (state == shot2_)
			App->particles->AddParticle(App->particles->arrowLeft, position.x + 15, position.y + 22, COLLIDER_ARROW, NULL, { -2,0 });
		else if (state == crouchShot2_)
			App->particles->AddParticle(App->particles->arrowLeft, position.x + 5, position.y + 38, COLLIDER_ARROW, NULL, { -2,0 });
		stopshot = true;
		time = 0;
	}
}

void j1Player::CheckIfDead() {
	if (App->player->GetState() == punished_)
		time += 0.1f;

	if (time >= 3.0f) {
		App->player->position = App->player->startPos;
		App->player->SetState(idle_);
		time = 0;
	}
}

void j1Player::PlayerStateMachine() {

	switch (state) {

	case idle_:
		checkDash = false;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			state = forward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			state = backward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			state = crouch_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && down == false) {
			state = jump_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && current_animation == &idle) {
			state = shot_;
			break;
		}
		current_animation = &idle;
		break;

	case idle2_:
		checkDash = false;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			state = forward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			state = backward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			state = crouch2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && down == false) {
			state = jump2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && current_animation == &idle2) {
			state = shot2_;
			break;
		}
		current_animation = &idle2;
		break;

	case forward_:
		MoveForward();
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && down == false) {
			state = jump_;
			break;
		}
		current_animation = &forward;
		state = idle_;
		break;

	case backward_:
		MoveBackward();
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && down == false) {
			state = jump2_;
			break;
		}
		current_animation = &backward;
		state = idle2_;
		break;

	case jump_:
		speed.y = Jump();
		current_animation = &jump;
		state = jumping_;
		break;

	case jump2_:
		speed.y = Jump();
		current_animation = &jump2;
		state = jumping2_;
		break;

	case jumping_:
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			MoveForwardJumping();
			current_animation = &jump;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && current_animation == &jump && !checkDash) {
				state = dashForward_;
				break;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			MoveBackwardJumping();
			current_animation = &jump2;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && current_animation == &jump2 && !checkDash) {
				state = dashBackward_;
				break;
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && current_animation == &jump && !checkDash) {
			state = dashForward_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && current_animation == &jump2 && !checkDash) {
			state = dashBackward_;
			break;
		}
		else {
			if (current_animation == &jump)
				current_animation = &jump;
			else if (current_animation == &jump2)
				current_animation = &jump2;
			else
				current_animation = &jump;
		}

		if (down == false) {
			if (current_animation == &jump)
				state = idle_;
			else if (current_animation == &jump2)
				state = idle2_;
			break;
		}
		break;

	case jumping2_:
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right) {
			MoveForwardJumping();
			current_animation = &jump;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && current_animation == &jump && !checkDash) {
				state = dashForward_;
				break;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left) {
			MoveBackwardJumping();
			current_animation = &jump2;

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && current_animation == &jump2 && !checkDash) {
				state = dashBackward_;
				break;
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && left && current_animation == &jump2 && !checkDash) {
			state = dashBackward_;
			break;
		}

		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && right && current_animation == &jump && !checkDash) {
			state = dashForward_;
			break;
		}
		else {
			if (current_animation == &jump)
				current_animation = &jump;
			else if (current_animation == &jump2)
				current_animation = &jump2;
			else
				current_animation = &jump2;
		}

		if (down == false) {
			if (current_animation == &jump)
				state = idle_;
			else if (current_animation == &jump2)
				state = idle2_;
			break;
		}
		break;

	case dashForward_:
		if (time == 0)
			App->audio->PlayFx(1);

		current_animation = &dash;
		time += 0.1f;
		DashForward();
		if (time >= 2) {
			checkDash = true;
			current_animation = &jump;
			state = jumping_;
			time = 0;
			break;
		}
		break;

	case dashBackward_:
		if (time == 0)
			App->audio->PlayFx(1);

		current_animation = &dash2;
		time += 0.1f;
		DashBackward();
		if (time >= 2) {
			checkDash = true;
			current_animation = &jump2;
			state = jumping2_;
			time = 0;
			break;
		}
		break;

	case crouch_:
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			state = crouchShot_;
			break;
		}
		current_animation = &crouch;
		state = idle_;
		break;

	case crouch2_:
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			state = crouchShot2_;
			break;
		}
		current_animation = &crouch2;
		state = idle2_;
		break;

	case shot_:
		current_animation = &shot;
		ShotRight();
		if (stopshot == true) {
			stopshot = false;
			state = idle_;
			break;
		}
		break;

	case shot2_:
		current_animation = &shot2;
		ShotLeft();
		if (stopshot == true) {
			stopshot = false;
			state = idle2_;
			break;
		}
		break;

	case stop_:
		speed.y = 0;
		if (App->fade->GetStep() == 0) {
			state = idle_;
			break;
		}
		break;

	case crouchShot_:
		current_animation = &crouchShot;
		ShotRight();
		if (stopshot == true) {
			stopshot = false;
			state = idle_;
			break;
		}
		break;

	case crouchShot2_:
		current_animation = &crouchShot2;
		ShotLeft();
		if (stopshot == true) {
			stopshot = false;
			state = idle2_;
			break;
		}
		break;

	case punished_:
		if (current_animation == &forward || current_animation == &jump || current_animation == &punished)
			current_animation = &punished;
		else if (current_animation == &backward || current_animation == &jump2 || current_animation == &punished2)
			current_animation = &punished2;
		speed.y = 0;
		speed.x = 0;
		break;
	}
}

// Save
bool j1Player::Save(pugi::xml_node& save) const {
	bool ret = false;

	if (save.child("position") == NULL) {
		save.append_child("position").append_attribute("position.x") = position.x;
		save.child("position").append_attribute("position.y") = position.y;
	}
	else {
		save.child("position").attribute("position.x") = position.x;
		save.child("position").attribute("position.y") = position.y;
	}

	if (save.child("state") == NULL) {
		save.append_child("state").append_attribute("state") = state;
	}
	else {
		save.child("state").attribute("state") = state;
	}

	if (save.child("speed") == NULL) {
		save.append_child("speed").append_attribute("speed.x") = speed.x;
		save.child("speed").append_attribute("speed.y") = speed.y;
	}
	else {
		save.child("speed").attribute("speed.x") = speed.x;
		save.child("speed").attribute("speed.y") = speed.y;
	}

	ret = true;
	return ret;
}

// Load
bool j1Player::Load(pugi::xml_node& save) {
	bool ret = false;

	if (save.child("position") != NULL) {
		position.x = save.child("position").attribute("position.x").as_float();
		position.y = save.child("position").attribute("position.y").as_float();
	}

	if (save.child("state") != NULL)
		state = (playerstates)save.child("state").attribute("state").as_int();

	if (save.child("speed") != NULL) {
		speed.x = save.child("speed").attribute("speed.x").as_float();
		speed.y = save.child("speed").attribute("speed.y").as_float();
	}

	ret = true;
	return ret;
}

void j1Player::OnCollision(Collider* a, Collider* b) {

	if ((a->type == COLLIDER_PLAYER && b->type == COLLIDER_BOSS) || (a->type == COLLIDER_BOSS && b->type == COLLIDER_PLAYER)) {
		App->player->SetState(punished_);
		App->audio->PlayFx(5);
		App->scene->bossPosition = { App->player->position.x, 1200 };
	}
}