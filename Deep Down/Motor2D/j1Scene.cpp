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
#include "UIWindow.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UISlider.h"

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

	catsUI_anim.PushBack({ 0,0,26,26 });
	catsUI_anim.PushBack({ 26,0,26,26 });
	catsUI_anim.PushBack({ 26,0,26,26 });
	catsUI_anim.PushBack({ 0,26,26,26 });
	catsUI_anim.PushBack({ 26,26,26,26 });
	catsUI_anim.PushBack({ 0,26,26,26 });
	catsUI_anim.PushBack({ 26,0,26,26 });
	catsUI_anim.speed = 2.5f;
	//TODO ADD TO XML

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	// Ingame UI
	UILifeBar_Info girl_life_bar;
	girl_life_bar.bar = { 86,532,222,4 };
	girl_life_bar.life = 222;
	girl_life_bar.life_bar_position = { 5,1 };
	girl_life_bar.tex_name = DSUI_;
	girl_life_bar.tex_area = { 80,524,230,8 };
	girl_life_bar.backgorund_life_bar = { 85,538,223,4 };
	girl_life_bar.horizontal_orientation = CENTER_;
	progress_bar = App->gui->CreateUILifeBar({ 512,668 }, girl_life_bar, this);

	UIImage_Info cats_obtained;
	cats_obtained.tex_name = CAT_SCORE_;
	cats_obtained.tex_area = { 0,0,26,26 };
	cat_UI = App->gui->CreateUIImage({ 800,40 }, cats_obtained, this);

	UILabel_Info numbers;
	numbers.font_name = BLACK_BLOC_;
	numbers.interactive = false;
	numbers.text = "00";
	numbers.draggable = false;

	cats_score = App->gui->CreateUILabel({ 885,45 }, numbers);

	numbers.text = "00";
	numbers.font_name = DIGITAL7_;
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
				App->fade->FadeToBlack(this, this, 1.0f, fades::slider_fade);
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

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && pause == false)
	{
		pause = true;
		OpeningPauseMenu();
	}
	else if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && pause == true)
	{
		pause = false;
		App->gui->DestroyElement(pause_menu); // Closing pause menu
	}
}

void j1Scene::OpeningPauseMenu()
{
	UIWindow_Info menu;
	menu.tex_name = MENU_PAUSE_;
	menu.draggable = true;

	pause_menu = App->gui->CreateUIWindow({ 256,171 }, menu, this);

	UILabel_Info label;
	label.font_name = SOBAD_;
	label.normal_color = { 0,0,0,255 };
	label.draggable = false;

	label.text = "Resume";
	menu_pause_labels[RESUME_] = App->gui->CreateUILabel({ 40,40 }, label, this, pause_menu); // Resume label

	label.text = "Save";
	menu_pause_labels[SAVE_] = App->gui->CreateUILabel({ 40,140 }, label, this, pause_menu); // Save label

	label.text = "Options";
	menu_pause_labels[OPTIONS_] = App->gui->CreateUILabel({ 40,240 }, label, this, pause_menu); // Options label

	label.text = "Quit";
	menu_pause_labels[QUIT_] = App->gui->CreateUILabel({ 40,340 }, label, this, pause_menu); // Quit label
}

void j1Scene::OnUIEvent(UIElement* UIelem, UIEvents UIevent)
{
	switch (UIevent)

	{
	case UIEvents::MOUSE_ENTER_:

		for (uint i = 0; i < 4; ++i) { // Labels: RESUME_, SAVE_, OPTIONS_, QUIT_
			if (UIelem == (UIElement*)menu_pause_labels[i]) {
				menu_pause_labels[i]->IncreasePos(LABELS_POS_MOUSE_ENTER);
				continue;
			}
		}

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
		}

		else if (UIelem == (UIElement*)menu_pause_labels[SAVE_])
		{
			App->SaveGame();
			last_index = index;
		}

		else if (UIelem == (UIElement*)menu_pause_labels[QUIT_])
		{
			App->quit_game = true;
		}

		else if (UIelem == (UIElement*)menu_pause_labels[OPTIONS_])
		{
			for (uint i = 0; i < 5; ++i) {
				App->gui->DestroyElement(menu_pause_labels[i]);
			}

			UILabel_Info label;
			label.font_name = SOBAD_;
			label.normal_color = { 0,0,0,255 };
			label.draggable = false;
			label.interactive = false;

			label.text = "volume";
			App->gui->CreateUILabel({ 80,60 }, label, this, pause_menu); // Volume label

			label.text = "cap_frames";
			App->gui->CreateUILabel({ 80,160 }, label, this, pause_menu); // Cap_frames label

			label.text = "camera blit";
			App->gui->CreateUILabel({ 80,260 }, label, this, pause_menu); // Camera_blit label

			label.interactive = true;

			label.text = "back";
			menu_pause_labels[BACK_] = App->gui->CreateUILabel({ 40,360 }, label, this, pause_menu); // Back label

			UISlider_Info slider;

			slider.tex_name = SLIDER_;
			slider.draggable = false;
			slider.tex_area = { 9,0,54,9 };
			slider.button_slider_area = { 0,1,9,10 };
			slider.offset = 3;
			slider.slider_button_pos.x = App->audio->music_volume * (slider.tex_area.w) / 128;
			slider.buggy_offset = -1;

			volume_slider = App->gui->CreateUISlider({ 240,70 }, slider, this, pause_menu);

			UIButton_Info checkbox;
			checkbox.checkbox = true;
			checkbox.tex_name = CHECKBOX_;
			checkbox.normal_tex_area = { 0,0,11,7 };
			checkbox.hover_tex_area = { 0,0,11,7 };
			checkbox.pressed_tex_area = { 12,0,11,7 };
			checkbox.draggable = true;

			if (App->toCap)
				checkbox.checkbox_checked = true;

			menu_pause_checkbox[0] = App->gui->CreateUIButton({ 300,180 }, checkbox, this, pause_menu); // Cap_frames checkbox

			if (App->map->camera_blit)
				checkbox.checkbox_checked = true;
			else
				checkbox.checkbox_checked = false;

			menu_pause_checkbox[1] = App->gui->CreateUIButton({ 300,280 }, checkbox, this, pause_menu); // Camera_blit checkbox

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
		}

		else if (UIelem == (UIElement*)volume_slider)
		{
			swap_music = true;
		}

		break;

	case UIEvents::MOUSE_LEFT_UP_:

		if (UIelem == (UIElement*)volume_slider)
		{
			swap_music = false;
		}

		break;
	}
}

UILabel* j1Scene::CreateLevelNameText(uint level)
{
	uint width, height;
	App->win->GetWindowSize(width, height);

	UILabel_Info label;
	label.normal_color = WarmYellow_;
	if (level == 0)
		label.text = "CONSUMED KING'S GARDEEP";
	else if (level == 1)
		label.text = "ANOR LONDEEP";
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.vertical_orientation = UIElement_VERTICAL_POS::MIDDLE_;
	label.font_name = Font_Names::MSMINCHO_;
	label.draggable = false;
	label.interactive = false;

	return App->gui->CreateUILabel({ (int)width / 2,(int)height / 2 }, label);
}

UILabel* j1Scene::CreateCatsPickedText(uint cats_picked)
{
	uint width, height;
	App->win->GetWindowSize(width, height);

	UILabel_Info label;
	p2SString tmp("%s%d%s", "Cats picked ", cats_picked, "/5");
	label.text = tmp.GetString();
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.font_name = Font_Names::MSMINCHO_;
	label.draggable = false;
	label.interactive = false;

	return App->gui->CreateUILabel({ (int)width / 2,(int)height / 2 + 30 }, label);
}