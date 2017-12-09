#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"

enum playerstates {
	null_,
	stop_,
	idle_, idle2_,
	shot_, shot2_,
	crouchShot_, crouchShot2_,
	forward_, backward_,
	jump_, jump2_,
	jumping_, jumping2_,
	crouch_, crouch2_,
	dashForward_, dashBackward_,
	firstAttack_, firstAttack2_,
	secondAttack_, secondAttack2_,
	thirdAttack_, thirdAttack2_,
	punished_,
};

struct PlayerInfo
{
	Animation idle, idle2;
	Animation forward, backward;
	Animation jump, jump2;
	Animation crouch, crouch2;
	Animation dash, dash2;
	Animation shot, shot2;
	Animation crouchShot, crouchShot2;
	Animation punished, punished2;
	Animation firstAttack, firstAttack2;
	Animation secondAttack, secondAttack2;
	Animation thirdAttack, thirdAttack2;

	iPoint coll_size = { 0,0 };
	SDL_Rect coll_offset = { 0,0,0,0 };
	uint check_collision_offset = 0;

	playerstates state = null_;
	float gravity = 0.0f;
	fPoint speed = { 0,0 };

	PlayerInfo();
	PlayerInfo(const PlayerInfo& i);
	~PlayerInfo();

	void SetState(playerstates);
	playerstates GetState() { return state; }
};

class Player : public Entity
{

public:
	Player(float x, float y, PathInfo* path);
	void OnCollision(Collider* c1, Collider* c2);
	void Move(float dt);
	void UpdateAnimations(float dt);
	void LoadAnimationsSpeed();

	void PlayerStateMachine();

	void DashForward();
	void DashBackward();
	void ShotRight();
	void ShotLeft();
	void MoveForward();
	void MoveBackward();
	void MoveForwardJumping();
	void MoveBackwardJumping();
	float Jump() const;
	void ApplySpeed();
	void CheckIfDead();
	void GodMode();

	void CheckCollision(iPoint position, iPoint size, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state = null_);
	void CalculateCollision(iPoint position, iPoint size, uint x, uint y, uint id, int offset, bool &up, bool &down, bool &left, bool &right, playerstates state = null_);

public:

	// General info
	playerstates default_state = null_;
	Animation* animationPlayer = nullptr;

private:

	// Movement
	float time = 0.0f, time2 = 0.0f;
	bool up = true, down = true, left = true, right = true, gravitySpeed = false;
	bool checkDash = false;
	bool stopshot = false, secondAttackToCheck = false, thirdAttackToCheck = false, secondAttackToCheck2 = false, thirdAttackToCheck2 = false;
	float dt = 0.0f;
	bool respawnGOD = false;
	float timeRespawn = 0.0f;

	// Animations speed
	float idle_speed = 0.0f, idle2_speed = 0.0f;
	float forward_speed = 0.0f, backward_speed = 0.0f;
	float jump_speed = 0.0f, jump2_speed = 0.0f;
	float crouch_speed = 0.0f, crouch2_speed = 0.0f;
	float dash_speed = 0.0f, dash2_speed = 0.0f;
	float shot_speed = 0.0f, shot2_speed = 0.0f;
	float crouchShot_speed = 0.0f, crouchShot2_speed = 0.0f;
	float punished_speed = 0.0f, punished2_speed = 0.0f;
	float firstAttack_speed = 0.0f, firstAttack2_speed = 0.0f;
	float secondAttack_speed = 0.0f, secondAttack2_speed = 0.0f;
	float thirdAttack_speed = 0.0f, thirdAttack2_speed = 0.0f;

	bool hurt_fx = false;

public:
	PlayerInfo player;
};

#endif