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
	label.font_name = Font_Names::ZELDA_;
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
	label.font_name = Font_Names::SOBAD_;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.text = "PRESS ANY BUTTON";
	press_any_button = App->gui->CreateUILabel({ (int)width / 2, (int)height - 150 }, label, this);
	press_any_button->SetColor({ press_any_button->GetColor().r,press_any_button->GetColor().g,press_any_button->GetColor().b,0 });

	// Skip button label
	label.font_name = Font_Names::SOBAD_8_;
	label.text = "SKIP";
	label.normal_color = LightGrey_;
	skip = App->gui->CreateUILabel({ 50, 20 }, label, this);
	skip->SetColor({ skip->GetColor().r,skip->GetColor().g,skip->GetColor().b,0 });

	// Main menu buttons
	UIButton_Info button;
	button.tex_name = Tex_Names::MAIN_MENU_;
	button.interactive = false;

	for (int i = 0; i < 5; ++i) {
		button.normal_tex_area = { 0,i * (21 + 2),81,21 };
		button.hover_tex_area = { 84,i * (21 + 2),81,21 };
		button.pressed_tex_area = { 168,i * (21 + 2),81,21 };
		main_menu_buttons[i] = App->gui->CreateUIButton({ 50,400 + 70 * i }, button, this);
	}
	i = 0;
	SDL_SetTextureAlphaMod((SDL_Texture*)App->gui->GetTexture(Tex_Names::MAIN_MENU_), 0);

	// Main menu options
	label.vertical_orientation = UIElement_VERTICAL_POS::MIDDLE_;
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
	entity.type = ENTITY_TYPES::CAT_;
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
	float skip_button_speed = 2.5f;
	float press_any_button_speed = 1.5f;

	float press_any_button_seconds = 2.0f;
	float scene_fade_seconds = 5.0f;
	float options_seconds = 3.0f;
	float title_opaque_seconds = 1.0f;

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

		if (print_title) {
			if (title_letters[0]->IntermitentFade(1.2f, false)) {
				if (title_letters[1]->IntermitentFade(1.2f, false)) {
					if (title_letters[2]->IntermitentFade(1.2f, false)) {
						if (title_letters[3]->IntermitentFade(1.2f, false)) {
							if (title_letters[4]->IntermitentFade(1.2f, false)) {
								if (title_letters[5]->IntermitentFade(1.2f, false)) {
									if (title_letters[6]->IntermitentFade(1.2f, false)) {
										if (title_letters[7]->IntermitentFade(1.2f, false)) {

											if (timer >= title_opaque_seconds) {
												for (uint j = 0; j < 8; ++j)
													title_letters[j]->SetColor(title_letters[j]->GetColor());

												menuState = MenuState::PRESS_ANY_BUTTON_;
												break;
											}
											else
												timer += 1.0f * dt;
										}
									}
								}
							}
						}
					}
				}
			}
		}		

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

		is_button_invisible = press_any_button->FromAlphaToAlphaFade(press_any_button->GetColor(false).a, 0.0f, scene_fade_seconds);
		is_black_screen_image_invisible = black_screen_image->FromAlphaToAlphaFade(black_screen_image->GetColor().a, 0.0f, scene_fade_seconds);

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
		else {
			if (settings_window->SlideTransition(dt, 500.0f, true, 20.0f)) {
				if (music_volume_text->IntermitentFade(1.0f, false, true)) {
					if (FX_volume_text->IntermitentFade(1.0f, false, true)) {
						if (fullscreen_text->IntermitentFade(1.0f, false, true)) {
							if (cap_frames_text->IntermitentFade(1.0f, false, true)) {
								if (camera_blit_text->IntermitentFade(1.0f, false, true)) {
									settings_window->SetInteraction(true);
									music_volume_text->SetInteraction(true);
									FX_volume_text->SetInteraction(true);
									fullscreen_text->SetInteraction(true);
									cap_frames_text->SetInteraction(true);
									camera_blit_text->SetInteraction(true);
									fullscreen_checkbox->SetInteraction(true);
									cap_frames_checkbox->SetInteraction(true);
									camera_blit_checkbox->SetInteraction(true);
								}
							}
						}
					}
				}
			}
		}
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
			App->gui->ClearAllUI();
			CreateSettingsUIElements();
			menuState = MenuState::AT_SETTINGS_;
			App->fade->FadeToBlack(this, this, 2.0f, slider_fade, false, false);
			break;
		}
		else if (UIelem == main_menu_buttons[MenuOptions::MM_CREDITS_]) {
			App->gui->ClearAllUI();
			menuState = MenuState::AT_CREDITS_;
			App->fade->FadeToBlack(this, this, 2.0f, slider_fade, false, false);
			break;
		}
		else if (UIelem == main_menu_buttons[MenuOptions::MM_EXIT_]) {
			// QUIT GAME
			break;
		}

		for (uint i = 0; i < 5; ++i) {
			if (main_menu_buttons[i] != nullptr) {
				if (UIelem == main_menu_buttons[i]) {
					main_menu_buttons[i]->SetInteraction(false);
				}
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

void j1Menu::CreateMainMenuUIElements() 
{

}

void j1Menu::CreateSettingsUIElements()
{
	// Window
	UIWindow_Info window;
	window.interactive = false;
	window.tex_name = Tex_Names::MENU_PAUSE_;
	settings_window = App->gui->CreateUIWindow({ 50, -500 }, window);

	// Options
	UILabel_Info label;
	label.interactive = false;
	label.font_name = Font_Names::SOBAD_;
	label.text = "Music volume";
	music_volume_text = App->gui->CreateUILabel({ 40,50 }, label, this, settings_window);
	music_volume_text->SetColor({ music_volume_text->GetColor().r,music_volume_text->GetColor().g,music_volume_text->GetColor().b,0 });
	label.text = "FX volume";
	FX_volume_text = App->gui->CreateUILabel({ 40,130 }, label, this, settings_window);
	FX_volume_text->SetColor({ FX_volume_text->GetColor().r,FX_volume_text->GetColor().g,FX_volume_text->GetColor().b,0 });
	label.text = "Fullscreen";
	fullscreen_text = App->gui->CreateUILabel({ 40,210 }, label, this, settings_window);
	fullscreen_text->SetColor({ fullscreen_text->GetColor().r,fullscreen_text->GetColor().g,fullscreen_text->GetColor().b,0 });
	label.text = "Cap frames to 30";
	cap_frames_text = App->gui->CreateUILabel({ 40,290 }, label, this, settings_window);
	cap_frames_text->SetColor({ cap_frames_text->GetColor().r,cap_frames_text->GetColor().g,cap_frames_text->GetColor().b,0 });
	label.text = "Camera blit";
	camera_blit_text = App->gui->CreateUILabel({ 40,370 }, label, this, settings_window);
	camera_blit_text->SetColor({ camera_blit_text->GetColor().r,camera_blit_text->GetColor().g,camera_blit_text->GetColor().b,0 });

	// Checkbox
	UIButton_Info checkbox;
	checkbox.interactive = false;
	checkbox.checkbox = true;
	checkbox.tex_name = Tex_Names::CHECKBOX_;
	checkbox.normal_tex_area = { 0,0,11,7 };
	checkbox.hover_tex_area = { 0,0,11,7 };
	checkbox.pressed_tex_area = { 12,0,11,7 };
	fullscreen_checkbox = App->gui->CreateUIButton({ 40,220 }, checkbox, this, fullscreen_text);
	cap_frames_checkbox = App->gui->CreateUIButton({ 40,300 }, checkbox, this, cap_frames_text);
	camera_blit_checkbox = App->gui->CreateUIButton({ 40,380 }, checkbox, this, camera_blit_text);
}