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
}

j1Player::~j1Player()
{
}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config) {

	bool ret = true;

	// Load animations
	pugi::xml_node animations_node = config.child("animations");
	pugi::xml_node node;
	
	//idle
	node = animations_node.child("idle");
	idle.speed = node.attribute("speed").as_float();
	idle.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//idle2
	node = animations_node.child("idle2");
	idle2.speed = node.attribute("speed").as_float();
	idle2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		idle2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//forward
	node = animations_node.child("forward");
	forward.speed = node.attribute("speed").as_float();
	forward.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		forward.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//backward
	node = animations_node.child("backward");
	backward.speed = node.attribute("speed").as_float();
	backward.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		backward.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouch
	node = animations_node.child("crouch");
	crouch.speed = node.attribute("speed").as_float();
	crouch.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		crouch.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouch2
	node = animations_node.child("crouch2");
	crouch2.speed = node.attribute("speed").as_float();
	crouch2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		crouch2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//shot
	node = animations_node.child("shot");
	shot.speed = node.attribute("speed").as_float();
	shot.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		shot.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//shot2
	node = animations_node.child("shot2");
	shot2.speed = node.attribute("speed").as_float();
	shot2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		shot2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouchShot
	node = animations_node.child("crouchShot");
	crouchShot.speed = node.attribute("speed").as_float();
	crouchShot.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		crouchShot.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//crouchShot2
	node = animations_node.child("crouchShot2");
	crouchShot2.speed = node.attribute("speed").as_float();
	crouchShot2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		crouchShot2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//jump
	node = animations_node.child("jump");
	jump.speed = node.attribute("speed").as_float();
	jump.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		jump.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//jump2
	node = animations_node.child("jump2");
	jump2.speed = node.attribute("speed").as_float();
	jump2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		jump2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//punished
	node = animations_node.child("punished");
	punished.speed = node.attribute("speed").as_float();
	punished.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		punished.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//punished2
	node = animations_node.child("punished2");
	punished2.speed = node.attribute("speed").as_float();
	punished2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		punished2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//dash
	node = animations_node.child("dash");
	dash.speed = node.attribute("speed").as_float();
	dash.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		dash.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//dash2
	node = animations_node.child("dash2");
	dash2.speed = node.attribute("speed").as_float();
	dash2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		dash2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//firstAttack
	node = animations_node.child("firstAttack");
	firstAttack.speed = node.attribute("speed").as_float();
	firstAttack.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		firstAttack.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//secondAttack
	node = animations_node.child("secondAttack");
	secondAttack.speed = node.attribute("speed").as_float();
	secondAttack.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		secondAttack.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//thirdAttack
	node = animations_node.child("thirdAttack");
	thirdAttack.speed = node.attribute("speed").as_float();
	thirdAttack.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		thirdAttack.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//firstAttack2
	node = animations_node.child("firstAttack2");
	firstAttack2.speed = node.attribute("speed").as_float();
	firstAttack2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		firstAttack2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//secondAttack2
	node = animations_node.child("secondAttack2");
	secondAttack2.speed = node.attribute("speed").as_float();
	secondAttack2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		secondAttack2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//thirdAttack2
	node = animations_node.child("thirdAttack2");
	thirdAttack2.speed = node.attribute("speed").as_float();
	thirdAttack2.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		thirdAttack2.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}
	//_load_animations

	// Load textures path
	spritesheet = config.child("spritesheets").child("spritesheet").attribute("name").as_string();
	//_load_textures_path

	// Load general info
	pugi::xml_node general_node = config.child("general");

	default_state = (playerstates)general_node.child("state").attribute("value").as_int();
	gravity = general_node.child("gravity").attribute("value").as_float();
	speed = { general_node.child("speed").attribute("x").as_float(),  general_node.child("speed").attribute("y").as_float() };
	coll_offset = { general_node.child("collider").attribute("x").as_int(),  general_node.child("collider").attribute("y").as_int() };
	coll_size = { general_node.child("collider").attribute("w").as_int(),  general_node.child("collider").attribute("h").as_int() };
	check_collision_offset = general_node.child("check_collision").attribute("offset").as_uint();
	//_load_general_info

	return ret;
}

// Load assets
bool j1Player::Start()
{
	// Set player animation (state)
	SetState(default_state);

	// Create player collider
	colliderPos = { (int)position.x + coll_offset.x, (int)position.y + coll_offset.y };
	coll = App->collision->AddCollider({ colliderPos.x, colliderPos.y, coll_size.x, coll_size.y }, COLLIDER_PLAYER, this);

	// Load player textures
	LOG("Loading player textures");
	player = App->tex->Load(spritesheet.GetString());

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
	up = true;
	down = true;
	left = true;
	right = true;
	CheckCollision(colliderPos, coll_size, check_collision_offset, up, down, left, right, GetState());

	CheckIfDead();

	PlayerStateMachine();

	ApplySpeed();

	r = &current_animation->GetCurrentFrame();

	// Update collider
	colliderPos = { (int)position.x + coll_offset.x, (int)position.y + coll_offset.y };
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
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && current_animation == &idle && !secondAttackToCheck && !thirdAttackToCheck) {
			state = firstAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && current_animation == &idle && secondAttackToCheck && !thirdAttackToCheck) {
			state = secondAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && current_animation == &idle && thirdAttackToCheck) {
			state = thirdAttack_;
			break;
		}

		if (secondAttackToCheck) {
			time2++;
			if (time2 >= 50) {
				secondAttackToCheck = false;
				time2 = 0;
			}
		}


		if (thirdAttackToCheck) {
			time2++;
			if (time2 >= 50) {
				thirdAttackToCheck = false;
				time2 = 0;
			}
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
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && current_animation == &idle2 && !secondAttackToCheck2 && !thirdAttackToCheck2) {
			state = firstAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && current_animation == &idle2 && secondAttackToCheck2 && !thirdAttackToCheck2) {
			state = secondAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && current_animation == &idle2 && thirdAttackToCheck2) {
			state = thirdAttack2_;
			break;
		}

		if (secondAttackToCheck2) {
			time2++;
			if (time2 >= 50) {
				secondAttackToCheck2 = false;
				time2 = 0;
			}
		}


		if (thirdAttackToCheck2) {
			time2++;
			if (time2 >= 50) {
				thirdAttackToCheck2 = false;
				time2 = 0;
			}
		}
		current_animation = &idle2;
		break;

	case forward_:
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		MoveForward();
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && down == false) {
			state = jump_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && current_animation == &forward && !secondAttackToCheck && !thirdAttackToCheck) {
			state = firstAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && current_animation == &forward && secondAttackToCheck && !thirdAttackToCheck) {
			state = secondAttack_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && current_animation == &forward && thirdAttackToCheck) {
			state = thirdAttack_;
			break;
		}

		if (secondAttackToCheck) {
			time2++;
			if (time2 >= 50) {
				secondAttackToCheck = false;
				time2 = 0;
			}
		}


		if (thirdAttackToCheck) {
			time2++;
			if (time2 >= 50) {
				thirdAttackToCheck = false;
				time2 = 0;
			}
		}

		current_animation = &forward;
		state = idle_;
		break;

	case backward_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		MoveBackward();
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && down == false) {
			state = jump2_;
			break;
		}

		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && current_animation == &backward && !secondAttackToCheck2 && !thirdAttackToCheck2) {
			state = firstAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && current_animation == &backward && secondAttackToCheck2 && !thirdAttackToCheck2) {
			state = secondAttack2_;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && current_animation == &backward && thirdAttackToCheck2) {
			state = thirdAttack2_;
			break;
		}

		if (secondAttackToCheck2) {
			time2++;
			if (time2 >= 50) {
				secondAttackToCheck2 = false;
				time2 = 0;
			}
		}


		if (thirdAttackToCheck2) {
			time2++;
			if (time2 >= 50) {
				thirdAttackToCheck2 = false;
				time2 = 0;
			}
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
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
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
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			state = crouchShot_;
			break;
		}
		current_animation = &crouch;
		state = idle_;
		break;

	case crouch2_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			state = crouchShot2_;
			break;
		}
		current_animation = &crouch2;
		state = idle2_;
		break;

	case shot_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
		current_animation = &shot;
		ShotRight();
		if (stopshot == true) {
			stopshot = false;
			state = idle_;
			break;
		}
		break;

	case shot2_:
		secondAttackToCheck = false;
		thirdAttackToCheck = false;
		secondAttackToCheck2 = false;
		thirdAttackToCheck2 = false;
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

	case firstAttack_:
		if (current_animation != &firstAttack)
		App->particles->AddParticle(App->particles->firstAttack, position.x + 35, position.y + 20, COLLIDER_ARROW, NULL, { 0,0 });
		current_animation = &firstAttack;
		if (right)
			position.x += 0.1f;
		if (firstAttack.Finished()) {
			firstAttack.Reset();
			secondAttackToCheck = true;
			state = idle_;
			break;
		}
		break;

	case secondAttack_:
		if (current_animation != &secondAttack)
			App->particles->AddParticle(App->particles->secondAttack, position.x + 35, position.y + 20, COLLIDER_ARROW, NULL, { 0,0 });
		secondAttackToCheck = false;
		current_animation = &secondAttack;
		if (right)
			position.x += 0.1f;
		if (secondAttack.Finished()) {
			thirdAttackToCheck = true;
			secondAttack.Reset();
			state = idle_;
			break;
		}
		break;

	case thirdAttack_:
		if (current_animation != &thirdAttack)
			App->particles->AddParticle(App->particles->thirdAttack, position.x + 35, position.y + 20, COLLIDER_ARROW, NULL, { 0,0 });
		thirdAttackToCheck = false;
		current_animation = &thirdAttack;
		if(right)
			position.x += 0.1f;
		if (thirdAttack.Finished()) {
			thirdAttack.Reset();
			state = idle_;
			break;
		}
		break;

	case firstAttack2_:
		if (current_animation != &firstAttack2)
			App->particles->AddParticle(App->particles->firstAttack, position.x, position.y + 20, COLLIDER_ARROW, NULL, { 0,0 });
		current_animation = &firstAttack2;
		if(left)
		position.x -= 0.1f;
		if (firstAttack2.Finished()) {
			firstAttack2.Reset();
			secondAttackToCheck2 = true;
			state = idle2_;
			break;
		}
		break;

	case secondAttack2_:
		if (current_animation != &secondAttack2)
			App->particles->AddParticle(App->particles->secondAttack, position.x, position.y + 20, COLLIDER_ARROW, NULL, { 0,0 });
		secondAttackToCheck2 = false;
		current_animation = &secondAttack2;
		if (left)
			position.x -= 0.1f;
		if (secondAttack2.Finished()) {
			thirdAttackToCheck2 = true;
			secondAttack2.Reset();
			state = idle2_;
			break;
		}
		break;

	case thirdAttack2_:
		if (current_animation != &thirdAttack2)
			App->particles->AddParticle(App->particles->thirdAttack, position.x, position.y + 20, COLLIDER_ARROW, NULL, { 0,0 });
		thirdAttackToCheck2 = false;
		current_animation = &thirdAttack2;
		if (left)
			position.x -= 0.1f;
		if (thirdAttack2.Finished()) {
			thirdAttack2.Reset();
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
		if (punished.Finished() && current_animation == &punished) {
			state = idle_;
			punished.Reset();
			break;
		}
		else if (punished2.Finished() && current_animation == &punished2) {
			state = idle2_;
			punished.Reset();
			break;
		}
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

	if ((a->type == COLLIDER_PEASANT_SHOT && b->type == COLLIDER_PLAYER) || (a->type == COLLIDER_PLAYER && b->type == COLLIDER_PEASANT_SHOT)) {

		App->player->SetState(punished_);

	}

	if ((a->type == COLLIDER_CATPEASANT && b->type == COLLIDER_PLAYER) || (a->type == COLLIDER_PLAYER && b->type == COLLIDER_CATPEASANT)) {

		App->player->SetState(punished_);

	}

	if ((a->type == COLLIDER_IMP && b->type == COLLIDER_PLAYER) || (a->type == COLLIDER_PLAYER && b->type == COLLIDER_IMP)) {

		App->player->SetState(punished_);

	}
}

void j1Player::CheckCollision(iPoint position, iPoint size, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state) {

	for (int i = 0; i < App->map->collisionLayer->width; i++) {
		for (int j = 0; j < App->map->collisionLayer->height; j++) {

			uint id = App->map->collisionLayer->Get(i, j);

			if (id != 0) {
				iPoint world = App->map->MapToWorld(i, j);
				CalculateCollision(position, size, world.x, world.y, id, offset, up, down, left, right, state);
			}
		}
	}
}

void j1Player::CalculateCollision(iPoint position, iPoint size, uint x, uint y, uint id, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state) {

	SDL_Rect B = { x, y, 16, 16 }; //object rectangle
	SDL_Rect result = { 0, 0, 0, 0 };

	iPoint c_up = { 0, -offset };
	iPoint c_down = { 0, offset };
	iPoint c_left = { -offset, 0 };
	iPoint c_right = { offset, 0 };

	//UP
	SDL_Rect A_up = { position.x + c_up.x, position.y + c_up.y, size.x, size.y }; //player rectangle
	if (SDL_IntersectRect(&A_up, &B, &result)) {
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			up = false;
		else if (id == 1183 && state != null_)
			SetState(punished_);
	}

	//DOWN
	SDL_Rect A_down = { position.x + c_down.x, position.y + c_down.y, size.x, size.y }; //player rectangle
	if (SDL_IntersectRect(&A_down, &B, &result))
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			down = false;
		else if (id == 1183 && state != null_)
			SetState(punished_);

	//LEFT
	SDL_Rect A_left = { position.x + c_left.x, position.y + c_left.y, size.x, size.y }; //player rectangle
	if (SDL_IntersectRect(&A_left, &B, &result))
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			left = false;
		else if (id == 1183 && state != null_)
			SetState(punished_);

	//RIGHT
	SDL_Rect A_right = { position.x + c_right.x, position.y + c_right.y, size.x, size.y }; //player rectangle
	if (SDL_IntersectRect(&A_right, &B, &result))
		if (id == 1181 || (id == 1182 && App->scene->gate == false))
			right = false;
		else if (id == 1183 && state != null_)
			SetState(punished_);
}