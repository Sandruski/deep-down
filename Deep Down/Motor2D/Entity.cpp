#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Entity.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Render.h"
#include "j1Pathfinding.h"

Entity::Entity(float x, float y, PathInfo* path) : position(x, y), start_pos(x, y), path_info(path), mlast_pathfinding(DEFAULT_PATH_LENGTH)
{
	last_pathfinding = nullptr;
	mlast_pathfinding.Clear();
}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites)
{
	//if (collider != nullptr)
		//collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void Entity::OnCollision(Collider* c1, Collider* c2)
{
}