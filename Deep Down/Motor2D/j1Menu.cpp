#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"

#include "j1EntityFactory.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Menu.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Window.h"

#include "UIButton.h"
#include "UICursor.h"
#include "UIImage.h"
#include "UILabel.h"
#include "UIWindow.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h""

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

	// Map
	App->map->Load("menu.tmx");

	// Get screen size
	uint width = 0, height = 0, scale = 0;

	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();

	UIImage_Info black_screen;
	black_screen.color = { 0,0,0,255 };
	black_screen.quad = true;
	black_screen.tex_area = { 0, 0, static_cast<int>(width * scale), static_cast<int>(height * scale) };
	black_screen_image = App->gui->CreateUIImage({0,0}, black_screen);
	// Camera
	camera_start_position = { 22,0 };
	camera_start_position = App->map->MapToWorld(camera_start_position.x, camera_start_position.y);
	App->render->camera.x = -(camera_start_position.x + 10) * App->win->GetScale();

	// Game title
	UILabel_Info label;
	label.font_name = ZELDA_;
	label.normal_color = { 255,255,255,0 };

	iPoint title_position = { camera_start_position.x + 70,55 };
	iPoint tracking = { 5,12 };

	label.text = "D";
	letters[i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);
	iPoint letter_size = letters[i]->GetSize();
	iPoint letter_position = letters[i]->GetLocalPos();
	label.text = "e";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + letter_size.x + tracking.x,letter_position.y + tracking.y }, label);
	letter_size = letters[i]->GetSize();
	letter_position = letters[i]->GetLocalPos();
	label.text = "e";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + letter_size.x + tracking.x, letter_position.y }, label);
	letter_size = letters[i]->GetSize();
	letter_position = letters[i]->GetLocalPos();
	label.text = "p";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + letter_size.x + tracking.x, letter_position.y }, label);

	letter_size = letters[i]->GetSize();
	letter_position = letters[i]->GetLocalPos();
	label.text = "D";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x - 2 * letter_size.x, letter_position.y + letter_size.y - letter_size.y / 3 }, label);
	letter_size = letters[i]->GetSize();
	letter_position = letters[i]->GetLocalPos();
	label.text = "o";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + letter_size.x + tracking.x, letter_position.y + tracking.y }, label);
	letter_size = letters[i]->GetSize();
	letter_position = letters[i]->GetLocalPos();
	label.text = "w";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + letter_size.x + tracking.x, letter_position.y }, label);
	letter_size = letters[i]->GetSize();
	letter_position = letters[i]->GetLocalPos();
	label.text = "n";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + letter_size.x + tracking.x, letter_position.y }, label);
	i = 0;

	float fade_seconds = 2.0f;
	total_time = (Uint32)(fade_seconds * 0.5f * 1000.0f);
	//_game_title

	// Cat
	EntityInfo entity;
	entity.type = CAT_;
	entity.position = { -40,180 };

	cat = (Cat*)App->entities->SpawnEntity(entity);
	//_cat

	print_game_title = true;
	// Press ANY BUTTON


	//_press_enter

	return ret;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	bool ret = true;

	// Game title

	if (print_game_title) {
		Uint32 now = (SDL_GetTicks() - start_time);
		float normalized = MIN(1.0f, (float)now / (float)total_time);
		float normalized2 = MIN(1.0f, (float)now / (float)total_time);
		normalized2 = 1 - normalized2;

		float alpha = 255.0f * normalized;
		float alpha2 = 255.0f * normalized2;

		if (!visible_again)
			letters[i]->SetColor({ 255,255,255,(Uint8)alpha });
		if (i > 0)
			letters[i - 1]->SetColor({ 255,255,255,(Uint8)alpha2 });

		if (alpha == 255.0f && i < 8 - 1) {
			alpha = 0.0f;
			alpha2 = 255.0f;
			++i;
			start_time = SDL_GetTicks();
		}
		else if (alpha == 255.0f && i == 8 - 1) {
			alpha2 = 255.0f;
			++i;
			start_time = SDL_GetTicks();
			visible_again = true;
		}

		if (visible_again) {
			timer += 1.0f * dt;

			if (timer >= 2.0f) {
				for (uint j = 0; j < 8; ++j)
					letters[j]->SetColor({ 255,255,255,255 });

				print_game_title = false;
				start_time = SDL_GetTicks();
			}
		}
	}
	//_game_title

	// Cat
	if (!print_game_title && !all_visible) {
		total_time = (Uint32)(5.0f * 0.5f * 1000.0f);
		Uint32 now = (SDL_GetTicks() - start_time);
		float normalized = MIN(1.0f, (float)now / (float)total_time);
		normalized = 1 - normalized;

		float alpha = 255.0f * normalized;
		black_screen_image->SetColor({ 0,0,0,(Uint8)alpha });

		if (alpha == 0.0f)
			all_visible = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		left_transition = true;
		right_transition = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		left_transition = false;
		right_transition = true;
	}

	if (left_transition) {
		if (App->render->camera.x < -10)
			App->render->camera.x += 250 * dt;
		else
			left_transition = false;
	}

	if (right_transition) {
		if (App->render->camera.x > -(camera_start_position.x + 10) * App->win->GetScale())
			App->render->camera.x -= 250 * dt;
		else
			right_transition = false;
	}

	/*
	cat->position.x += 20 * dt;
	cat->SetCatState(CatState::rc_run);
	*/
	//_cat

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
	App->map->UnLoad();

	return ret;
}