#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"

#include "j1EntityFactory.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Menu.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"

#include"Brofiler\Brofiler.h"

#include "UILifeBar.h"

j1Menu::j1Menu() : j1Module()
{
	name.create("menu");
}

// Destructor
j1Menu::~j1Menu()
{}

// Called before render is available
bool j1Menu::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

// Called before the first frame
bool j1Menu::Start()
{
	bool ret = true;
	return ret;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	bool ret = true;

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		//App->fade->FadeToBlack(this, App->scene, 1);

	return ret;
}

// Called each loop iteration
bool j1Menu::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool j1Menu::CleanUp()
{
	bool ret = true;


	return ret;
}