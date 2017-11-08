#include "j1App.h"
#include "j1Enemies.h"
#include "Imp.h"

#include "j1Collision.h"
#include "j1Player.h"

#include "SDL/include/SDL_timer.h"

Imp::Imp(int x, int y) : Enemy(x, y)
{
	// Load animations
	pugi::xml_node animations_node = config.child("types").child("cat").child("animations");
	pugi::xml_node node;

	//r_shield_idle
	node = animations_node.child("r_shield_idle");
	r_shield_idle.speed = node.attribute("speed").as_float();
	r_shield_idle.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		r_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_shield_idle
	node = animations_node.child("l_shield_idle");
	l_shield_idle.speed = node.attribute("speed").as_float();
	l_shield_idle.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		l_shield_idle.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_shield_hurt
	node = animations_node.child("r_shield_hurt");
	r_shield_hurt.speed = node.attribute("speed").as_float();
	r_shield_hurt.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		r_shield_hurt.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_shield_hurt
	node = animations_node.child("l_shield_hurt");
	l_shield_hurt.speed = node.attribute("speed").as_float();
	l_shield_hurt.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		l_shield_hurt.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_jump
	node = animations_node.child("r_jump");
	r_jump.speed = node.attribute("speed").as_float();
	r_jump.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		r_jump.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_jump
	node = animations_node.child("l_jump");
	l_jump.speed = node.attribute("speed").as_float();
	l_jump.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		l_jump.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_throw_bomb
	node = animations_node.child("r_throw_bomb");
	r_throw_bomb.speed = node.attribute("speed").as_float();
	r_throw_bomb.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		r_throw_bomb.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_throw_bomb
	node = animations_node.child("l_throw_bomb");
	l_throw_bomb.speed = node.attribute("speed").as_float();
	l_throw_bomb.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		l_throw_bomb.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_shield_walk
	node = animations_node.child("r_shield_walk");
	r_shield_walk.speed = node.attribute("speed").as_float();
	r_shield_walk.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		r_shield_walk.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_shield_walk
	node = animations_node.child("l_shield_walk");
	l_shield_walk.speed = node.attribute("speed").as_float();
	l_shield_walk.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		l_shield_walk.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//r_bomb
	node = animations_node.child("r_bomb");
	r_bomb.speed = node.attribute("speed").as_float();
	r_bomb.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		r_bomb.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	//l_bomb
	node = animations_node.child("l_bomb");
	l_bomb.speed = node.attribute("speed").as_float();
	l_bomb.loops = node.attribute("loops").as_bool();
	for (node = node.child("frame"); node; node = node.next_sibling("frame")) {
		l_bomb.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	animation = &r_shield_idle;
	collider = App->collision->AddCollider({ 0, 0, 18, 18 }, COLLIDER_TYPE::COLLIDER_IMP, App->enemies);
}

void Imp::Move()
{
	position.x = App->player->position.x;
	position.y = App->player->position.y;

}

void Imp::OnCollision(Collider* c1, Collider* c2) 
{


}