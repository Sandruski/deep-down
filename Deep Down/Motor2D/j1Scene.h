#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

#include "Animation.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Save
	bool Save(pugi::xml_node&) const;

	// Load
	bool Load(pugi::xml_node&);

	// Debug keys
	void DebugKeys();

	void MoveCamera();

	// Camera
	float up, down, left, right;
	uint width;
	uint height;
	uint scale;

	bool gate, fx, loading;

private:

	uint index;

	p2SString map1;
	p2SString map2;
	p2SString song1;
	p2SString song2;
	p2SString fx_names[5];

	uint volume_adjustment;

	iPoint mouse;

	//Boss
	SDL_Texture* boss;
	Collider* bossColl;
	iPoint bossColliderPos = { 0,0 };
	Animation* current_animation;
	Animation bossAnimation;

public:
	fPoint bossPosition = { 0,0 };
};

#endif // __j1SCENE1_H__