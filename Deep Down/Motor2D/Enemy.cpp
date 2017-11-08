#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Render.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
	start_pos.x = x;
	start_pos.y = y;
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
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void Enemy::OnCollision(Collider* collider, Collider* c2)
{

}