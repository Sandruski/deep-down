#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"

//struct SDL_Texture;

enum playerstates {
	null_,
	stop_,
	idle_,
	idle2_,
	shot_,
	shot2_,
	crouchShot_,
	crouchShot2_,
	forward_,
	backward_,
	jump_,
	jump2_,
	jumping_,
	jumping2_,
	crouch_,
	crouch2_,
	dashForward_,
	dashBackward_,
	punished_,
	firstAttack_,
	secondAttack_,
	thirdAttack_,
	firstAttack2_,
	secondAttack2_,
	thirdAttack2_,
};

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	// Save
	bool Save(pugi::xml_node&) const;

	// Load
	bool Load(pugi::xml_node&);

	void PlayerStateMachine();

	void DashForward();
	void DashBackward();
	void ShotRight();
	void ShotLeft();
	void MoveForward();
	void MoveBackward();
	void MoveForwardJumping();
	void MoveBackwardJumping();
	float Jump();
	void ApplySpeed();
	void CheckIfDead();

	void SetState(playerstates);
	playerstates GetState() { return state; }

	void OnCollision(Collider* a, Collider* b);

	void CheckCollision(iPoint position, iPoint size, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state = null_);
	void CalculateCollision(iPoint position, iPoint size, uint x, uint y, uint id, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state = null_);

public:

	fPoint startPos = { 0,0 };
	fPoint position = { 0,0 };
	iPoint colliderPos = { 0,0 };
	iPoint size = { 0,0 };

	fPoint speed;
	SDL_Rect* r;
	SDL_Texture* player;

	bool up = true, down = true, left = true, right = true;

private:
	// Collision stuff
	Collider* coll;

	// Movement
	float gravity;
	float time = 0, time2 = 0;
	bool checkDash;
	bool stopshot, secondAttackToCheck, thirdAttackToCheck, secondAttackToCheck2, thirdAttackToCheck2;

	Animation* current_animation;
	Animation idle, idle2, forward, backward, jump, jump2, crouch, crouch2, dash, dash2, shot, shot2, crouchShot, crouchShot2, punished, punished2, firstAttack, secondAttack, thirdAttack, firstAttack2, secondAttack2, thirdAttack2;
	playerstates state;
};

#endif // _j1PLAYER_H_