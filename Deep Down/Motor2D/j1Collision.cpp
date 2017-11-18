#include "p2Defs.h"
#include "p2Log.h"
#include "p2List.h"

#include "j1App.h"

#include "j1Collision.h"
#include "j1Input.h"
#include "j1Render.h"

j1Collision::j1Collision()
{
	name.create("collision");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_ARROW] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_BOSS] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_CATPEASANT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PEASANT_SHOT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_IMP_BOMB] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_IMP] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_MONKEY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;

	matrix[COLLIDER_ARROW][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_ARROW][COLLIDER_BOSS] = false;
	matrix[COLLIDER_ARROW][COLLIDER_CATPEASANT] = true;
	matrix[COLLIDER_ARROW][COLLIDER_PEASANT_SHOT] = false;
	matrix[COLLIDER_ARROW][COLLIDER_IMP_BOMB] = false;
	matrix[COLLIDER_ARROW][COLLIDER_IMP] = true;
	matrix[COLLIDER_ARROW][COLLIDER_MONKEY] = false;
	matrix[COLLIDER_ARROW][COLLIDER_ARROW] = false;

	matrix[COLLIDER_BOSS][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_BOSS][COLLIDER_ARROW] = false;
	matrix[COLLIDER_BOSS][COLLIDER_CATPEASANT] = false;
	matrix[COLLIDER_BOSS][COLLIDER_PEASANT_SHOT] = false;
	matrix[COLLIDER_BOSS][COLLIDER_IMP_BOMB] = false;
	matrix[COLLIDER_BOSS][COLLIDER_IMP] = false;
	matrix[COLLIDER_BOSS][COLLIDER_BOSS] = false;

	matrix[COLLIDER_CATPEASANT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_CATPEASANT][COLLIDER_ARROW] = true;
	matrix[COLLIDER_CATPEASANT][COLLIDER_BOSS] = false;
	matrix[COLLIDER_CATPEASANT][COLLIDER_PEASANT_SHOT] = false;
	matrix[COLLIDER_CATPEASANT][COLLIDER_IMP_BOMB] = false;
	matrix[COLLIDER_CATPEASANT][COLLIDER_IMP] = false;
	matrix[COLLIDER_CATPEASANT][COLLIDER_MONKEY] = false;
	matrix[COLLIDER_CATPEASANT][COLLIDER_CATPEASANT] = true;

	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_ARROW] = false;
	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_BOSS] = false;
	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_PEASANT_SHOT] = false;
	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_IMP_BOMB] = false;
	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_IMP] = false;
	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_MONKEY] = false;
	matrix[COLLIDER_PEASANT_SHOT][COLLIDER_PEASANT_SHOT] = false;

	matrix[COLLIDER_IMP_BOMB][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_IMP_BOMB][COLLIDER_ARROW] = false;
	matrix[COLLIDER_IMP_BOMB][COLLIDER_BOSS] = false;
	matrix[COLLIDER_IMP_BOMB][COLLIDER_CATPEASANT] = false;
	matrix[COLLIDER_IMP_BOMB][COLLIDER_PEASANT_SHOT] = false;
	matrix[COLLIDER_IMP_BOMB][COLLIDER_IMP] = false;
	matrix[COLLIDER_IMP_BOMB][COLLIDER_MONKEY] = false;
	matrix[COLLIDER_IMP_BOMB][COLLIDER_IMP_BOMB] = false;

	matrix[COLLIDER_IMP][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_IMP][COLLIDER_ARROW] = true;
	matrix[COLLIDER_IMP][COLLIDER_BOSS] = false;
	matrix[COLLIDER_IMP][COLLIDER_CATPEASANT] = false;
	matrix[COLLIDER_IMP][COLLIDER_PEASANT_SHOT] = false;
	matrix[COLLIDER_IMP][COLLIDER_IMP_BOMB] = false;
	matrix[COLLIDER_IMP][COLLIDER_MONKEY] = false;
	matrix[COLLIDER_IMP][COLLIDER_IMP] = true;

	matrix[COLLIDER_MONKEY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_MONKEY][COLLIDER_ARROW] = false;
	matrix[COLLIDER_MONKEY][COLLIDER_BOSS] = false;
	matrix[COLLIDER_MONKEY][COLLIDER_CATPEASANT] = false;
	matrix[COLLIDER_MONKEY][COLLIDER_PEASANT_SHOT] = false;
	matrix[COLLIDER_MONKEY][COLLIDER_IMP_BOMB] = false;
	matrix[COLLIDER_MONKEY][COLLIDER_IMP] = false;
	matrix[COLLIDER_MONKEY][COLLIDER_MONKEY] = false;

}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	DebugDraw();

	return true;
}

void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_ARROW: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_BOSS: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_CATPEASANT:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_PEASANT_SHOT:
			App->render->DrawQuad(colliders[i]->rect, 140, 255, 140, alpha);
			break;
		case COLLIDER_IMP:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 140, alpha);
			break;
		case COLLIDER_IMP_BOMB:
			App->render->DrawQuad(colliders[i]->rect, 5, 100, 5, alpha);
			break;
		case COLLIDER_MONKEY:
			App->render->DrawQuad(colliders[i]->rect, 30, 10, 50, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			return true;
		}
	}

	return false;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w && rect.x + rect.w > r.x && rect.y < r.y + r.h && rect.h + rect.y > r.y);
}