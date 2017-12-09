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
	UILifeBar_Info girl_life_bar;
	girl_life_bar.bar = { 86,400,100,4 };
	girl_life_bar.life = 222;
	girl_life_bar.life_bar_position = { 678,209 };
	girl_life_bar.tex_name = DSUI_;
	girl_life_bar.tex_area = { 80,524,230,8 };
	App->gui->CreateUILifeBar({ 672,208 }, girl_life_bar, this);
	return ret;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		App->fade->FadeToBlack(this, App->scene, 1);

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