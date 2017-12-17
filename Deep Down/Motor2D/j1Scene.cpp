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
#include "j1Menu.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "UILifeBar.h"
#include "UIWindow.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UISlider.h"
#include "UICursor.h"
#include "j1BetweenTransitions.h"

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

	// Load cat animation
	for (pugi::xml_node node = config.child("UI_cat").child("animation").child("frame"); node; node = node.next_sibling("frame"))
		catsUI_anim.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	catsUI_anim.speed = config.child("UI_cat").child("animation").attribute("speed").as_float();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	god = false;

	// In-game UI
	UILifeBar_Info girl_life_bar;
	girl_life_bar.tex_area = UIElement_Rect::LIFEBAR_FRAME_;
	girl_life_bar.background = UIElement_Rect::LIFEBAR_BROWN_;
	girl_life_bar.bar = UIElement_Rect::LIFEBAR_RED_;
	girl_life_bar.life = 222;
	girl_life_bar.life_bar_position = { 5,1 };
	girl_life_bar.horizontal_orientation = CENTER_;
	progress_bar = App->gui->CreateUILifeBar({ 512,668 }, girl_life_bar, this);

	UIImage_Info cats_obtained;
	cats_obtained.tex_area = UIElement_Rect::CAT_CIRCLE_IDLE_;
	cat_UI = App->gui->CreateUIImage({ 800,40 }, cats_obtained, this);

	UILabel_Info numbers;
	numbers.font_name = Font_Names::MSMINCHO_;
	numbers.interactive = false;
	numbers.draggable = false;

	if (index == 0)
		numbers.text = p2SString("%i", cats_first_map);
	else
		numbers.text = p2SString("%i", cats_second_map);

	cats_score = App->gui->CreateUILabel({ 885,45 }, numbers);
	numbers.text = "00";
	countdown_time = App->gui->CreateUILabel({ 50,50 }, numbers);

	// Start entities
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
		if (!loading_state && !check_continue) {
			gate = false;
			fx = false;

			// Player start position
			App->entities->playerData->start_pos = App->map->data.GetObjectPosition("Player", "StartPos");
			App->entities->playerData->position = App->entities->playerData->start_pos;
		}
		check_continue = false;
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

	if (countdown_to_die >= 0.5f)
	{
		progress_bar->DecreaseLifeProgress(1);
		countdown_to_die = 0.0f;
	}
	countdown_to_die += dt;

	count_time += dt;
	if (count_time < 10)
		countdown_time->SetText(p2SString("0%i", (int)count_time));
	else
		countdown_time->SetText(p2SString("%i", (int)count_time));

	if (activate_UI_anim) {
		cat_UI->StartAnimation(catsUI_anim);
		activate_UI_anim = false;
	}

	if (swap_music) {
		int volume = volume_slider->GetPercent();
		volume = 128 * volume / 100;
		App->audio->SetMusicVolume(volume);
	}
	else if (swap_fx && App->audio->active) {
		int volume = fx_slider->GetPercent();
		volume = 128 * volume / 100;
		App->audio->SetFxVolume(volume);
	}

	if (menu_bouncing) {
		if (pause_menu->SlideTransition(App->auxiliar_dt, height / 2, 500.0f, true, 20.0f, 2.0f, false)) {
			menu_position = pause_menu->GetLocalPos();
			menu_pause_labels[RESUME_]->SetInteraction(true);
			menu_pause_labels[SAVE_]->SetInteraction(true);
			menu_pause_labels[OPTIONS_]->SetInteraction(true);
			menu_pause_labels[QUIT_]->SetInteraction(true);
			closeWindow->SetInteraction(true);
			menu_bouncing = false;
		}
	}

	if (App->entities->playerData != nullptr) {

		// If player dies, go back to main menu
		if (App->entities->playerData->player_is_dead) {
			loading_state = false;

			App->trans->do_transition = true;
			App->entities->KillAllEntities();
			App->trans->back_to_main_menu = true;
			App->fade->FadeToBlack(this, App->menu, 8.0f, fades::slider_fade);
			return true;
		}

		if (App->map->data.CheckIfEnter("Player", "EndPos", App->entities->playerData->position) && App->fade->GetStep() == fade_step::none) {
			if (App->entities->playerData->player.GetState() == forward_ || App->entities->playerData->player.GetState() == backward_
				|| App->entities->playerData->player.GetState() == idle_ || App->entities->playerData->player.GetState() == idle2_) {
				loading_state = false;
				god = true;

				if (index == 0)
					index = 1;
				else
					index = 0;

				App->entities->playerData->player.SetState(stop_);

				if (index == 1) {
					App->trans->SetNextTransitionInfo(1, true);
					App->fade->FadeToBlack(this, this, 6.0f, fades::slider_fade);
					return true;
				}
				else {
					App->entities->KillAllEntities();
					App->trans->back_to_main_menu = true;
					App->trans->SetNextTransitionInfo(1, false);
					App->trans->highscore = cats_first_map * 100 + cats_second_map * 100 + progress_bar->GetProgress() - count_time;
					App->fade->FadeToBlack(this, App->menu, 8.0f, fades::slider_fade);
					return true;
				}
			}
		}
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

	if (forced_cleanup) {
		App->entities->KillAllEntitiesExceptPlayer();
		forced_cleanup = false;
	}

	if (!loading_state)
		App->entities->CleanUp();

	App->gui->ClearAllUI();

	return true;
}

void j1Scene::MoveCamera() {

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) //move camera left (debug functionality)
		App->render->camera.x += 5;
	else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) //move camera right (debug functionality)
		App->render->camera.x -= 5;
	else {
		if (App->entities->playerData != nullptr)
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

	if (save.child("cats_obtained") == NULL) {
		save.append_child("cats_obtained1").append_attribute("cats1") = cats_first_map;
		save.append_child("cats_obtained2").append_attribute("cats2") = cats_second_map;
	}
	else {
		save.append_child("cats_obtained1").attribute("cats1") = cats_first_map;
		save.append_child("cats_obtained2").attribute("cats2") = cats_second_map;
	}

	if (save.child("lifebar") == NULL) {
		save.append_child("lifebar").append_attribute("lifebar") = progress_bar->GetProgress();
	}
	else {
		save.append_child("lifebar").attribute("lifebar") = progress_bar->GetProgress();
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
		if (save.child("index").attribute("index").as_uint() != index && App->fade->GetStep() != fade_to_black) {

			if (App->trans->continue_game) {
				forced_cleanup = true;
				App->trans->continue_game = false;
			}

			index = save.child("index").attribute("index").as_uint();
			App->trans->SetNextTransitionInfo(index, true);
			App->fade->FadeToBlack(this, this, 6.0f, fades::slider_fade);
		}
		else {
			forced_cleanup = true;

			App->trans->continue_game = false;
			App->trans->SetNextTransitionInfo(index, true);
			App->fade->FadeToBlack(this, this, 6.0f, fades::slider_fade); // false false
		}
	}

	if (save.child("lifebar") != NULL) {
		progress_bar->SetLifeProgress(save.child("lifebar").attribute("lifebar").as_int());
	}

	if (save.child("cats_obtained") != NULL) {
		cats_first_map = save.child("cats_obtained1").attribute("cats1").as_uint();
		cats_second_map = save.child("cats_obtained2").attribute("cats2").as_uint();
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
			god = true;

			App->entities->playerData->player.SetState(stop_);

			if (index == 0) {
				App->entities->playerData->position = App->entities->playerData->start_pos;
				gate = false;
				fx = false;
			}
			else
				index = 0;

			App->trans->SetNextTransitionInfo(0, true);
			App->fade->FadeToBlack(this, this, 6.0f, fades::slider_fade);
		}
	}

	// F2: start from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		if (App->entities->playerData->player.GetState() == forward_ || App->entities->playerData->player.GetState() == backward_
			|| App->entities->playerData->player.GetState() == idle_ || App->entities->playerData->player.GetState() == idle2_) {
			loading_state = false;
			god = true;

			App->entities->playerData->player.SetState(stop_);
			App->entities->playerData->position = App->entities->playerData->start_pos;
			gate = false;
			fx = false;

			App->trans->SetNextTransitionInfo(index, true);
			App->fade->FadeToBlack(this, this, 6.0f, fades::slider_fade);
		}
	}

	// F3: show colliders

	// F4: change between maps
	if (App->entities->playerData != nullptr) {
		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
			if (App->entities->playerData->player.GetState() == forward_ || App->entities->playerData->player.GetState() == backward_
				|| App->entities->playerData->player.GetState() == idle_ || App->entities->playerData->player.GetState() == idle2_) {
				loading_state = false;
				god = true;

				if (index == 0)
					index = 1;
				else
					index = 0;

				App->entities->playerData->player.SetState(stop_);

				App->trans->SetNextTransitionInfo(index, true);
				App->fade->FadeToBlack(this, this, 6.0f, fades::slider_fade);
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
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN || App->trans->continue_game) {
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

	// 1, 2, 3: camera blit
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->map->blit_offset < 15 && App->map->camera_blit)
		App->map->blit_offset += 7;

	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->map->blit_offset > -135 && App->map->camera_blit)
		App->map->blit_offset -= 7;

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->map->camera_blit = !App->map->camera_blit;

	// Tab: open in-game pause menu
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && pause == false)
	{
		pause = true;
		menu_bouncing = true;
		OpeningPauseMenu(true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && pause == true)
	{
		pause = false;
		App->gui->DestroyElement(pause_menu); // close pause menu
	}
}

void j1Scene::OpeningPauseMenu(bool from_scratch)
{
	UIWindow_Info menu;
	menu.tex_area = UIElement_Rect::BIG_WINDOW_;

	if (from_scratch) {
		pause_menu = App->gui->CreateUIWindow({ 256, (int)height + 500 }, menu, this);
		menu.interactive = false;
	}
	else {
		pause_menu = App->gui->CreateUIWindow({ 256, menu_position.y }, menu, this);
	}

	UILabel_Info label;
	if (from_scratch)
		label.interactive = false;
	label.draggable = false;
	label.font_name = Font_Names::MSMINCHO_;
	label.normal_color = { 0,0,0,255 };

	label.text = "Resume";
	menu_pause_labels[RESUME_] = App->gui->CreateUILabel({ 40,40 }, label, this, pause_menu); // Resume label

	label.text = "Save";
	menu_pause_labels[SAVE_] = App->gui->CreateUILabel({ 40,140 }, label, this, pause_menu); // Save label

	label.text = "Options";
	menu_pause_labels[OPTIONS_] = App->gui->CreateUILabel({ 40,240 }, label, this, pause_menu); // Options label

	label.text = "Quit";
	menu_pause_labels[QUIT_] = App->gui->CreateUILabel({ 40,340 }, label, this, pause_menu); // Quit label

	UIButton_Info close_window;
	if (from_scratch)
		close_window.interactive = false;
	close_window.normal_tex_area = UIElement_Rect::CLOSE_BUTTON_NORMAL_;
	close_window.hover_tex_area = UIElement_Rect::CLOSE_BUTTON_NORMAL_;
	close_window.pressed_tex_area = UIElement_Rect::CLOSE_BUTTON_PRESSED_;
	closeWindow = App->gui->CreateUIButton({ 490,3 }, close_window, this, pause_menu);
}

void j1Scene::OnUIEvent(UIElement* UIelem, UIEvents UIevent)
{
	switch (UIevent)

	{
	case UIEvents::MOUSE_ENTER_:

		for (uint i = 0; i < 4; ++i) { // Labels: RESUME_, SAVE_, OPTIONS_, QUIT_
			if (UIelem == (UIElement*)menu_pause_labels[i]) {
				menu_pause_labels[i]->IncreasePos(LABELS_POS_MOUSE_ENTER);
				App->audio->PlayFx(9);
				continue;
			}
		}

		if (UIelem == (UIElement*)menu_pause_labels[BACK_])
			App->audio->PlayFx(9);

		break;

	case UIEvents::MOUSE_LEAVE_:

		for (uint i = 0; i < 4; ++i) { // Labels: RESUME_, SAVE_, OPTIONS_, QUIT_
			if (UIelem == (UIElement*)menu_pause_labels[i]) {
				menu_pause_labels[i]->DecreasePos(LABELS_POS_MOUSE_ENTER);
				continue;
			}
		}

		break;

	case UIEvents::MOUSE_LEFT_CLICK_:

		if (UIelem == (UIElement*)menu_pause_labels[RESUME_])
		{
			pause = false;
			App->gui->DestroyElement(pause_menu); // Closing pause menu
			App->audio->PlayFx(10);
		}

		else if (UIelem == (UIElement*)menu_pause_labels[SAVE_])
		{
			App->SaveGame();
			last_index = index;
			App->audio->PlayFx(10);
		}

		else if (UIelem == (UIElement*)menu_pause_labels[QUIT_])
		{
			App->quit_game = true;
			App->audio->PlayFx(10);
		}

		else if (UIelem == (UIElement*)menu_pause_labels[OPTIONS_])
		{
			App->audio->PlayFx(10);
			for (uint i = 0; i < 4; ++i) {
				App->gui->DestroyElement(menu_pause_labels[i]);
			}
			OpeningSubMenuOptions();		
		}

		else if (UIelem == (UIElement*)menu_pause_checkbox[0]) // Cap_frames checkbox
		{
			App->toCap = !App->toCap;
		}

		else if (UIelem == (UIElement*)menu_pause_checkbox[1]) // Camera_blit checkbox
		{
			App->map->camera_blit = !App->map->camera_blit;
		}

		else if (UIelem == (UIElement*)menu_pause_labels[BACK_])
		{
			App->gui->DestroyElement(pause_menu); // Closing pause menu
			OpeningPauseMenu();
			App->audio->PlayFx(10);
		}

		else if (UIelem == (UIElement*)volume_slider)
		{
			swap_music = true;
		}

		else if (UIelem == (UIElement*)fx_slider)
		{
			swap_fx = true;
		}
		break;

	case UIEvents::MOUSE_LEFT_UP_:

		if (UIelem == (UIElement*)volume_slider)
		{
			swap_music = false;
		}

		if (UIelem == (UIElement*)volume_slider)
		{
			swap_fx = false;
		}

		else if (UIelem == (UIElement*)closeWindow)
		{
			pause = false;
			App->gui->DestroyElement(pause_menu);
			App->audio->PlayFx(10);
		}
		break;
	}
}

void j1Scene::OpeningSubMenuOptions()
{
	UILabel_Info label;
	label.font_name = Font_Names::MSMINCHO_;
	label.normal_color = { 0,0,0,255 };
	label.draggable = false;

	label.text = "volume";
	App->gui->CreateUILabel({ 80,60 }, label, this, pause_menu); // Volume label

	label.text = "FX volume";
	App->gui->CreateUILabel({ 80,150 }, label, this, pause_menu); // FX Volume label

	label.text = "cap_frames";
	App->gui->CreateUILabel({ 80,240 }, label, this, pause_menu); // Cap_frames label

	label.text = "camera blit";
	App->gui->CreateUILabel({ 80,330 }, label, this, pause_menu); // Camera_blit label

	label.interactive = true;

	label.text = "back";
	menu_pause_labels[BACK_] = App->gui->CreateUILabel({ 40,400 }, label, this, pause_menu); // Back label

	UISlider_Info slider;
	slider.draggable = false;
	slider.tex_area = UIElement_Rect::SLIDER_BAR_;
	slider.button_slider_area = UIElement_Rect::SLIDER_BUTTON_;
	slider.offset = 3;
	slider.slider_button_pos.x = App->audio->music_volume * 54 / 128;
	slider.buggy_offset = -1;
	volume_slider = App->gui->CreateUISlider({ 300,75 }, slider, this, pause_menu);
	slider.slider_button_pos.x = App->audio->fx_volume * 54 / 128;
	fx_slider = App->gui->CreateUISlider({ 300,165 }, slider, this, pause_menu);

	UIButton_Info checkbox;
	checkbox.checkbox = true;
	checkbox.normal_tex_area = UIElement_Rect::CHECKBOX_NORMAL_;
	checkbox.hover_tex_area = UIElement_Rect::CHECKBOX_NORMAL_;
	checkbox.pressed_tex_area = UIElement_Rect::CHECKBOX_PRESSED_;
	checkbox.draggable = true;

	if (App->toCap)
		checkbox.checkbox_checked = true;

	menu_pause_checkbox[0] = App->gui->CreateUIButton({ 300,260 }, checkbox, this, pause_menu); // Cap_frames checkbox

	if (App->map->camera_blit)
		checkbox.checkbox_checked = true;
	else
		checkbox.checkbox_checked = false;

	menu_pause_checkbox[1] = App->gui->CreateUIButton({ 300,350 }, checkbox, this, pause_menu); // Camera_blit checkbox
}