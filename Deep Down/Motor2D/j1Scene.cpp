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
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Enemies.h"
#include "j1Pathfinding.h"

#include"Brofiler\Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	/*
	bossAnimation.PushBack({ 0, 0, 203, 298 });
	bossAnimation.PushBack({ 309, 0, 203, 298 });
	bossAnimation.PushBack({ 0, 298, 203, 298 });
	bossAnimation.speed = 0.1f;
	*/
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
	//Boss
	bossPosition = { App->player->position.x, 1200 };
	bossColliderPos = { (int)bossPosition.x, (int)bossPosition.y };
	bossColl = App->collision->AddCollider({ bossColliderPos.x, bossColliderPos.y, 203, 298 }, COLLIDER_BOSS, this);
	
	LOG("Loading boss textures");
	boss = App->tex->Load("Assets/Sprites/Textures/Boss.png");

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

	//App->player->SetState(stop_);

	if (!loading) {
		App->player->SetState(App->player->default_state);

		// Player start position
		App->player->startPos = App->map->data.GetObjectPosition("Player", "StartPos");
		App->player->position = App->player->startPos;

		gate = false;
		fx = false;
	}

	// Load enemies
	if (App->enemies->LoadPathsInfo())
		App->enemies->AddEnemies();

	loading = false;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// F1, F2, F3, F4, F5, F6, +, -
	DebugKeys();

	// Move camera
	MoveCamera();

	// Draw
	DrawEverything();
	
	// Set window title
	App->input->GetMousePosition(mouse.x, mouse.y);
	p2SString title("DEEP DOWN - Map %d:%dx%d Tiles:%dx%d Tilesets:%d Mouse (camera coords):%d,%d",					
					index, App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(), App->map->MouseTile(mouse.x, mouse.y));
	//App->win->SetTitle(title.GetString());

	// Scene2 boss
	/*
	if (index == 1 && App->player->position.y <= 1050)
		Boss();
	*/

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
	App->tex->UnLoad(boss);

	if (bossColl != nullptr)
		App->collision->EraseCollider(bossColl);

	return true;
}

void j1Scene::MoveCamera() {

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) //move camera left (debug functionality)
		App->render->camera.x += 5;
	else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) //move camera right (debug functionality)
		App->render->camera.x -= 5;
	else
		App->render->camera.x = (int)(App->player->position.x - 100) * (-1) * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_REPEAT) //move camera up (debug functionality)
		App->render->camera.y += 5;
	else if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT) //move camera down (debug functionality)
		App->render->camera.y -= 5;
	else
		App->render->camera.y = (int)(App->player->position.y - 150) * (-1) *  App->win->GetScale();
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

	App->player->SetState(stop_);

	if (save.child("gate") != NULL) {
		gate = save.child("gate").attribute("opened").as_bool();
		fx = save.child("gate").attribute("fx").as_bool();
	}

	if (save.child("index") != NULL) {
		if (save.child("index").attribute("index").as_uint() != index && App->fade->GetStep() == 0) {
			loading = true;
			index = save.child("index").attribute("index").as_uint();
			App->fade->FadeToBlack(this, this, 1);
		}
	}

	ret = true;
	return ret;
}

// Debug keys
void j1Scene::DebugKeys() {
	// F1: start from the beginning of the first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { 
		if (App->player->GetState() == forward_ || App->player->GetState() == backward_
			|| App->player->GetState() == idle_ || App->player->GetState() == idle2_) {
			App->player->SetState(stop_);

			if (index == 0) {
				App->player->position = App->player->startPos;
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
		if (App->player->GetState() == forward_ || App->player->GetState() == backward_
			|| App->player->GetState() == idle_ || App->player->GetState() == idle2_) {
			App->player->SetState(stop_);
			App->player->position = App->player->startPos;
			App->scene->bossPosition = { App->player->position.x, 1200 };
			gate = false;
			fx = false;
		}
	}

	// F3: show colliders

	// F4: change between maps
	if ((App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN || App->map->data.CheckIfEnter("Player", "EndPos", App->player->position)) && App->fade->GetStep() == 0) {
		if (App->player->GetState() == forward_ || App->player->GetState() == backward_
			|| App->player->GetState() == idle_ || App->player->GetState() == idle2_) {
			if (index == 0)
				index = 1;
			else
				index = 0;

			App->player->SetState(stop_);
			App->fade->FadeToBlack(this, this, 1);
		}
	}

	// F5: save the current state
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	// F6: load the previous state
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		App->scene->bossPosition = { App->player->position.x, 1200 };
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

	// ---------------------------------------

	// +, -: adjust music volume
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->ChangeMusicVolume(true); //music volume + 8

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->ChangeMusicVolume(false); //music volume - 8

	//camera blit

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT && App->map->blit_offset < 15 && App->map->camera_blit)
		App->map->blit_offset += 7;

	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT && App->map->blit_offset > -135 && App->map->camera_blit)
		App->map->blit_offset -= 7;

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->map->camera_blit = !App->map->camera_blit;

}

void  j1Scene::DrawEverything() {

	BROFILER_CATEGORY("DrawEverything", Profiler::Color::Azure);

	// Draw Map
	App->map->Draw();

	// Draw player
	App->render->Blit(App->player->player, App->player->position.x, App->player->position.y, App->player->r);

	// Draw Above layer
	App->map->DrawAboveLayer();

}

void j1Scene::Boss() {

	bossColliderPos = { (int)bossPosition.x, (int)bossPosition.y };
	bossColl->SetPos(bossColliderPos.x - (203 / 2), bossColliderPos.y + 10);

	bossPosition.x = App->player->position.x;

	current_animation = &bossAnimation;
	App->player->r = &current_animation->GetCurrentFrame();
	App->render->Blit(boss, bossPosition.x - (203 / 2), bossPosition.y, App->player->r);
	bossPosition.y -= 0.3;
}