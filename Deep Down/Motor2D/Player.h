#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Enemy.h"

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

struct PlayerInfo
{
	Animation idle, idle2, forward, backward, jump, jump2, crouch, crouch2, dash, dash2, shot, shot2, crouchShot, crouchShot2, punished, punished2, firstAttack, secondAttack, thirdAttack, firstAttack2, secondAttack2, thirdAttack2;

	iPoint coll_size;
	SDL_Rect coll_offset;

	playerstates state;
	float gravity;
	fPoint speed;

	PlayerInfo();
	PlayerInfo(const PlayerInfo& i);
	~PlayerInfo();

	void SetState(playerstates);
	playerstates GetState() { return state; }

};

class Player : public Enemy
{

public:
	Player(float x, float y, PathInfo* path);
	void OnCollision(Collider* c1, Collider* c2);
	void Move(float dt);

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

	void CheckCollision(iPoint position, iPoint size, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state = null_);
	void CalculateCollision(iPoint position, iPoint size, uint x, uint y, uint id, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state = null_);

	// Textures

public:

	// Collisions
	uint check_collision_offset;

	// General info
	playerstates default_state;

private:
	// Textures
	p2SString spritesheet;

	// Player collider

	// Movement
	
	float time = 0, time2 = 0;
	bool up = true, down = true, left = true, right = true;
	bool checkDash;
	bool stopshot, secondAttackToCheck, thirdAttackToCheck, secondAttackToCheck2, thirdAttackToCheck2;
	float dt;

	// Animations
	PlayerInfo player;
};

#endif