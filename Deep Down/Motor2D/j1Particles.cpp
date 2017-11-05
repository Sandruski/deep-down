#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"

#include "j1Render.h"
#include "j1Textures.h"
#include "j1Particles.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"

#include <math.h>

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	name.create("particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	arrowRight.anim.PushBack({ 58, 520, 22, 3 });
	arrowLeft.anim.PushBack({ 900, 520, 22, 3 });
	arrowRight.life = 10000;
	arrowLeft.life = 10000;

	firstAttack.anim.PushBack({ 1000, 1000, 10, 10 });
	secondAttack.anim.PushBack({ 1000, 1000, 10, 10 });
	thirdAttack.anim.PushBack({ 1000, 1000, 10, 10 });
	firstAttack.life = 100;
	secondAttack.life = 100;
	thirdAttack.life = 100;
}

j1Particles::~j1Particles()
{}

// Called before render is available
bool j1Particles::Awake(pugi::xml_node& config) {
	bool ret = true;

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

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			App->render->Blit(App->player->player, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
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
	fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update()
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

	App->player->CheckCollision({ (int)position.x, (int)position.y }, { 22, 3 }, 2, up, down, left, right);

	if (App->map->data.CheckIfEnter("Player", "Gate", position) && App->scene->gate == false) {

		position.SetToZero();

		if (App->scene->fx == false) {
			App->audio->PlayFx(4, 0);
			App->scene->fx = true;
		}

		App->scene->gate = true;
	}

	if (left || right)
		position.x += speed.x;
	else
		position.x = position.x;

	//position.y += speed.y;

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}
