#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"

#include "j1Input.h"
#include "j1Textures.h"
#include "j1FadeToBlack.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "UILifeBar.h"

#include"Brofiler\Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	bool ret = true;

	index = config.child("maps").child("index").attribute("first").as_uint();
	LOG("Loading Scene %d", index);

	// Load maps
	for (pugi::xml_node node = config.child("maps").child("map"); node; node = node.next_sibling("map")) {
		if (node.attribute("id").as_uint() == 1)
			map1 = node.attribute("name").as_string();
		if (node.attribute("id").as_uint() == 2)
			map2 = node.attribute("name").as_string();
	}

	// Load songs
	for (pugi::xml_node node = config.child("audio").child("songs").child("song"); node; node = node.next_sibling("song")) {
		if (node.attribute("id").as_uint() == 1)
			song1 = node.attribute("name").as_string();
		if (node.attribute("id").as_uint() == 2) {
			song2 = node.attribute("name").as_string();
			volume_adjustment = node.attribute("volume_adjustment").as_uint();
		}
	}

	// Load FX
	for (pugi::xml_node node = config.child("audio").child("fx").child("fx"); node; node = node.next_sibling("fx"))
		App->audio->LoadFx(node.attribute("name").as_string());

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	UILifeBar_Info girl_life_bar;
	girl_life_bar.bar = { 86,532,222,4 };
	girl_life_bar.life = 222;
	girl_life_bar.life_bar_position = { 678,209 };
	girl_life_bar.tex_name = DSUI_;
	girl_life_bar.tex_area = { 80,524,230,8 };
	progress_bar = App->gui->CreateUILifeBar({672,208}, girl_life_bar, this);

	if (!loading)
		App->entities->Start();

	// Change between maps
	if (index == 0) {
		App->map->Load(map1.GetString());
		App->audio->PlayMusic(song1.GetString());
		App->audio->SetMusicVolume(App->audio->music_volume);
	}
	else {
		App->map->Load(map2.GetString());
		App->audio->PlayMusic(song2.GetString());
		App->audio->SetMusicVolume(App->audio->music_volume + volume_adjustment);
	}

	// Camera
	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();

	// Load entities
	if (App->entities->LoadPathsInfo())
		App->entities->AddEntities();

	// Pathfinding collision data
	App->pathfinding->SetMap(App->map->data.width, App->map->data.height, (uchar*)App->map->collisionLayer->data);

	loading = true;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (loading) {
		if (!loading_state) {
			gate = false;
			fx = false;

			// Player start position
			App->entities->playerData->start_pos = App->map->data.GetObjectPosition("Player", "StartPos");
			App->entities->playerData->position = App->entities->playerData->start_pos;
		}
		loading = false;
	} 

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// F1, F2, F3, F4, F5, F6, +, -
	DebugKeys();
	
	// Set window title
	App->input->GetMousePosition(mouse.x, mouse.y);

	if (countdown_to_die.ReadSec() >= 2.0f)
	{
		progress_bar->DecreaseLifeProgress(1);
		countdown_to_die.Start();
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing Scene %d", index);
	App->audio->PauseMusic();
	App->map->UnLoad();

	if (!loading_state)
		App->entities->CleanUp();

	return true;
}

void j1Scene::MoveCamera() {

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) //move camera left (debug functionality)
		App->render->camera.x += 5;
	else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) //move camera right (debug functionality)
		App->render->camera.x -= 5;
	else {
		if(App->entities->playerData != nullptr)
			App->render->camera.x = (int)(App->entities->playerData->position.x - 100) * (-1) * App->win->GetScale();
	}

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_REPEAT) //move camera up (debug functionality)
		App->render->camera.y += 5;
	else if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT) //move camera down (debug functionality)
		App->render->camera.y -= 5;
	else {
		if (App->entities->playerData != nullptr)
			App->render->camera.y = (int)(App->entities->playerData->position.y - 150) * (-1) *  App->win->GetScale();
	}
}

// Save
bool j1Scene::Save(pugi::xml_node& save) const {
	bool ret = false;

	if (save.child("index") == NULL) {
		save.append_child("index").append_attribute("index") = index;
	}
	else {
		save.append_child("index").attribute("index") = index;
	}

	if (save.child("gate") == NULL) {
		save.append_child("gate").append_attribute("opened") = gate;
		save.child("gate").append_attribute("fx") = fx;
	}
	else {
		save.child("gate").attribute("opened") = gate;
		save.child("gate").attribute("fx") = fx;
	}

	ret = true;
	return ret;
}

// Load
bool j1Scene::Load(pugi::xml_node& save) {
	bool ret = false;

	App->entities->playerData->player.SetState(stop_);

	if (save.child("gate") != NULL) {
		gate = save.child("gate").attribute("opened").as_bool();
		fx = save.child("gate").attribute("fx").as_bool();
	}

	if (save.child("index") != NULL) {
		if (save.child("index").attribute("index").as_uint() != index && App->fade->GetStep() == 0) {
			index = save.child("index").attribute("index").as_uint();
			App->fade->FadeToBlack(this, this, 1);
		}
	}
	loading = true;

	ret = true;
	return ret;
}

// Debug keys
void j1Scene::DebugKeys() {
	// F1: start from the beginning of the first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { 
		if (App->entities->playerData->player.GetState() == forward_ || App->entities->playerData->player.GetState() == backward_
			|| App->entities->playerData->player.GetState() == idle_ || App->entities->playerData->player.GetState() == idle2_) {
			loading_state = false;

			App->entities->playerData->player.SetState(stop_);

			if (index == 0) {
				App->entities->playerData->position = App->entities->playerData->start_pos;
				gate = false;
				fx = false;
			}
			else {
				index = 0;
				App->fade->FadeToBlack(this, this, 1);
			}
		}
	}

	// F2: start from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) { 
		if (App->entities->playerData->player.GetState() == forward_ || App->entities->playerData->player.GetState() == backward_
			|| App->entities->playerData->player.GetState() == idle_ || App->entities->playerData->player.GetState() == idle2_) {
			loading_state = false;

			App->entities->playerData->player.SetState(stop_);
			App->entities->playerData->position = App->entities->playerData->start_pos;
			gate = false;
			fx = false;
		}
	}

	// F3: show colliders

	// F4: change between maps
	if (App->entities->playerData != nullptr) {
		if ((App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN || App->map->data.CheckIfEnter("Player", "EndPos", App->entities->playerData->position)) && App->fade->GetStep() == 0) {
			if (App->entities->playerData->player.GetState() == forward_ || App->entities->playerData->player.GetState() == backward_
				|| App->entities->playerData->player.GetState() == idle_ || App->entities->playerData->player.GetState() == idle2_) {
				loading_state = false;

				if (index == 0)
					index = 1;
				else
					index = 0;

				App->entities->playerData->player.SetState(stop_);
				App->fade->FadeToBlack(this, this, 1);
			}
		}
	}
	// F5: save the current state
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->entities->playerData->player.GetState() != playerstates::punished_)
	{
		App->SaveGame();
		last_index = index;
	}

	// F6: load the previous state
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		loading_state = true;
		App->LoadGame();
	}

	// F7: fullscreen
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		if (App->win->fullscreen) {
			App->win->fullscreen = false;
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_SHOWN);
		}
		else {
			App->win->fullscreen = true;
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
	}

	// F10: God mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		god = !god;

	// ---------------------------------------

	// +, -: adjust music volume
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->ChangeMusicVolume(true); //music volume + 8

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->ChangeMusicVolume(false); //music volume - 8

	//camera blit

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->map->blit_offset < 15 && App->map->camera_blit)
		App->map->blit_offset += 7;

	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->map->blit_offset > -135 && App->map->camera_blit)
		App->map->blit_offset -= 7;

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->map->camera_blit = !App->map->camera_blit;

}