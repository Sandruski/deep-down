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

	camera_blit = App->map->camera_blit;
	App->map->camera_blit = false;

	// Map
	if (App->map->Load("menu.tmx"))
		App->entities->AddEntities();

	// Get screen parameters
	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();
	//_get_screen_parameters

	UIImage_Info black_screen;
	black_screen.color = { 0,0,0,255 };
	black_screen.quad = true;
	black_screen.tex_area = { 0, 0, static_cast<int>(width * scale), static_cast<int>(height * scale) };
	black_screen_image = App->gui->CreateUIImage({ 0,0 }, black_screen);

	// Camera
	camera_start_position = { 22,0 };
	camera_start_position = App->map->MapToWorld(camera_start_position.x, camera_start_position.y);
	App->render->camera.x = -(camera_start_position.x + 10) * App->win->GetScale();

	// Game title
	UILabel_Info label;
	label.font_name = ZELDA_;
	label.normal_color = { 255,255,255,0 };
	label.interactive = false;

	iPoint title_position = { 70 * scale,55 * scale };
	iPoint tracking = { 5,12 };

	label.text = "D";
	letters[i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);
	SDL_Rect letter_size = letters[i]->GetLocalRect();
	iPoint letter_position = letters[i]->GetLocalPos();
	label.text = "e";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + (letter_size.w + tracking.x) * scale,letter_position.y + tracking.y * scale }, label);
	letter_size = letters[i]->GetLocalRect();
	letter_position = letters[i]->GetLocalPos();
	label.text = "e";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y }, label);
	letter_size = letters[i]->GetLocalRect();
	letter_position = letters[i]->GetLocalPos();
	label.text = "p";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y }, label);

	letter_size = letters[i]->GetLocalRect();
	letter_position = letters[i]->GetLocalPos();
	label.text = "D";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x - 2 * letter_size.w * scale, letter_position.y + (letter_size.h - letter_size.h / scale) * scale }, label);
	letter_size = letters[i]->GetLocalRect();
	letter_position = letters[i]->GetLocalPos();
	label.text = "o";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y + tracking.y * scale }, label);
	letter_size = letters[i]->GetLocalRect();
	letter_position = letters[i]->GetLocalPos();
	label.text = "w";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y }, label);
	letter_size = letters[i]->GetLocalRect();
	letter_position = letters[i]->GetLocalPos();
	label.text = "n";
	letters[++i] = App->gui->CreateUILabel({ letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y }, label);
	i = 0;
	
	float cat_seconds = 2.5f;
	total_time = (Uint32)(cat_seconds * 0.5f * 1000.0f);
	//_game_title

	// Cat
	EntityInfo entity;
	entity.type = CAT_;
	entity.position = { camera_start_position.x,50 };
	cat = (Cat*)App->entities->SpawnEntity(entity);
	//_cat

	// Press any button
	label.font_name = SOBAD_;
	label.text = "PRESS ANY BUTTON";
	label.horizontal_orientation = CENTER_;
	press_any_button = App->gui->CreateUILabel({ (int)width / 2, (int)height - 150 }, label, this);

	label.text = "SKIP";
	skip = App->gui->CreateUILabel({ 50, 30 }, label, this);
	//_press_any_button

	UIButton_Info button;
	button.tex_name = MAIN_MENU_;
	button.interactive = false;

	for (int i = 0; i < 5; ++i) {
		button.normal_tex_area = { 0,i * (21 + 2),81,21 };
		button.hover_tex_area = { 84,i * (21 + 2),81,21 };
		button.pressed_tex_area = { 168,i * (21 + 2),81,21 };
		MENU_PAUSE_buttons[i] = App->gui->CreateUIButton({ 50,400 + 70 * i }, button, this);
	}
	i = 0;
	SDL_SetTextureAlphaMod((SDL_Texture*)App->gui->GetTexture(MAIN_MENU_), 0);

	label.text = "Play";
	label.font_name = SOBAD_8_;
	label.normal_color = { 47,33,56,0 };
	label.hover_color = { 183,116,141,255 };
	label.pressed_color = { 255,247,226,255 };
	label.horizontal_orientation = CENTER_;
	label.vertical_orientation = MIDDLE_;
	MENU_PAUSE_options[i] = App->gui->CreateUILabel({ MENU_PAUSE_buttons[i]->GetLocalRect().w * scale / 2,MENU_PAUSE_buttons[i]->GetLocalRect().h * scale / 2 }, label, this, MENU_PAUSE_buttons[i]);
	label.text = "Continue";
	MENU_PAUSE_options[i] = App->gui->CreateUILabel({ MENU_PAUSE_buttons[i]->GetLocalRect().w * scale / 2,MENU_PAUSE_buttons[i]->GetLocalRect().h * scale / 2 }, label, this, MENU_PAUSE_buttons[++i]);
	label.text = "Settings";
	MENU_PAUSE_options[i] = App->gui->CreateUILabel({ MENU_PAUSE_buttons[i]->GetLocalRect().w * scale / 2,MENU_PAUSE_buttons[i]->GetLocalRect().h * scale / 2 }, label, this, MENU_PAUSE_buttons[++i]);
	label.text = "Credits";
	MENU_PAUSE_options[i] = App->gui->CreateUILabel({ MENU_PAUSE_buttons[i]->GetLocalRect().w * scale / 2,MENU_PAUSE_buttons[i]->GetLocalRect().h * scale / 2 }, label, this, MENU_PAUSE_buttons[++i]);
	label.text = "Exit";
	MENU_PAUSE_options[i] = App->gui->CreateUILabel({ MENU_PAUSE_buttons[i]->GetLocalRect().w * scale / 2,MENU_PAUSE_buttons[i]->GetLocalRect().h * scale / 2 }, label, this, MENU_PAUSE_buttons[++i]);
	i = 0;

	menuState = MenuState::TITLE_TO_START_;
	
	return ret;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	bool ret = true;

	Uint32 now = (SDL_GetTicks() - start_time);
	float normalized = MIN(1.0f, (float)now / (float)total_time);
	float normalized2 = MIN(1.0f, (float)now / (float)total_time);
	normalized2 = 1 - normalized2;
	float normalized3 = MIN(1.0f, (float)now / (float)total_time);
	normalized3= 1 - normalized3;
	float normalized4 = MIN(1.0f, (float)now / (float)total_time);

	float alpha = 255.0f * normalized;
	float alpha2 = 255.0f * normalized2;
	float alpha3 = 255.0f * normalized3;
	float alpha4 = 255.0f * normalized3;

	// Time variables
	float press_any_button_speed = 1.5f;
	float press_any_button_seconds = 2.0f;
	float fade_seconds = 2.0f;
	float scene_seconds = 5.0f;

	if (cat != nullptr)
		cat->Draw(App->entities->CatTex);

	switch (catPosition) {

	case 0:
		cat->position.x += 30 * dt;
		cat->SetCatState(CatState::rc_run);

		if (cat->position.x >= camera_start_position.x + 45.0f) {
			cat->SetCatState(CatState::rc_jump);
			catPosition++;
			break;
		}
		break;

	case 1:
		cat->position.x += 20 * dt;
		cat->position.y -= 20 * dt;

		if (cat->position.x >= camera_start_position.x + 65.0f) {
			print_title = true;
			start_time = SDL_GetTicks();
			now = (SDL_GetTicks() - start_time);
			normalized = MIN(1.0f, (float)now / (float)total_time);
			alpha = 255.0f * normalized;

			cat->SetCatState(CatState::rc_run);
			catPosition++;
			break;
		}
		break;

	case 2:
		cat->position.x += 30 * dt;

		if (cat->position.x >= camera_start_position.x + 85.0f) {
			cat->SetCatState(CatState::rc_roll);
			catPosition++;
			break;
		}
		break;

	case 3:
		cat->position.x += 20 * dt;
		cat->position.y += 6 * dt;

		if (cat->position.x >= camera_start_position.x + 120.0f) {
			cat->SetCatState(CatState::rc_run);
			catPosition++;
			break;
		}
		break;

	case 4:
		cat->position.x += 30 * dt;

		if (cat->position.x >= camera_start_position.x + 190.0f) {
			cat->SetCatState(CatState::rc_roll);
			catPosition++;
			break;
		}
		break;

	case 5:
		cat->position.x += 15 * dt;
		cat->position.y += 25 * dt;

		if (cat->position.x >= camera_start_position.x + 250.0f) {
			cat->SetCatState(CatState::rc_jump_no_fall);
			catPosition = 6;
			break;
		}
		break;

	case 6:
		if (timer >= 2.0f) {
			cat->SetCatState(CatState::rc_roll);
			cat->position.x += 10 * dt;
			cat->position.y += 20 * dt;

			if (cat->position.x >= camera_start_position.x + 275.0f) {
				timer = 0;

				cat->SetCatState(CatState::rc_land_soft);
				catPosition++;
				break;
			}
		}
		break;

	case 7:
		timer += 1.0f * dt;

		if (timer >= 0.5f) {
			cat->SetCatState(CatState::rc_idle);
			catPosition++;
		}

		break;

	case 8:

		// Don't blit cat in the other 2 menu screens + move cat right when start game
		break;
	}

	switch (menuState) {

	case MenuState::TITLE_TO_START_:

		// Skip
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			is_invisible = true;
			for (uint j = 0; j < 8; ++j)
				letters[j]->SetColor({ 255,255,255,255 });
			cat->position = { 416,496 };
			start_time = SDL_GetTicks();
			catPosition = 7;
			menuState = MenuState::TITLE_TO_MENU_PAUSE_;
			break;
		}

		if (!is_invisible) {
			skip->SetColor({ 255,255,255,(Uint8)alpha4 });

			if (alpha4 == 255.0f) {
				alpha3 = 255.0f;
				start_time = SDL_GetTicks();
				is_invisible = true;
			}
		}
		else {
			skip->SetColor({ 255,255,255,(Uint8)alpha3 });

			if (alpha3 == 0.0f) {
				alpha4 = 0.0f;
				start_time = SDL_GetTicks();
				is_invisible = false;
			}
		}
		//_skip

		if (print_title) {
			if (!visible_again) {
				letters[i]->SetColor({ 255,255,255,(Uint8)alpha });
			}
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

					i = 0;
					timer = 0.0f;
					alpha = 0.0f;
					total_time = (Uint32)(press_any_button_speed * 0.5f * 1000.0f);
					start_time = SDL_GetTicks();
					menuState = MenuState::PRESS_START_;
					break;
				}
			}
		}
		break;

	case MenuState::PRESS_START_:

		if (timer >= press_any_button_seconds) {

			static SDL_Event event;

			if (App->input->IsAnyKeyPressed()) {
				alpha = 0.0f;
				alpha2 = 255.0f;
				total_time = (Uint32)(scene_seconds * 0.5f * 1000.0f);
				start_time = SDL_GetTicks();
				menuState = MenuState::TITLE_TO_MENU_PAUSE_;
				break;
			}

			if (!is_invisible) {
				press_any_button->SetColor({ 255,255,255,(Uint8)alpha });

				if (alpha == 255.0f) {
					alpha2 = 255.0f;
					start_time = SDL_GetTicks();
					is_invisible = true;
				}
			}
			else {
				press_any_button->SetColor({ 255,255,255,(Uint8)alpha2 });

				if (alpha2 == 0.0f) {
					alpha = 0.0f;
					start_time = SDL_GetTicks();
					is_invisible = false;
				}
			}
		}
		else
			timer += 1.0f * dt;

		break;

	case MenuState::TITLE_TO_MENU_PAUSE_:

		black_screen_image->SetColor({ 0,0,0,(Uint8)alpha2 });
		press_any_button->SetColor({ 0,0,0,(Uint8)alpha2 });

		if (alpha2 == 0.0f) {
			start_time = SDL_GetTicks();
			menuState = MenuState::APPEAR_MENU_PAUSE_OPTIONS_;
			break;
		}
		break;

	case MenuState::APPEAR_MENU_PAUSE_OPTIONS_:

		SDL_SetTextureAlphaMod((SDL_Texture*)App->gui->GetTexture(MAIN_MENU_), (Uint8)alpha);

		SDL_Color normal = MENU_PAUSE_options[i]->GetColor();

		MENU_PAUSE_options[i]->SetColor({ normal.r,normal.g,normal.b,(Uint8)alpha }, true);
		if (alpha >= 51.0f)
			MENU_PAUSE_options[1]->SetColor({ normal.r,normal.g,normal.b,(Uint8)alpha }, true);
		if (alpha >= 102.0f)
			MENU_PAUSE_options[2]->SetColor({ normal.r,normal.g,normal.b,(Uint8)alpha }, true);
		if (alpha >= 153.0f)
			MENU_PAUSE_options[3]->SetColor({ normal.r,normal.g,normal.b,(Uint8)alpha }, true);
		if (alpha >= 204.0f)
			MENU_PAUSE_options[4]->SetColor({ normal.r,normal.g,normal.b,(Uint8)alpha }, true);

		if (alpha == 255.0f) {

			for (uint i = 0; i < 5; ++i) {
				MENU_PAUSE_buttons[i]->SetInteraction(true);
				MENU_PAUSE_options[i]->SetInteraction(true);
			}

			menuState = MenuState::IN_MAIN_MENU_;
			break;
		}
		break;

	case MenuState::IN_MAIN_MENU_:

		break;

	case MenuState::IN_SETTINGS_:
		if (App->render->camera.x < 0)
			App->render->camera.x += 250 * dt;
		break;

	case MenuState::IN_CREDITS_:
		if (App->render->camera.y > -(camera_start_position.y + 10) * App->win->GetScale())
			App->render->camera.x -= 250 * dt;
		break;

	case MenuState::NO_MENU_PAUSE_PAUSE_:
	default:
		break;
	}

	// Clouds transition
	if (clouds_transition) {

		UIImage_Info cloud;
		cloud.tex_name = GENERAL_;
		cloud.tex_area = { 0, 754, 208, 57 };
		UIImage* big_cloud = App->gui->CreateUIImage({ 0,0 }, cloud);
		cloud.tex_area = { 16, 832, 112, 27 };
		UIImage* medium_cloud = App->gui->CreateUIImage({ 0,0 }, cloud);
		cloud.tex_area = { 0, 877, 109, 23 };
		UIImage* small_cloud = App->gui->CreateUIImage({ 0,0 }, cloud);
	}

	return ret;
}

void j1Menu::OnUIEvent(UIElement* UIelem, UIEvents UIevent)
{

	switch (UIevent) {

	case UIEvents::MOUSE_LEFT_CLICK_:

		for (uint i = 0; i < 5; ++i) {
			if (UIelem == MENU_PAUSE_buttons[i]) {
				MENU_PAUSE_options[i]->SetColor(MENU_PAUSE_options[i]->GetColor(false, false, true));
			}
		}
		break;

	case UIEvents::MOUSE_LEFT_UP_:

		if (UIelem == MENU_PAUSE_buttons[MenuOptions::START_]) {
			App->map->camera_blit = camera_blit;
			App->fade->FadeToBlack(this, App->scene, 2.0f, slider_fade);
			break;
		}
		else if (UIelem == MENU_PAUSE_buttons[MenuOptions::CONTINUE_]) {
			App->map->camera_blit = camera_blit;
			App->scene->loading_state = true;
			App->LoadGame();
			break;
		}
		else if (UIelem == MENU_PAUSE_buttons[MenuOptions::SETTINGS_]) {
			menuState = MenuState::IN_SETTINGS_;
			App->fade->FadeToBlack(this, this, 2.0f, slider_fade, false, false);
			break;
		}
		else if (UIelem == MENU_PAUSE_buttons[MenuOptions::CREDITS_]) {
			menuState = MenuState::IN_CREDITS_;
			App->fade->FadeToBlack(this, this, 2.0f, slider_fade, false, false);
			break;
		}
		else if (UIelem == MENU_PAUSE_buttons[MenuOptions::EXIT_]) {
			// QUIT GAME
			break;
		}
		break;

	case UIEvents::MOUSE_ENTER_:

		for (uint i = 0; i < 5; ++i) {
			if (UIelem == MENU_PAUSE_buttons[i]) {
				MENU_PAUSE_options[i]->SetColor(MENU_PAUSE_options[i]->GetColor(false, true));
			}
		}

		break;

	case UIEvents::MOUSE_LEAVE_:

		for (uint i = 0; i < 5; ++i) {
			if (UIelem == MENU_PAUSE_buttons[i]) {
				MENU_PAUSE_options[i]->SetColor(MENU_PAUSE_options[i]->GetColor());
			}
		}

		break;
	}
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

	App->gui->ClearAllUI();
	App->entities->KillAllEntities();
	cat = nullptr;
	App->map->UnLoad();
	App->scene->active = true;

	return ret;
}