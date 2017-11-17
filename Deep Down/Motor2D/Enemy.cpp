#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Render.h"
#include "j1Pathfinding.h"

Enemy::Enemy(float x, float y, PathInfo* path) : position(x, y), start_pos(x, y), path_info(path), mlast_pathfinding(DEFAULT_PATH_LENGTH)
{
	last_pathfinding = nullptr;
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	//if (collider != nullptr)
		//collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void Enemy::OnCollision(Collider* c1, Collider* c2)
{
	if ((c1->type == COLLIDER_PEASANT_SHOT && c2->type == COLLIDER_PLAYER) || (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_PEASANT_SHOT)) {



	}

	if ((c1->type == COLLIDER_IMP && c2->type == COLLIDER_ARROW) || (c1->type == COLLIDER_ARROW && c2->type == COLLIDER_IMP)) {
		collider->to_delete = true;
	}

}