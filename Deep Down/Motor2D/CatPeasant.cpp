#include "j1App.h"
#include "j1Enemies.h"
#include "CatPeasant.h"

#include "j1Collision.h"
#include "j1Player.h"
#include "j1Pathfinding.h"

#include "SDL/include/SDL_timer.h"

CatPeasant::CatPeasant(int x, int y) : Enemy(x, y)
{
	idle.PushBack({ 2, 21, 64, 64 });
	idle.PushBack({ 68, 21, 64, 64 });
	idle.PushBack({ 134, 21, 64, 64 });
	idle.PushBack({ 200, 21, 64, 64 });
	idle.PushBack({ 266, 21, 64, 64 });
	idle.speed = 0.15f;

	idle2.PushBack({ 774, 992, 64, 64 });
	idle2.PushBack({ 708, 992, 64, 64 });
	idle2.PushBack({ 642, 992, 64, 64 });
	idle2.PushBack({ 576, 992, 64, 64 });
	idle2.PushBack({ 510, 992, 64, 64 });
	idle2.speed = 0.15f;

	idleNoStuff.PushBack({ 2, 563, 64, 64 });
	idleNoStuff.PushBack({ 68, 563, 64, 64 });
	idleNoStuff.PushBack({ 134, 563, 64, 64 });
	idleNoStuff.PushBack({ 200, 563, 64, 64 });
	idleNoStuff.PushBack({ 266, 563, 64, 64 });
	idleNoStuff.speed = 0.15f;

	idleNoStuff2.PushBack({ 774, 1464, 64, 64 });
	idleNoStuff2.PushBack({ 708, 1464, 64, 64 });
	idleNoStuff2.PushBack({ 642, 1464, 64, 64 });
	idleNoStuff2.PushBack({ 576, 1464, 64, 64 });
	idleNoStuff2.PushBack({ 510, 1464, 64, 64 });
	idleNoStuff2.speed = 0.15f;

	throwStuff.PushBack({ 2, 361, 64, 64 });
	throwStuff.PushBack({ 68, 361, 64, 64 });
	throwStuff.PushBack({ 134, 361, 64, 64 });
	throwStuff.PushBack({ 200, 361, 64, 64 });
	throwStuff.PushBack({ 266, 361, 64, 64 });
	throwStuff.PushBack({ 332, 361, 64, 64 });
	throwStuff.PushBack({ 398, 361, 64, 64 });
	throwStuff.PushBack({ 464, 361, 64, 64 });
	throwStuff.PushBack({ 530, 361, 64, 64 });
	throwStuff.speed = 0.15f;

	throwStuff2.PushBack({ 742, 1347, 64, 64 });
	throwStuff2.PushBack({ 708, 1347, 64, 64 });
	throwStuff2.PushBack({ 642, 1347, 64, 64 });
	throwStuff2.PushBack({ 576, 1347, 64, 64 });
	throwStuff2.PushBack({ 510, 1347, 64, 64 });
	throwStuff2.PushBack({ 444, 1347, 64, 64 });
	throwStuff2.PushBack({ 378, 1347, 64, 64 });
	throwStuff2.PushBack({ 312, 1347, 64, 64 });
	throwStuff2.PushBack({ 246, 1347, 64, 64 });
	throwStuff2.speed = 0.15f;

	hurt.PushBack({ 2, 106, 64, 64 });

	hurt2.PushBack({ 742, 1007, 64, 64 });

	hurtNoStuff.PushBack({ 2, 648, 64, 64 });

	hurtNoStuff2.PushBack({ 742, 1549, 64, 64 });

	death.PushBack({ 2, 276, 64, 64 });
	death.PushBack({ 68, 276, 64, 64 });
	death.PushBack({ 134, 276, 64, 64 });
	death.PushBack({ 200, 276, 64, 64 });
	death.PushBack({ 266, 276, 64, 64 });
	death.PushBack({ 332, 276, 64, 64 });
	death.PushBack({ 398, 276, 64, 64 });
	death.PushBack({ 464, 276, 64, 64 });
	death.speed = 0.15f;

	death2.PushBack({ 742, 1177, 64, 64 });
	death2.PushBack({ 708, 1177, 64, 64 });
	death2.PushBack({ 642, 1177, 64, 64 });
	death2.PushBack({ 576, 1177, 64, 64 });
	death2.PushBack({ 510, 1177, 64, 64 });
	death2.PushBack({ 444, 1177, 64, 64 });
	death2.PushBack({ 378, 1177, 64, 64 });
	death2.PushBack({ 312, 1177, 64, 64 });
	death2.speed = 0.15f;

	deathNoStuff.PushBack({ 2, 276, 64, 64 });
	deathNoStuff.PushBack({ 68, 276, 64, 64 });
	deathNoStuff.PushBack({ 134, 276, 64, 64 });
	deathNoStuff.PushBack({ 200, 276, 64, 64 });
	deathNoStuff.PushBack({ 266, 276, 64, 64 });
	deathNoStuff.PushBack({ 332, 276, 64, 64 });
	deathNoStuff.PushBack({ 398, 276, 64, 64 });
	deathNoStuff.PushBack({ 464, 276, 64, 64 });
	deathNoStuff.speed = 0.15f;

	deathNoStuff2.PushBack({ 742, 1634, 64, 64 });
	deathNoStuff2.PushBack({ 708, 1634, 64, 64 });
	deathNoStuff2.PushBack({ 642, 1634, 64, 64 });
	deathNoStuff2.PushBack({ 576, 1634, 64, 64 });
	deathNoStuff2.PushBack({ 510, 1634, 64, 64 });
	deathNoStuff2.PushBack({ 444, 1634, 64, 64 });
	deathNoStuff2.PushBack({ 378, 1634, 64, 64 });
	deathNoStuff2.PushBack({ 312, 1634, 64, 64 });
	deathNoStuff2.speed = 0.15f;

	App->pathfinding->CreatePath(position, App->player->position);

	catPeasantState = stateEnemies::enemyIdle_;

	animation = &idle;
	collider = App->collision->AddCollider({ 0, 0, 18, 18 }, COLLIDER_TYPE::COLLIDER_CATPEASANT, App->enemies);

}

void CatPeasant::Move()
{
	

	if (position.x > start_pos.x - 100)
		position.x--;

	ActualDirection();

	GeneralStatesMachine();

	SetDirectionBoolsToFalse();
	//position = App->pathfinding->GetLastPath();
		
}

void CatPeasant::GeneralStatesMachine() {

	switch (catPeasantState) {

	case enemyIdle_:
		if (toBackward == true) {
			catPeasantState = stateEnemies::enemyIdle2_;
			break;
		}
		animation = &idle;
		break;

	case enemyIdle2_:
		if (toForward == true) {
			catPeasantState = stateEnemies::enemyIdle_;
			break;
		}	
		animation = &idle2;
		break;

	case enemyPunished_:
		animation = &hurt;
		break;

	case enemyPunished2_:
		animation = &hurt2;
		break;

	case enemyDeath_:
		animation = &death;
		break;

	case enemyDeath2_:
		animation = &death2;
		break;

	case enemyAttack_:

		break;

	}


}

void CatPeasant::ActualDirection() {

	if (position.x < lastPosition.x)
		toBackward = true;
	else if (position.x > lastPosition.x)
		toForward = true;
	if (position.y < lastPosition.y)
		toUp = true;
	else if (position.y > lastPosition.y)
		toDown = true;

	lastPosition = position;

}

void CatPeasant::SetDirectionBoolsToFalse() {

	toUp = false;
	toDown = false;
	toForward = false;
	toBackward = false;

}

void CatPeasant::OnCollision(Collider* c1, Collider* c2) {


}