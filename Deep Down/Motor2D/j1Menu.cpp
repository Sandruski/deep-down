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
#include "j1Audio.h"
#include "j1Fonts.h"
#include "j1BetweenTransitions.h"

#include "UIButton.h"
#include "UICursor.h"
#include "UIImage.h"
#include "UILabel.h"
#include "UIWindow.h"
#include "UISlider.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

#include"Brofiler\Brofiler.h"

#include "UILifeBar.h"

#ifdef _WIN32	
#include <shellapi.h>
void open_url(const string& url)
{
	ShellExecute(GetActiveWindow(),"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
#else
void open_url(const string& url) {}
#endif

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

	skip_button_speed = config.child("skip_button_speed").attribute("value").as_float();
	press_any_button_speed = config.child("press_any_button_speed").attribute("value").as_float();
	press_any_button_seconds = config.child("press_any_button_seconds").attribute("value").as_float();
	scene_fade_seconds = config.child("scene_fade_seconds").attribute("value").as_float();
	start_fade_seconds = config.child("start_fade_seconds").attribute("value").as_float();
	continue_fade_seconds = config.child("continue_fade_seconds").attribute("value").as_float();
	options_seconds = config.child("options_seconds").attribute("value").as_float();
	title_opaque_seconds = config.child("title_opaque_seconds").attribute("value").as_float();
	
	camera_speed = config.child("camera_speed").attribute("value").as_float();
	cat_run = config.child("cat_run").attribute("value").as_float();
	cat_jump.x = config.child("cat_jump").attribute("x").as_float();
	cat_jump.y = config.child("cat_jump").attribute("y").as_float();
	
	cat_width_roll.x = config.child("cat_width_roll").attribute("x").as_float();
	cat_width_roll.y = config.child("cat_width_roll").attribute("y").as_float();

	cat_height_roll.x = config.child("cat_height_roll").attribute("x").as_float();
	cat_height_roll.y = config.child("cat_height_roll").attribute("y").as_float();

	cat_roll.x = config.child("cat_roll").attribute("x").as_float();
	cat_roll.y = config.child("cat_roll").attribute("y").as_float();
	seconds_stop_jumping = config.child("seconds_stop_jumping").attribute("value").as_float();
	cat_seconds = config.child("cat_seconds").attribute("value").as_float();

	cat_final_position.x = config.child("cat_final_pos").attribute("x").as_int();
	cat_final_position.y = config.child("cat_final_pos").attribute("y").as_int();
	
	cam_start_pos.x = config.child("camera_start_tile").attribute("x").as_int();
	cam_start_pos.y = config.child("camera_start_tile").attribute("y").as_int();

	title_pos.x = config.child("title_pos").attribute("x").as_int();
	title_pos.y = config.child("title_pos").attribute("y").as_int();
	title_track.x = config.child("title_track").attribute("x").as_int();
	title_track.y = config.child("title_track").attribute("y").as_int();
	
	return ret;
}

// Called before the first frame
bool j1Menu::Start()
{
	bool ret = true;
	continue_button = false;

	// Set Default Score
	App->scene->cats_first_map = 0;
	App->scene->cats_second_map = 0;

	// Load menu map
	if (App->map->Load("menu.tmx"))
		App->entities->AddEntities();

	App->audio->PlayMusic("MenuTheme.ogg");
	App->audio->SetMusicVolume(MENU_THEME_VOLUME);

	// Get screen parameters
	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();

	// Set camera position
	camera_start_position = { cam_start_pos.x,cam_start_pos.y };
	camera_start_position = App->map->MapToWorld(camera_start_position.x, camera_start_position.y);
	App->render->camera.x = -(camera_start_position.x + CAMERA_OFFSET) * scale;
	App->render->camera.y = camera_start_position.y * scale;

	// Create UI elements
	// Black quad
	if (!App->trans->back_to_main_menu) {
		UIImage_Info black_screen;
		black_screen.color = Black_;
		black_screen.quad = true;
		black_screen.quad_area = { 0, 0, (int)(width * scale), (int)(height * scale) };
		black_screen_image = App->gui->CreateUIImage({ 0,0 }, black_screen);
	}

	// Game title
	UILabel_Info label;
	label.font_name = Font_Names::ZELDA_;
	label.interactive = false;
	label.draggable = false;

	label.text = "D";
	iPoint title_position = { title_pos.x * scale,title_pos.y * scale };
	iPoint tracking = { title_track.x,title_track.y };
	uint i = 0;
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
	label.font_name = Font_Names::MSMINCHO_;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.text = "PRESS ANY BUTTON";
	press_any_button = App->gui->CreateUILabel({ (int)width / 2, (int)height - 150 }, label, this);
	press_any_button->SetColor({ press_any_button->GetColor().r,press_any_button->GetColor().g,press_any_button->GetColor().b,0 });

	// Skip button label
	label.font_name = Font_Names::MSMINCHO_;
	label.text = "SKIP";
	label.normal_color = LightGrey_;
	skip = App->gui->CreateUILabel({ 60, 20 }, label, this);
	skip->SetColor({ skip->GetColor().r,skip->GetColor().g,skip->GetColor().b,0 });

	// Main menu buttons
	UIButton_Info button;
	button.interactive = false;
	button.draggable = false;
	button.normal_tex_area = UIElement_Rect::MM_OPT_1_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_1_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_1_PRESSED_;
	main_menu_buttons[i] = App->gui->CreateUIButton({ 50,400 }, button, this);

	pugi::xml_document config_file;
	if (pugi::xml_parse_result result = config_file.load_file("save_game.xml")) {
		button.normal_tex_area = UIElement_Rect::MM_OPT_2_NORMAL_;
		button.hover_tex_area = UIElement_Rect::MM_OPT_2_HOVER_;
		button.pressed_tex_area = UIElement_Rect::MM_OPT_2_PRESSED_;
		main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 }, button, this);
		continue_button = true;
	}
	else {
		++i;
	}

	button.normal_tex_area = UIElement_Rect::MM_OPT_3_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_3_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_3_PRESSED_;
	main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 * 2 }, button, this);
	button.normal_tex_area = UIElement_Rect::MM_OPT_4_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_4_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_4_PRESSED_;
	main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 * 3 }, button, this);
	button.normal_tex_area = UIElement_Rect::MM_OPT_5_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_5_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_5_PRESSED_;
	main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 * 4 }, button, this);
	i = 0;
	App->gui->SetTextureAlphaMod(0);

	// Main menu options
	label.interaction_from_father = true;
	label.vertical_orientation = UIElement_VERTICAL_POS::MIDDLE_;
	label.normal_color = Purple_;
	label.hover_color = Pink_;
	label.pressed_color = LightPink_;

	label.text = "Play";
	iPoint options_position = { main_menu_buttons[i]->GetLocalRect().w * scale / 2,main_menu_buttons[i]->GetLocalRect().h * scale / 2 };
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[i]);

	if (continue_button) {
		label.text = "Continue";
		main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	}
	else {
		++i;
	}
	label.text = "Settings";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	label.text = "Credits";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	label.text = "Exit";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	i = 0;

	for (i; i < 5; ++i) {
		if (i == 1) {
			if (continue_button)
				main_menu_options[i]->SetColor({ main_menu_options[i]->GetColor().r,main_menu_options[i]->GetColor().g,main_menu_options[i]->GetColor().b,0 });
		}
		else
			main_menu_options[i]->SetColor({ main_menu_options[i]->GetColor().r,main_menu_options[i]->GetColor().g,main_menu_options[i]->GetColor().b,0 });
	}
	i = 0;

	// Highscore
	label.interaction_from_father = false;
	label.font_name = Font_Names::MSMINCHO_;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	label.vertical_orientation = UIElement_VERTICAL_POS::TOP_;
	label.normal_color = White_;
	label.text = "LAST SCORE";
	highscore_text = App->gui->CreateUILabel({ (int)width - 50, -500 }, label);
	int font_width, font_height;
	App->font->CalcSize(label.text.GetString(), font_width, font_height, App->gui->GetFont(Font_Names::MSMINCHO_));
	highscore_text->DecreasePos({ font_width * scale, font_height * scale });
	p2SString tmp("%i", App->trans->highscore);
	label.text = tmp.GetString();
	highscore_value = App->gui->CreateUILabel({ (int)width - 50, -500 }, label);
	App->font->CalcSize(label.text.GetString(), font_width, font_height, App->gui->GetFont(Font_Names::MSMINCHO_));
	highscore_value->DecreasePos({ font_width * scale, font_height * scale });
	//_create_UI_elements

	if (App->trans->back_to_main_menu) {
		App->entities->Start();
		App->entities->playerData = nullptr;
		highscore_text->ResetFade();
		highscore_value->ResetFade();
		camera_moved = true;
	}

	// Cat
	EntityInfo entity;
	entity.type = ENTITY_TYPES::CAT_;
	entity.position = { camera_start_position.x,50 };
	cat = (Cat*)App->entities->SpawnEntity(entity);

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

	if (App->trans->back_to_main_menu) {
		cat->position = { (float)cat_final_position.x, (float)cat_final_position.y };
		menuCatState = MenuCatState::MC_AT_GROUND_;
		menuState = MenuState::MAIN_MENU_OPTIONS_ANIMATION_;
	}
	else {
		menuCatState = MenuCatState::MC_APPEAR_RUNNING_;
		menuState = MenuState::TITLE_ANIMATION_;
	}

	return ret;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->quit_game = true;

	if (swap_music && music_slider != nullptr) {
		int volume = music_slider->GetPercent();
		volume = MAX_VOLUME * volume / 100;
		App->audio->SetMusicVolume(volume);
	}
	else if (swap_fx && App->audio->active && FX_slider != nullptr) {
		int volume = FX_slider->GetPercent();
		volume = MAX_VOLUME * volume / 100;
		App->audio->SetFxVolume(volume);
	}

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

			App->scene->index = 0;
			App->trans->continue_game = false;

			App->trans->SetNextTransitionInfo(0, true);
			App->fade->FadeToBlack(this, App->scene, start_fade_seconds, fades::slider_fade);
			break;
		}
		break;

	case MenuCatState::MC_CONTINUE_:
		cat->SetCatState(CatState::rc_run);
		cat->position.x += cat_run * dt;

		if (cat->position.x >= camera_start_position.x + cat_position_increment[7]) {
			menuCatState = MenuCatState::NO_CAT_;

			App->trans->continue_game = true;

			App->fade->FadeToBlack(this, App->scene, continue_fade_seconds, fades::slider_fade);
			break;
		}
		break;

	case MenuCatState::NO_CAT_:
	default:
		break;
	}

	float alpha = 0.0f;
	bool is_button_invisible = false;
	bool is_black_screen_image_invisible = false;
	bool first_letter = false;
	bool second_letter = false;
	bool third_letter = false;
	bool fourth_letter = false;
	bool fifth_letter = false;
	bool sixth_letter = false;
	bool seventh_letter = false;
	bool eighth_letter = false;

	// Update menu
	switch (menuState) {

	case MenuState::TITLE_ANIMATION_:

		// Skip
		if (menuCatState > MenuCatState::NO_CAT_) {

			if (App->input->IsAnyKeyPressed()) {
				skip->SetColor({ skip->GetColor().r,skip->GetColor().g,skip->GetColor().b,0 });

				for (uint i = 0; i < 8; ++i)
					title_letters[i]->SetColor(title_letters[i]->GetColor());

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
			if (title_letters[0]->IntermitentFade(INTERMITENT_FADE, false)) {
				if (title_letters[1]->IntermitentFade(INTERMITENT_FADE, false)) {
					if (title_letters[2]->IntermitentFade(INTERMITENT_FADE, false)) {
						if (title_letters[3]->IntermitentFade(INTERMITENT_FADE, false)) {
							if (title_letters[4]->IntermitentFade(INTERMITENT_FADE, false)) {
								if (title_letters[5]->IntermitentFade(INTERMITENT_FADE, false)) {
									if (title_letters[6]->IntermitentFade(INTERMITENT_FADE, false)) {
										if (title_letters[7]->IntermitentFade(INTERMITENT_FADE, false)) {

											if (timer >= title_opaque_seconds) {
												for (uint i = 0; i < 8; ++i)
													title_letters[i]->SetColor(title_letters[i]->GetColor());

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

		if (from_settings || from_credits) {
			if (from_settings) {
				if (App->render->camera.x > -(camera_start_position.x + CAMERA_OFFSET) * scale) {
					if (camera_speed * dt > 1)
						App->render->camera.x -= camera_speed * dt;
					else
						App->render->camera.x -= 1;
				}
				else {
					if (App->render->camera.x < -(camera_start_position.x + CAMERA_OFFSET) * scale)
						App->render->camera.x = -(camera_start_position.x + CAMERA_OFFSET) * scale;
					camera_moved = true;
				}
			}
			else if (from_credits) {
				if (App->render->camera.y < 0) {
					if (camera_speed * dt > 1)
						App->render->camera.y += camera_speed * dt;
					else
						App->render->camera.y += 1;
				}
				else {
					if (App->render->camera.y > 0)
						App->render->camera.y = 0;
					camera_moved = true;
				}
			}
		}

		if (camera_moved) {
			blit_cat = true;

			alpha = App->gui->IncreaseDecreaseAlpha(0.0f, MAX_ALPHA, options_seconds);
			App->gui->SetTextureAlphaMod(alpha);

			main_menu_options[0]->SetColor({ main_menu_options[0]->GetColor().r,main_menu_options[0]->GetColor().g,main_menu_options[0]->GetColor().b,(Uint8)alpha });
			if (alpha >= 1.0f * (MAX_ALPHA / 5.0f) && continue_button) {
				main_menu_options[1]->SetColor({ main_menu_options[1]->GetColor().r,main_menu_options[1]->GetColor().g,main_menu_options[1]->GetColor().b,(Uint8)alpha });
			}
			if (alpha >= 2.0f * (MAX_ALPHA / 5.0f))
				main_menu_options[2]->SetColor({ main_menu_options[2]->GetColor().r,main_menu_options[2]->GetColor().g,main_menu_options[2]->GetColor().b,(Uint8)alpha });
			if (alpha >= 3.0f * (MAX_ALPHA / 5.0f))
				main_menu_options[3]->SetColor({ main_menu_options[3]->GetColor().r,main_menu_options[3]->GetColor().g,main_menu_options[3]->GetColor().b,(Uint8)alpha });
			if (alpha >= 4.0f * (MAX_ALPHA / 5.0f))
				main_menu_options[4]->SetColor({ main_menu_options[4]->GetColor().r,main_menu_options[4]->GetColor().g,main_menu_options[4]->GetColor().b,(Uint8)alpha });

			if (alpha == MAX_ALPHA) {

				first_letter = title_letters[0]->IntermitentFade(INTERMITENT_FADE, false, true);
				second_letter = title_letters[1]->IntermitentFade(INTERMITENT_FADE, false, true);
				third_letter = title_letters[2]->IntermitentFade(INTERMITENT_FADE, false, true);
				fourth_letter = title_letters[3]->IntermitentFade(INTERMITENT_FADE, false, true);
				fifth_letter = title_letters[4]->IntermitentFade(INTERMITENT_FADE, false, true);
				sixth_letter = title_letters[5]->IntermitentFade(INTERMITENT_FADE, false, true);
				seventh_letter = title_letters[6]->IntermitentFade(INTERMITENT_FADE, false, true);
				eighth_letter = title_letters[7]->IntermitentFade(INTERMITENT_FADE, false, true);

				if (first_letter && second_letter && third_letter && fourth_letter && fifth_letter && sixth_letter && seventh_letter && eighth_letter) {

					if (highscore_value->SlideTransition(dt, (int)height / 6, SLIDE_FAST_SPEED, true, 10.0f, 3.0f)) {
						if (highscore_text->SlideTransition(dt, (int)height / 10, SLIDE_FAST_SPEED, true, 10.0f, 3.0f)) {
							for (uint i = 0; i < 5; ++i) {
								if (i == 1) {
									if (continue_button) {
										main_menu_buttons[i]->SetInteraction(true);
										main_menu_options[i]->SetInteraction(true);
									}
								}
								else {
									main_menu_options[i]->SetInteraction(true);
									main_menu_buttons[i]->SetInteraction(true);
								}
							}

							from_settings = false;
							from_credits = false;
							camera_moved = false;
							App->trans->back_to_main_menu = false;

							menuState = MenuState::AT_MAIN_MENU_;
							break;
						}
					}
				}
			}
		}
		else if (!from_settings && !from_credits && !App->trans->back_to_main_menu) {
			alpha = App->gui->IncreaseDecreaseAlpha(0.0f, MAX_ALPHA, options_seconds);
			App->gui->SetTextureAlphaMod(alpha);

			main_menu_options[0]->SetColor({ main_menu_options[0]->GetColor().r,main_menu_options[0]->GetColor().g,main_menu_options[0]->GetColor().b,(Uint8)alpha });
			if (alpha >= 1.0f * (MAX_ALPHA / 5.0f) && continue_button)
				main_menu_options[1]->SetColor({ main_menu_options[1]->GetColor().r,main_menu_options[1]->GetColor().g,main_menu_options[1]->GetColor().b,(Uint8)alpha });
			if (alpha >= 2.0f * (MAX_ALPHA / 5.0f))
				main_menu_options[2]->SetColor({ main_menu_options[2]->GetColor().r,main_menu_options[2]->GetColor().g,main_menu_options[2]->GetColor().b,(Uint8)alpha });
			if (alpha >= 3.0f * (MAX_ALPHA / 5.0f))
				main_menu_options[3]->SetColor({ main_menu_options[3]->GetColor().r,main_menu_options[3]->GetColor().g,main_menu_options[3]->GetColor().b,(Uint8)alpha });
			if (alpha >= 4.0f * (MAX_ALPHA / 5.0f))
				main_menu_options[4]->SetColor({ main_menu_options[4]->GetColor().r,main_menu_options[4]->GetColor().g,main_menu_options[4]->GetColor().b,(Uint8)alpha });

			if (alpha == 255.0f) {

				if (highscore_value->SlideTransition(dt, (int)height / 6, SLIDE_FAST_SPEED, true, 10.0f, 3.0f)) {
					if (highscore_text->SlideTransition(dt, (int)height / 10, SLIDE_FAST_SPEED, true, 10.0f, 3.0f)) {

						for (uint i = 0; i < 5; ++i) {
							if (i == 1) {
								if (continue_button) {
									main_menu_buttons[i]->SetInteraction(true);
									main_menu_options[i]->SetInteraction(true);
								}
							}
							else {
								main_menu_options[i]->SetInteraction(true);
								main_menu_buttons[i]->SetInteraction(true);
							}
						}

						menuState = MenuState::AT_MAIN_MENU_;
						break;
					}
				}
			}
		}
		break;

	case MenuState::AT_MAIN_MENU_:
		
		break;

	case MenuState::AT_SETTINGS_:
		blit_cat = false;

		if (App->render->camera.x < -10) {
			if (camera_speed * dt > 1)
				App->render->camera.x += camera_speed * dt;
			else
				App->render->camera.x += 1;
		}
		else {
			if (!settings_done) {
				if (App->render->camera.x > -10)
					App->render->camera.x = 0;

				if (settings_window->SlideTransition(dt, height / 2, SLIDE_SLOW_SPEED, true, 20.0f, 2.0f)) {
					if (music_volume_text->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
						if (FX_volume_text->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
							if (fullscreen_text->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
								if (cap_frames_text->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
									if (camera_blit_text->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
										if (back_to_main_menu_from_settings->SlideTransition(dt, height - 50, SLIDE_SLOW_SPEED, true, 10.0f, 2.0f, false)) {
											settings_window->SetInteraction(true);
											fullscreen_checkbox->SetInteraction(true);
											cap_frames_checkbox->SetInteraction(true);
											camera_blit_checkbox->SetInteraction(true);
											back_to_main_menu_from_settings->SetInteraction(true);
											FX_slider->SetInteraction(true);
											music_slider->SetInteraction(true);
											settings_done = true;
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

	case MenuState::AT_CREDITS_:
		blit_cat = false;
		
		// Text slider
		if (sliding) {
			int percent = license_slider->GetPercent();
			percent = license_description->GetLocalRect().w * percent / 100;
			license_description->SetLocalPos({ license_description->GetLocalPos().x, -percent / 3 + 20 });
		}

		if (App->render->camera.y > -(int)height - 2 * 16 * scale) {
			if (camera_speed * dt > 1)
				App->render->camera.y -= camera_speed * dt;
			else
				App->render->camera.y -= 1;
		}
		else {
			if (!credits_done) {
				if (App->render->camera.y < -(int)height - 2 * 16 * scale)
					App->render->camera.y = -(int)height - 2 * 16 * scale;

				if (license_slider->SlideTransition(dt, (int)height / 2 - (int)height / 5, SLIDE_FAST_SPEED, true, 10.0f, 2.0f, false)) {
					if (credits_window->SlideTransition(dt, (int)height / 2 + (int)height / 8, SLIDE_SLOW_SPEED, true, 20.0f, 2.0f, false)) {
						if (license_title->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
							if (authors_title->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
								if (authors_description->IntermitentFade(CS_INTERMITENT_FADE, false, true)) {
									if (website_button->SlideTransition(dt, (int)height / 2 + (int)height / 6, SLIDE_FAST_SPEED, true, 10.0f, 2.0f, false)) {
										if (back_to_main_menu_from_credits->SlideTransition(dt, 50, SLIDE_SLOW_SPEED, true, 10.0f, 2.0f)) {
											website_button->SetInteraction(true);
											website_title->SetInteraction(true);
											back_to_main_menu_from_credits->SetInteraction(true);
											license_slider->SetInteraction(true);
											credits_done = true;
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

	case MenuState::NO_MENU_:
	default:
		break;
	}

	return ret;
}

void j1Menu::OnUIEvent(UIElement* UIelem, UIEvents UIevent)
{
	switch (UIevent) {

	case UIEvents::MOUSE_LEFT_CLICK_:

		for (uint i = 0; i < 5; ++i)
		{
			if (i == 1) {
				if (continue_button) {
					if (UIelem == main_menu_buttons[i]) {
						App->audio->PlayFx(10);
						continue;
					}
				}
			}
			else {
				if (UIelem == main_menu_buttons[i]) {
					App->audio->PlayFx(10);
					continue;
				}
			}
		}

		if (UIelem == (UIElement*)license_slider) {
			sliding = true;
			break;
		}

		else if (UIelem == (UIElement*)fullscreen_checkbox)
		{
			if (App->win->fullscreen) {
				App->win->fullscreen = false;
				SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_SHOWN);
				break;
			}
			else {
				App->win->fullscreen = true;
				SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				break;
			}
		}

		else if (UIelem == (UIElement*)cap_frames_checkbox)
		{
			App->toCap = !App->toCap;
			break;
		}

		else if (UIelem == (UIElement*)camera_blit_checkbox)
		{
			App->map->camera_blit = !App->map->camera_blit;
			break;
		}

		else if (UIelem == (UIElement*)music_slider) {
			swap_music = true;
			break;
		}
		else if (UIelem == (UIElement*)FX_slider) {
			swap_fx = true;
			break;
		}

		break;

	case UIEvents::MOUSE_LEFT_UP_:

		// Main menu
		if (UIelem == (UIElement*)main_menu_buttons[MenuOptions::MM_START_]) {
			for (uint i = 0; i < 5; ++i) {
				if (i == 1) {
					if (continue_button) {
						main_menu_buttons[i]->SetInteraction(false);
						main_menu_options[i]->SetInteraction(false);
						main_menu_buttons[i] = nullptr;
						main_menu_options[i] = nullptr;
					}
				}
				else {
					main_menu_buttons[i]->SetInteraction(false);
					main_menu_options[i]->SetInteraction(false);
					main_menu_buttons[i] = nullptr;
					main_menu_options[i] = nullptr;
				}
			}

			menuCatState = MenuCatState::MC_START_;
			break;
		}
		else if (UIelem == (UIElement*)main_menu_buttons[MenuOptions::MM_CONTINUE_] && continue_button) {

			for (uint i = 0; i < 5; ++i) {
				if (i == 1) {
					if (continue_button) {
						main_menu_buttons[i]->SetInteraction(false);
						main_menu_options[i]->SetInteraction(false);
						main_menu_buttons[i] = nullptr;
						main_menu_options[i] = nullptr;
					}
				}
				else {
					main_menu_buttons[i]->SetInteraction(false);
					main_menu_options[i]->SetInteraction(false);
					main_menu_buttons[i] = nullptr;
					main_menu_options[i] = nullptr;
				}
			}

			menuCatState = MenuCatState::MC_CONTINUE_;
			break;
		}
		else if (UIelem == (UIElement*)main_menu_buttons[MenuOptions::MM_SETTINGS_]) {
			for (uint i = 0; i < 5; ++i) {
				if (i == 1) {
					if (continue_button) {
						main_menu_buttons[i]->SetInteraction(false);
						main_menu_options[i]->SetInteraction(false);
						main_menu_buttons[i] = nullptr;
						main_menu_options[i] = nullptr;
					}
				}
				else {
					main_menu_buttons[i]->SetInteraction(false);
					main_menu_options[i]->SetInteraction(false);
					main_menu_buttons[i] = nullptr;
					main_menu_options[i] = nullptr;
				}
			}

			App->gui->ClearAllUI();
			CreateSettingsUIElements();
			menuState = MenuState::AT_SETTINGS_;
			break;
		}
		else if (UIelem == (UIElement*)main_menu_buttons[MenuOptions::MM_CREDITS_]) {
			for (uint i = 0; i < 5; ++i) {
				if (i == 1) {
					if (continue_button) {
						main_menu_buttons[i]->SetInteraction(false);
						main_menu_options[i]->SetInteraction(false);
						main_menu_buttons[i] = nullptr;
						main_menu_options[i] = nullptr;
					}
				}
				else {
					main_menu_buttons[i]->SetInteraction(false);
					main_menu_options[i]->SetInteraction(false);
					main_menu_buttons[i] = nullptr;
					main_menu_options[i] = nullptr;
				}
			}

			App->gui->ClearAllUI();
			CreateCreditsUIElements();
			menuState = MenuState::AT_CREDITS_;
			break;
		}
		else if (UIelem == (UIElement*)main_menu_buttons[MenuOptions::MM_EXIT_]) {
			App->quit_game = true;
			break;
		}

		// Settings
		else if (UIelem == (UIElement*)back_to_main_menu_from_settings) {
			settings_window->SetInteraction(false);
			fullscreen_checkbox->SetInteraction(false);
			cap_frames_checkbox->SetInteraction(false);
			camera_blit_checkbox->SetInteraction(false);
			back_to_main_menu_from_settings->SetInteraction(false);
			FX_slider->SetInteraction(false);
			music_slider->SetInteraction(false);

			settings_window = nullptr;
			fullscreen_checkbox = nullptr;
			cap_frames_checkbox = nullptr;
			camera_blit_checkbox = nullptr;
			back_to_main_menu_from_settings = nullptr;
			FX_slider = nullptr;
			music_slider = nullptr;

			App->gui->ClearAllUI();
			CreateMainMenuUIElements();
			from_settings = true;
			settings_done = false;
			menuState = MenuState::MAIN_MENU_OPTIONS_ANIMATION_;
			break;
		}

		// Credits
		else if (UIelem == (UIElement*)back_to_main_menu_from_credits) {
			website_button->SetInteraction(false);
			website_title->SetInteraction(false);
			back_to_main_menu_from_credits->SetInteraction(false);
			license_slider->SetInteraction(false);

			website_button = nullptr;
			website_title = nullptr;
			back_to_main_menu_from_credits = nullptr;
			license_slider = nullptr;

			App->gui->ClearAllUI();
			CreateMainMenuUIElements();
			from_credits = true;
			credits_done = false;
			menuState = MenuState::MAIN_MENU_OPTIONS_ANIMATION_;
			break;
		}		
		else if (UIelem == (UIElement*)website_button)
		{
			open_url("https://sandruski.github.io/Deep-Down-Game/");
			break;
		}
		else if (UIelem == (UIElement*)license_slider) {
			sliding = false;
			break;
		}
		else if (UIelem == (UIElement*)music_slider) {
			swap_music = false;
			break;
		}
		else if (UIelem == (UIElement*)FX_slider) {
			swap_fx = false;
			break;
		}

		break;

	case UIEvents::MOUSE_ENTER_:
		for (uint i = 0; i < 5; ++i)
		{
			if (i == 1) {
				if (continue_button) {
					if (UIelem == (UIElement*)main_menu_buttons[i]) {
						App->audio->PlayFx(9);
						continue;
					}
				}
			}
			else {
				if (UIelem == (UIElement*)main_menu_buttons[i]) {
					App->audio->PlayFx(9);
					continue;
				}
			}
		}
		break;

	case UIEvents::MOUSE_LEAVE_:

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

	App->map->UnLoad();
	App->scene->active = true;

	return ret;
}

void j1Menu::CreateMainMenuUIElements() 
{
	App->gui->ResetAlpha();

	// Game title
	UILabel_Info label;
	label.font_name = Font_Names::ZELDA_;
	label.interactive = false;

	label.text = "D";
	iPoint title_position = { 70 * scale,55 * scale };
	iPoint tracking = { 5,12 };
	uint i = 0;
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

	// Main menu buttons
	UIButton_Info button;
	button.interactive = false;
	button.draggable = false;
	button.normal_tex_area = UIElement_Rect::MM_OPT_1_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_1_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_1_PRESSED_;
	main_menu_buttons[i] = App->gui->CreateUIButton({ 50,400 }, button, this);

	pugi::xml_document config_file;
	if (pugi::xml_parse_result result = config_file.load_file("save_game.xml")) {
		button.normal_tex_area = UIElement_Rect::MM_OPT_2_NORMAL_;
		button.hover_tex_area = UIElement_Rect::MM_OPT_2_HOVER_;
		button.pressed_tex_area = UIElement_Rect::MM_OPT_2_PRESSED_;
		main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 }, button, this);
		continue_button = true;
	}
	else {
		++i;
		continue_button = false;
	}

	button.normal_tex_area = UIElement_Rect::MM_OPT_3_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_3_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_3_PRESSED_;
	main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 * 2 }, button, this);
	button.normal_tex_area = UIElement_Rect::MM_OPT_4_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_4_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_4_PRESSED_;
	main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 * 3 }, button, this);
	button.normal_tex_area = UIElement_Rect::MM_OPT_5_NORMAL_;
	button.hover_tex_area = UIElement_Rect::MM_OPT_5_HOVER_;
	button.pressed_tex_area = UIElement_Rect::MM_OPT_5_PRESSED_;
	main_menu_buttons[++i] = App->gui->CreateUIButton({ 50,400 + 70 * 4 }, button, this);
	i = 0;
	App->gui->SetTextureAlphaMod(0);

	// Main menu options
	label.interaction_from_father = true;
	label.font_name = Font_Names::MSMINCHO_;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.vertical_orientation = UIElement_VERTICAL_POS::MIDDLE_;
	label.normal_color = Purple_;
	label.hover_color = Pink_;
	label.pressed_color = LightPink_;

	label.text = "Play";
	iPoint options_position = { main_menu_buttons[i]->GetLocalRect().w * scale / 2,main_menu_buttons[i]->GetLocalRect().h * scale / 2 };
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[i]);

	if (continue_button) {
		label.text = "Continue";
		main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	}
	else {
		++i;
	}

	label.text = "Settings";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	label.text = "Credits";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	label.text = "Exit";
	main_menu_options[i] = App->gui->CreateUILabel({ options_position.x,options_position.y }, label, this, main_menu_buttons[++i]);
	i = 0;

	for (i; i < 5; ++i) {
		if (i == 1) {
			if (continue_button)
				main_menu_options[i]->SetColor({ main_menu_options[i]->GetColor().r,main_menu_options[i]->GetColor().g,main_menu_options[i]->GetColor().b,0 });
		}
		else
			main_menu_options[i]->SetColor({ main_menu_options[i]->GetColor().r,main_menu_options[i]->GetColor().g,main_menu_options[i]->GetColor().b,0 });
	}
	i = 0;

	// Highscore
	label.font_name = Font_Names::MSMINCHO_;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	label.vertical_orientation = UIElement_VERTICAL_POS::TOP_;
	label.normal_color = White_;
	label.text = "LAST SCORE";
	highscore_text = App->gui->CreateUILabel({ (int)width - 50, -500 }, label);
	int font_width, font_height;
	App->font->CalcSize(label.text.GetString(), font_width, font_height, App->gui->GetFont(Font_Names::MSMINCHO_));
	highscore_text->DecreasePos({ font_width * scale, font_height * scale });
	p2SString tmp("%i", App->trans->highscore);
	label.text = tmp.GetString();
	highscore_value = App->gui->CreateUILabel({ (int)width - 50, -500 }, label);
	App->font->CalcSize(label.text.GetString(), font_width, font_height, App->gui->GetFont(Font_Names::MSMINCHO_));
	highscore_value->DecreasePos({ font_width * scale, font_height * scale });
	//_create_UI_elements
}

void j1Menu::CreateSettingsUIElements()
{
	// Window
	UIWindow_Info window;
	window.interactive = false;
	window.tex_area = UIElement_Rect::BIG_WINDOW_;
	settings_window = App->gui->CreateUIWindow({ 50, -500 }, window, this);

	// Options
	UILabel_Info label;
	label.interactive = false;
	label.draggable = false;
	label.normal_color = Purple_;
	label.hover_color = Pink_;
	label.pressed_color = LightPink_;
	label.font_name = Font_Names::MSMINCHO_;
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
	checkbox.normal_tex_area = UIElement_Rect::CHECKBOX_NORMAL_;
	checkbox.hover_tex_area = UIElement_Rect::CHECKBOX_NORMAL_;
	checkbox.pressed_tex_area = UIElement_Rect::CHECKBOX_PRESSED_;

	if (App->win->fullscreen)
		checkbox.checkbox_checked = true;

	fullscreen_checkbox = App->gui->CreateUIButton({ 300,fullscreen_text->GetLocalPos().y + fullscreen_text->GetLocalRect().h }, checkbox, this, settings_window);

	if (App->toCap)
		checkbox.checkbox_checked = true;
	else
		checkbox.checkbox_checked = false;

	cap_frames_checkbox = App->gui->CreateUIButton({ 400,cap_frames_text->GetLocalPos().y + cap_frames_text->GetLocalRect().h }, checkbox, this, settings_window);

	if (App->map->camera_blit)
		checkbox.checkbox_checked = true;
	else
		checkbox.checkbox_checked = false;

	camera_blit_checkbox = App->gui->CreateUIButton({ 300,camera_blit_text->GetLocalPos().y + camera_blit_text->GetLocalRect().h }, checkbox, this, settings_window);

	// Sliders
	UISlider_Info slider;
	slider.draggable = false;
	slider.tex_area = UIElement_Rect::SLIDER_BAR_;
	slider.button_slider_area = UIElement_Rect::SLIDER_BUTTON_;
	slider.offset = 3;
	slider.slider_button_pos.x = App->audio->music_volume * 54 / 128;
	slider.buggy_offset = -1;
	music_slider = App->gui->CreateUISlider({ 300,music_volume_text->GetLocalPos().y + 10 }, slider, this, settings_window);
	slider.slider_button_pos.x = App->audio->fx_volume * 54 / 128;
	FX_slider = App->gui->CreateUISlider({ 300,FX_volume_text->GetLocalPos().y + 10 }, slider, this, settings_window);

	// Back to main menu button
	UIButton_Info button;
	button.interactive = false;
	button.normal_tex_area = UIElement_Rect::ARROW_LEFT_NORMAL_;
	button.hover_tex_area = UIElement_Rect::ARROW_LEFT_HOVER_;
	button.pressed_tex_area = UIElement_Rect::ARROW_LEFT_PRESSED_;
	back_to_main_menu_from_settings = App->gui->CreateUIButton({ (int)width - 100, (int)height + 200 }, button, this);
}

void j1Menu::CreateCreditsUIElements() 
{
	// Window
	UIWindow_Info window;
	window.interactive = false;
	window.tex_area = UIElement_Rect::SMALL_WINDOW_;
	credits_window = App->gui->CreateUIWindow({ (int)width - 100 - (App->gui->GetRectFromAtlas(UIElement_Rect::SMALL_WINDOW_).w * scale), (int)height + 500 }, window, this);

	// Back to main menu button
	UIButton_Info button;
	button.interactive = false;
	button.normal_tex_area = UIElement_Rect::ARROW_UP_NORMAL_;
	button.hover_tex_area = UIElement_Rect::ARROW_UP_HOVER_;
	button.pressed_tex_area = UIElement_Rect::ARROW_UP_PRESSED_;
	back_to_main_menu_from_credits = App->gui->CreateUIButton({ 50, -200 }, button, this);

	// Website button
	button.normal_tex_area = UIElement_Rect::WEB_BUTTON_NORMAL_;
	button.hover_tex_area = UIElement_Rect::WEB_BUTTON_HOVER_;
	button.pressed_tex_area = UIElement_Rect::WEB_BUTTON_PRESSED_;
	website_button = App->gui->CreateUIButton({ 220, (int)height + 500 }, button, this);

	// Labels
	UILabel_Info label;
	label.interactive = false;
	label.font_name = Font_Names::MSMINCHO_; // big titles
	label.text = "AUTHORS";
	authors_title = App->gui->CreateUILabel({ 120, (int)height / 2 + (int)height / 8 - App->gui->GetRectFromAtlas(UIElement_Rect::SMALL_WINDOW_).h * scale / 2 }, label);
	authors_title->SetColor({ authors_title->GetColor().r,authors_title->GetColor().g,authors_title->GetColor().b,0 });
	label.text = "LICENSE";
	license_title = App->gui->CreateUILabel({ (int)width - 100 - (App->gui->GetRectFromAtlas(UIElement_Rect::SMALL_WINDOW_).w * scale), 200 }, label);
	license_title->SetColor({ license_title->GetColor().r,license_title->GetColor().g,license_title->GetColor().b,0 });

	label.interaction_from_father = true;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.vertical_orientation = UIElement_VERTICAL_POS::MIDDLE_;
	label.normal_color = Purple_;
	label.hover_color = Pink_;
	label.pressed_color = LightPink_;
	label.text = "WEBSITE";
	website_title = App->gui->CreateUILabel({ (website_button->GetLocalRect().w * scale) / 2, (website_button->GetLocalRect().h * scale) / 2 }, label, this, website_button);

	label.interaction_from_father = false;
	label.font_name = Font_Names::MSMINCHO_; // small descriptions
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	label.vertical_orientation = UIElement_VERTICAL_POS::TOP_;
	label.normal_color = White_;
	label.text = "Sandra & Guillem";
	authors_description = App->gui->CreateUILabel({ 120, (int)height / 2 + (int)height / 8 - App->gui->GetRectFromAtlas(UIElement_Rect::SMALL_WINDOW_).h * scale / 2 + 50 }, label, this);
	authors_description->SetColor({ authors_title->GetColor().r,authors_title->GetColor().g,authors_title->GetColor().b,0 });
	label.text = "MIT License Copyright(c) 2017 Sandra Alvarez & Guillem Costa\n\n Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\n The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\n THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";
	label.text_wrap_length = 110;
	license_description = App->gui->CreateUILabel({ 10,30 }, label, this, credits_window);

	// Slider to scroll the previous text
	UISlider_Info slider;
	slider.interactive = false;
	slider.draggable = false;
	slider.tex_area = UIElement_Rect::SLIDER_BAR_;
	slider.button_slider_area = UIElement_Rect::SLIDER_BUTTON_;
	slider.slider_button_pos.x = 0;
	slider.offset = 3;
	slider.buggy_offset = -1;
	license_slider = App->gui->CreateUISlider({ credits_window->GetLocalPos().x + (credits_window->GetLocalRect().w * scale) / 2,(int)height + 500 }, slider, this);
}