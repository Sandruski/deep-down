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

	memset(title_letters, 0, 8);
	memset(main_menu_buttons, 0, 5);
	memset(main_menu_options, 0, 5);

	// Load menu map
	if (App->map->Load("menu.tmx"))
		App->entities->AddEntities();

	// Get screen parameters
	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();

	// Set camera position
	camera_start_position = { 22,0 };
	camera_start_position = App->map->MapToWorld(camera_start_position.x, camera_start_position.y);
	App->render->camera.x = -(camera_start_position.x + 10) * scale;

	// Create UI elements
	// Black quad
	UIImage_Info black_screen;
	black_screen.color = Black_;
	black_screen.quad = true;
	black_screen.tex_area = { 0, 0, (int)(width * scale), (int)(height * scale) };
	black_screen_image = App->gui->CreateUIImage({ 0,0 }, black_screen);

	// Game title
	UILabel_Info label;
	label.font_name = ZELDA_;
	label.interactive = false;

	label.text = "D";
	iPoint title_position = { 70 * scale,55 * scale };
	iPoint tracking = { 5,12 };
	title_letters[i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);

	label.text = "e";
	SDL_Rect letter_size = title_letters[i]->GetLocalRect();
	iPoint letter_position = title_letters[i]->GetLocalPos();
	title_position = { letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y + tracking.y * scale };
	title_letters[++i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);

	label.text = "e";
	letter_size = title_letters[i]->GetLocalRect();
	letter_position = title_letters[i]->GetLocalPos();
	title_position = { letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y };
	title_letters[++i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);

	label.text = "p";
	letter_size = title_letters[i]->GetLocalRect();
	letter_position = title_letters[i]->GetLocalPos();
	title_position = { letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y };
	title_letters[++i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);

	label.text = "D";
	letter_size = title_letters[i]->GetLocalRect();
	letter_position = title_letters[i]->GetLocalPos();
	title_position = { letter_position.x - 2 * letter_size.w * scale, letter_position.y + (letter_size.h - letter_size.h / scale) * scale };
	title_letters[++i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);

	label.text = "o";
	letter_size = title_letters[i]->GetLocalRect();
	letter_position = title_letters[i]->GetLocalPos();
	title_position = { letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y + tracking.y * scale };
	title_letters[++i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);

	label.text = "w";
	letter_size = title_letters[i]->GetLocalRect();
	letter_position = title_letters[i]->GetLocalPos();
	title_position = { letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y };
	title_letters[++i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);

	label.text = "n";
	letter_size = title_letters[i]->GetLocalRect();
	letter_position = title_letters[i]->GetLocalPos();
	title_position = { letter_position.x + (letter_size.w + tracking.x) * scale, letter_position.y };
	title_letters[++i] = App->gui->CreateUILabel({ title_position.x,title_position.y }, label);
	i = 0;

	for (i; i < 8; ++i)
		title_letters[i]->SetColor({ title_letters[i]->GetColor().r,title_letters[i]->GetColor().g,title_letters[i]->GetColor().b,0 });

	i = 0;
	//_game_title

	// Press any button label
	label.font_name = SOBAD_;
	label.horizontal_orientation = CENTER_;
	label.text = "PRESS ANY BUTTON";
	press_any_button = App->gui->CreateUILabel({ (int)width / 2, (int)height - 150 }, label, this);
	press_any_button->SetColor({ press_any_button->GetColor().r,press_any_button->GetColor().g,press_any_button->GetColor().b,0 });

	// Skip button label
	label.font_name = SOBAD_8_;
	label.text = "SKIP";
	label.normal_color = LightGrey_;
	skip = App->gui->CreateUILabel({ 50, 20 }, label, this);
	skip->SetColor({ skip->GetColor().r,skip->GetColor().g,skip->GetColor().b,0 });

	// Main menu buttons
	UIButton_Info button;
	button.tex_name = MAIN_MENU_;
	button.interactive = false;

	for (int i = 0; i < 5; ++i) {
		button.normal_tex_area = { 0,i * (21 + 2),81,21 };
		button.hover_tex_area = { 84,i * (21 + 2),81,21 };
		button.pressed_tex_area = { 168,i * (21 + 2),81,21 };
		main_menu_buttons[i] = App->gui->CreateUIButton({ 50,400 + 70 * i }, button, this);
	}
	i = 0;
	SDL_SetTextureAlphaMod((SDL_Texture*)App->gui->GetTexture(MAIN_MENU_), 0);

	// Main menu options
	label.vertical_orientation = MIDDLE_;
	label.normal_color = Purple_;
	label.hover_color = Pink_;
	label.pressed_color = LightPink_;

	label.text = "Play";
	iPoint options_position = { main_menu_buttons[i]->GetLocalRect().w * scale / 2,main_menu_buttons[i]->GetLocalRect().h * scale / 2 };
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[i]);
	label.text = "Continue";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	label.text = "Settings";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	label.text = "Credits";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	label.text = "Exit";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	i = 0;

	for (i; i < 5; ++i)
		main_menu_options[i]->SetColor({ main_menu_options[i]->GetColor().r,main_menu_options[i]->GetColor().g,main_menu_options[i]->GetColor().b,0 });

	i = 0;
	//_create_UI_elements

	// Cat
	EntityInfo entity;
	entity.type = CAT_;
	entity.position = { camera_start_position.x,50 };
	cat = (Cat*)App->entities->SpawnEntity(entity);

	float cat_seconds = 2.5f;

	cat_position_increment[i] = 45.0f;
	cat_position_increment[++i] = 65.0f;
	cat_position_increment[++i] = 85.0f;
	cat_position_increment[++i] = 120.0f;
	cat_position_increment[++i] = 190.0f;
	cat_position_increment[++i] = 250.0f;
	cat_position_increment[++i] = 275.0f;
	cat_position_increment[++i] = 350.0f;
	i = 0;
	//_cat

	menuState = MenuState::TITLE_ANIMATION_;
	menuCatState = MenuCatState::MC_APPEAR_RUNNING_;

	return ret;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	bool ret = true;

	// Time variables
	float press_any_button_speed = 1.5f;
	float press_any_button_seconds = 2.0f;
	float skip_button_speed = 2.5f;
	float fade_seconds = 2.0f;
	float scene_seconds = 5.0f;
	float options_seconds = 3.0f;

	// Cat variables
	iPoint cat_final_position = { 627,190 };
	float cat_run = 30.0f;
	fPoint cat_jump = { 20.0f, 20.0f };
	fPoint cat_width_roll = { 20.0f, 6.0f };
	fPoint cat_height_roll = { 15.0f, 25.0f };
	fPoint cat_roll = { 10.0f, 20.0f };
	float seconds_stop_jumping = 2.0f;

	// Update cat
	if (cat != nullptr)
		cat->Draw(App->entities->CatTex);

	switch (menuCatState) {

	case MenuCatState::MC_APPEAR_RUNNING_:
		cat->SetCatState(CatState::rc_run);
		cat->position.x += cat_run * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[0]) {
			cat->SetCatState(CatState::rc_jump);
			menuCatState = MenuCatState::MC_JUMP_TITLE_;
			break;
		}
		break;

	case MenuCatState::MC_JUMP_TITLE_:
		cat->position.x += cat_jump.x * dt;
		cat->position.y -= cat_jump.y * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[1]) {
			
			// Start title animation
			print_title = true;
			/*
			start_time = SDL_GetTicks();
			now = (SDL_GetTicks() - start_time);
			normalized = MIN(1.0f, (float)now / (float)total_time);
			alpha = 255.0f * normalized;
			*/
			cat->SetCatState(CatState::rc_run);
			menuCatState = MenuCatState::MC_RUN_FIRST_LETTER_;
			break;
		}
		break;

	case MenuCatState::MC_RUN_FIRST_LETTER_:
		cat->position.x += cat_run * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[2]) {
			cat->SetCatState(CatState::rc_roll);
			menuCatState = MenuCatState::MC_ROLL_SECOND_LETTER_;
			break;
		}
		break;

	case MenuCatState::MC_ROLL_SECOND_LETTER_:
		cat->position.x += cat_width_roll.x * dt;
		cat->position.y += cat_width_roll.y * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[3]) {
			cat->SetCatState(CatState::rc_run);
			menuCatState = MenuCatState::MC_RUN_WORD_;
			break;
		}
		break;

	case MenuCatState::MC_RUN_WORD_:
		cat->position.x += cat_run * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[4]) {
			cat->SetCatState(CatState::rc_roll);
			menuCatState = MenuCatState::MC_ROLL_END_WORD_;
			break;
		}
		break;

	case MenuCatState::MC_ROLL_END_WORD_:
		cat->position.x += cat_height_roll.x * dt;
		cat->position.y += cat_height_roll.y * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[5]) {
			cat->SetCatState(CatState::rc_jump_no_fall);
			menuCatState = MenuCatState::MC_STOP_JUMPING_;
			break;
		}
		break;

	case MenuCatState::MC_STOP_JUMPING_:
		if (timer >= seconds_stop_jumping) {
			cat->SetCatState(CatState::rc_roll);
			cat->position.x += cat_roll.x * dt;
			cat->position.y += cat_roll.y * dt;

			if (cat->position.x >= camera_start_position.x + cat_position_increment[6]) {
				timer = 0;

				cat->SetCatState(CatState::rc_land_soft);
				menuCatState = MenuCatState::MC_AT_GROUND_;
				break;
			}
		}
		break;

	case MenuCatState::MC_AT_GROUND_:
		cat->SetCatState(CatState::rc_idle);
		break;

	case MenuCatState::MC_START_:
		cat->SetCatState(CatState::rc_run);
		cat->position.x += cat_run * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[7]) {
			menuCatState = MenuCatState::NO_CAT_;
			App->map->camera_blit = camera_blit;
			App->fade->FadeToBlack(this, App->scene, 2.0f, slider_fade);
			break;
		}
		break;

	case MenuCatState::MC_CONTINUE_:
		cat->SetCatState(CatState::rc_run);
		cat->position.x += cat_run * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[7]) {
			menuCatState = MenuCatState::NO_CAT_;
			App->map->camera_blit = camera_blit;
			App->scene->loading_state = true;
			App->LoadGame();
			break;
		}
		break;

	case MenuCatState::NO_CAT_:
	default:
		break;
	}

	bool is_button_invisible = false;
	bool is_black_screen_image_invisible = false;
	float alpha = 0.0f;

	// Update menu
	switch (menuState) {

	case MenuState::TITLE_ANIMATION_:

		// Skip
		if (menuCatState > MenuCatState::NO_CAT_) {

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				skip->SetColor({ skip->GetColor().r,skip->GetColor().g,skip->GetColor().b,0 });
				is_invisible = true;

				for (uint j = 0; j < 8; ++j)
					title_letters[j]->SetColor(title_letters[j]->GetColor());

				cat->position = { (float)cat_final_position.x, (float)cat_final_position.y };
				menuCatState = MenuCatState::MC_AT_GROUND_;

				menuState = MenuState::TRANSITION_TO_MAIN_MENU_;
				break;
			}

			if (menuCatState <= MenuCatState::MC_RUN_WORD_) {
				skip->IntermitentFade(skip_button_speed);
			}
			else
				skip->SetColor({ skip->GetColor().r,skip->GetColor().g,skip->GetColor().b,0 });
		}
		//_skip

		/*
		if (print_title) {
			if (!visible_again) {
				title_letters[i]->SetColor({ title_letters[i]->GetColor().r,title_letters[i]->GetColor().g,title_letters[i]->GetColor().b,(Uint8)alpha });
			}
			if (i > 0)
				title_letters[i - 1]->SetColor({ title_letters[i - 1]->GetColor().r,title_letters[i - 1]->GetColor().g,title_letters[i - 1]->GetColor().b,(Uint8)alpha2 });

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
						title_letters[j]->SetColor(title_letters[j]->GetColor());

					i = 0;
					timer = 0.0f;
					alpha = 0.0f;
					total_time = (Uint32)(press_any_button_speed * 0.5f * 1000.0f);
					start_time = SDL_GetTicks();
					menuState = MenuState::PRESS_ANY_BUTTON_;
					break;
				}
			}
		}
		*/

		menuState = MenuState::PRESS_ANY_BUTTON_;

		break;

	case MenuState::PRESS_ANY_BUTTON_:

		if (timer >= press_any_button_seconds) {

			if (App->input->IsAnyKeyPressed()) {
				press_any_button->ResetFade();
				menuState = MenuState::TRANSITION_TO_MAIN_MENU_;
				break;
			}
			
			press_any_button->IntermitentFade(press_any_button_seconds);
		}
		else
			timer += 1.0f * dt;

		break;

	case MenuState::TRANSITION_TO_MAIN_MENU_:

		is_button_invisible = press_any_button->FromAlphaToAlphaFade(press_any_button->GetColor(false).a, 0.0f, scene_seconds);
		is_black_screen_image_invisible = black_screen_image->FromAlphaToAlphaFade(black_screen_image->GetColor().a, 0.0f, scene_seconds);

		if (is_button_invisible && is_black_screen_image_invisible) {
			menuState = MenuState::MAIN_MENU_OPTIONS_ANIMATION_;
			break;
		}
		break;

	case MenuState::MAIN_MENU_OPTIONS_ANIMATION_:

		alpha = App->gui->IncreaseDecreaseAlpha(0.0f, 255.0f, options_seconds);
		App->gui->SetTextureAlphaMod(Tex_Names::MAIN_MENU_, alpha);

		main_menu_options[0]->SetColor({ main_menu_options[0]->GetColor().r,main_menu_options[0]->GetColor().g,main_menu_options[0]->GetColor().b,(Uint8)alpha });
		if (alpha >= 1.0f * (255.0f / 5.0f))
			main_menu_options[1]->SetColor({ main_menu_options[1]->GetColor().r,main_menu_options[1]->GetColor().g,main_menu_options[1]->GetColor().b,(Uint8)alpha });
		if (alpha >= 2.0f * (255.0f / 5.0f))
			main_menu_options[2]->SetColor({ main_menu_options[2]->GetColor().r,main_menu_options[2]->GetColor().g,main_menu_options[2]->GetColor().b,(Uint8)alpha });
		if (alpha >= 3.0f * (255.0f / 5.0f))
			main_menu_options[3]->SetColor({ main_menu_options[3]->GetColor().r,main_menu_options[3]->GetColor().g,main_menu_options[3]->GetColor().b,(Uint8)alpha });
		if (alpha >= 4.0f * (255.0f / 5.0f))
			main_menu_options[4]->SetColor({ main_menu_options[4]->GetColor().r,main_menu_options[4]->GetColor().g,main_menu_options[4]->GetColor().b,(Uint8)alpha });

		if (alpha == 255.0f) {

			for (uint i = 0; i < 5; ++i) {
				main_menu_buttons[i]->SetInteraction(true);
				main_menu_options[i]->SetInteraction(true);
			}

			menuState = MenuState::AT_MAIN_MENU_;
			break;
		}
		break;

	case MenuState::AT_MAIN_MENU_:
		blit_cat = true;
		break;

	case MenuState::AT_SETTINGS_:
		blit_cat = false;
		if (App->render->camera.x < 0)
			App->render->camera.x += 250 * dt;
		break;

	case MenuState::AT_CREDITS_:
		blit_cat = false;
		if (App->render->camera.y > -(camera_start_position.y + 10) * App->win->GetScale())
			App->render->camera.x -= 250 * dt;
		break;

	case MenuState::NO_MENU_:
	default:
		break;
	}

	// Clouds transition
	/*
	UIImage_Info cloud;
	cloud.tex_name = GENERAL_;
	cloud.tex_area = { 0, 754, 208, 57 };
	UIImage* big_cloud = App->gui->CreateUIImage({ 0,0 }, cloud);
	cloud.tex_area = { 16, 832, 112, 27 };
	UIImage* medium_cloud = App->gui->CreateUIImage({ 0,0 }, cloud);
	cloud.tex_area = { 0, 877, 109, 23 };
	UIImage* small_cloud = App->gui->CreateUIImage({ 0,0 }, cloud);
	*/
	return ret;
}

void j1Menu::OnUIEvent(UIElement* UIelem, UIEvents UIevent)
{

	switch (UIevent) {

	case UIEvents::MOUSE_LEFT_CLICK_:

		for (uint i = 0; i < 5; ++i) {
			if (UIelem == main_menu_buttons[i]) {
				main_menu_options[i]->SetColor(main_menu_options[i]->GetColor(false, false, true));
			}
		}
		break;

	case UIEvents::MOUSE_LEFT_UP_:

		if (UIelem == main_menu_buttons[MenuOptions::MM_START_]) {
			menuCatState = MenuCatState::MC_START_;
			break;
		}
		else if (UIelem == main_menu_buttons[MenuOptions::MM_CONTINUE_]) {
			menuCatState = MenuCatState::MC_CONTINUE_;
			break;
		}
		else if (UIelem == main_menu_buttons[MenuOptions::MM_SETTINGS_]) {
			menuState = MenuState::AT_SETTINGS_;
			App->fade->FadeToBlack(this, this, 2.0f, slider_fade, false, false);
			break;
		}
		else if (UIelem == main_menu_buttons[MenuOptions::MM_CREDITS_]) {
			menuState = MenuState::AT_CREDITS_;
			App->fade->FadeToBlack(this, this, 2.0f, slider_fade, false, false);
			break;
		}
		else if (UIelem == main_menu_buttons[MenuOptions::MM_EXIT_]) {
			// QUIT GAME
			break;
		}

		for (uint i = 0; i < 5; ++i) {
			if (UIelem == main_menu_buttons[i]) {
				main_menu_buttons[i]->SetInteraction(false);
			}
		}
		break;

	case UIEvents::MOUSE_ENTER_:

		for (uint i = 0; i < 5; ++i) {
			if (UIelem == main_menu_buttons[i]) {
				main_menu_options[i]->SetColor(main_menu_options[i]->GetColor(false, true));
			}
		}
		break;

	case UIEvents::MOUSE_LEAVE_:

		for (uint i = 0; i < 5; ++i) {
			if (UIelem == main_menu_buttons[i]) {
				main_menu_options[i]->SetColor(main_menu_options[i]->GetColor(true));
			}
		}
		break;
	}
}

// Called before quitting
bool j1Menu::CleanUp()
{
	bool ret = true;

	App->entities->KillAllEntities();
	App->gui->ClearAllUI();

	cat = nullptr;
	press_any_button = nullptr;
	skip = nullptr;
	black_screen_image = nullptr;

	memset(title_letters, 0, 8);
	memset(main_menu_buttons, 0, 5);
	memset(main_menu_options, 0, 5);

	App->map->UnLoad();
	App->scene->active = true;

	return ret;
}