#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"

#include "j1Render.h"
#include "j1Textures.h"
#include "j1Particles.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "j1EntityFactory.h"
#include "j1Scene.h"
#include "j1Map.h"

#include <math.h>

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	name.create("particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
}

j1Particles::~j1Particles()
{}

// Called before render is available
bool j1Particles::Awake(pugi::xml_node& config) {
	
	bool ret = true;

	pugi::xml_node animations_node = config.child("animations");
	pugi::xml_node node;

	//arrowRight
	node = animations_node.child("arrowRight");
	arrowRight.life = node.attribute("life").as_uint();
	node = node.child("frame");
	arrowRight.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	arrowRight.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	//arrowLeft
	node = animations_node.child("arrowLeft");
	arrowLeft.life = node.attribute("life").as_uint();
	node = node.child("frame");
	arrowLeft.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	arrowLeft.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	//firstAttack
	node = animations_node.child("firstAttack");
	firstAttack.life = node.attribute("life").as_uint();
	node = node.child("frame");
	firstAttack.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	firstAttack.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	//secondAttack
	node = animations_node.child("secondAttack");
	secondAttack.life = node.attribute("life").as_uint();
	node = node.child("frame");
	secondAttack.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	secondAttack.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	//thirdAttack
	node = animations_node.child("thirdAttack");
	thirdAttack.life = node.attribute("life").as_uint();
	node = node.child("frame");
	thirdAttack.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	thirdAttack.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	//CatPeasantSinus
	node = animations_node.child("CatPeasantSinus");
	CatPeasantSinus.life = node.attribute("life").as_uint();
	node = node.child("frame");
	CatPeasantSinus.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	CatPeasantSinus.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	//Imp_r_bomb
	node = animations_node.child("Imp_r_bomb");
	Imp_r_bomb.life = node.attribute("life").as_uint();
	node = node.child("frame");
	Imp_r_bomb.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	Imp_r_bomb.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	//Imp_l_bomb
	node = animations_node.child("Imp_l_bomb");
	Imp_l_bomb.life = node.attribute("life").as_uint();
	node = node.child("frame");
	Imp_l_bomb.anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	Imp_l_bomb.coll_size = { node.attribute("w").as_int(), node.attribute("h").as_int() };

	return ret;
}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");

	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG("Unloading particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
bool j1Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update(dt) == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			if (p->collider->type == COLLIDER_PEASANT_SHOT)
				App->render->Blit(App->entities->CatPeasantTex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
			else
			App->render->Blit(App->entities->PlayerTex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
		}
	}

	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, Uint32 delay, fPoint speed)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->speed.x = speed.x;
			p->speed.y = speed.y;
			if (collider_type != COLLIDER_NONE)
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
			active[i] = p;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life), coll_size(p.coll_size)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update(float dt)
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;

	App->entities->playerData->CheckCollision({ (int)position.x, (int)position.y }, coll_size, App->entities->playerData->player.check_collision_offset, up, down, left, right);

	if (App->map->data.CheckIfEnter("Player", "Gate", position) && App->scene->gate == false) {

		position.SetToZero();

		if (App->scene->fx == false) {
			App->audio->PlayFx(4, 0);
			App->scene->fx = true;
		}

		App->scene->gate = true;
	}

	if (collider->type == COLLIDER_ARROW) {
		if (left || right)
			position.x += speed.x * dt;
		else
			position.x = position.x;
	}
	else if (collider->type == COLLIDER_PEASANT_SHOT) {

		if (App->entities->playerData->position.y < position.y)
			position.y--;
		else if (App->entities->playerData->position.y > position.y)
			position.y++;
		if (App->entities->playerData->position.x < position.x)
			position.x--;
		else if (App->entities->playerData->position.x > position.x)
			position.x++;

		if (App->entities->playerData->position.x == position.x && App->entities->playerData->position.y == position.y)
			Particle::~Particle();
	}

	//position.y += speed.y;

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}
