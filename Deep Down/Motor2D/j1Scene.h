#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

#include "Animation.h"
#include "j1Timer.h"

struct SDL_Texture;
struct UILifeBar;

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
	float up = false, down = false, left = false, right = false;
	uint width = 0;
	uint height = 0;
	uint scale = 0;

	bool gate = false, fx = false, loading = true;
	bool loading_state = false;

	uint index = 0;
	uint last_index = 0;

	bool god = false;

private:

	p2SString map1 = nullptr;
	p2SString map2 = nullptr;
	p2SString song1 = nullptr;
	p2SString song2 = nullptr;
	p2SString fx_names[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

	j1Timer countdown_to_die;
	uint volume_adjustment = 0;

	iPoint mouse = { 0,0 };
	UILifeBar* progress_bar;
};

#endif // __j1SCENE1_H__