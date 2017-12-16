#ifndef __j1MENU_PAUSE_PAUSE_H__
#define __j1MENU_PAUSE_PAUSE_H__

#include "j1Module.h"

#include "Animation.h"

struct SDL_Texture;
struct Cat;
struct UILabel;
struct UIImage;
struct UIWindow;

enum MenuState {
	NO_MENU_,
	TITLE_ANIMATION_,
	PRESS_ANY_BUTTON_,
	TRANSITION_TO_MAIN_MENU_,
	MAIN_MENU_OPTIONS_ANIMATION_,
	AT_MAIN_MENU_,
	AT_SETTINGS_,
	AT_CREDITS_
};

enum MenuCatState {
	NO_CAT_,
	MC_APPEAR_RUNNING_,
	MC_JUMP_TITLE_,
	MC_RUN_FIRST_LETTER_,
	MC_ROLL_SECOND_LETTER_,
	MC_RUN_WORD_,
	MC_ROLL_END_WORD_,
	MC_STOP_JUMPING_,
	MC_AT_GROUND_,
	MC_START_,
	MC_CONTINUE_
};

enum MenuOptions {
	MM_START_,
	MM_CONTINUE_,
	MM_SETTINGS_,
	MM_CREDITS_,
	MM_EXIT_,
};

class j1Menu : public j1Module
{
public:
	j1Menu();

	// Destructor
	virtual ~j1Menu();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void OnUIEvent(UIElement* UIelem, UIEvents UIevent);

	void CreateMainMenuUIElements();
	void CreateSettingsUIElements();
	void CreateCreditsUIElements();

private:
	MenuState menuState = MenuState::NO_MENU_;
	MenuCatState menuCatState = MenuCatState::NO_CAT_;

	Cat* cat = nullptr;
	float cat_position_increment[8];

	// UI Elements
	// Main menu
	UILabel* title_letters[8];
	UIButton* main_menu_buttons[5];
	UILabel* main_menu_options[5];

	UILabel* highscore_text = nullptr;
	UILabel* highscore_value = nullptr;
	UILabel* press_any_button = nullptr;
	UILabel* skip = nullptr;
	UIImage* black_screen_image = nullptr;

	// Settings
	UILabel* music_volume_text = nullptr;
	UILabel* FX_volume_text = nullptr;
	UILabel* fullscreen_text = nullptr;
	UILabel* camera_blit_text = nullptr;
	UILabel* cap_frames_text = nullptr;

	UIButton* fullscreen_checkbox = nullptr;
	UIButton* camera_blit_checkbox = nullptr;
	UIButton* cap_frames_checkbox = nullptr;
	UIButton* back_to_main_menu_from_settings = nullptr;

	UISlider* music_slider = nullptr;
	UISlider* FX_slider = nullptr;

	// Credits
	UIWindow* credits_window = nullptr;
	UIButton* back_to_main_menu_from_credits = nullptr;
	UIButton* website_button = nullptr;
	UILabel* authors_title = nullptr;
	UILabel* authors_description = nullptr;
	UILabel* website_title = nullptr;
	UILabel* website_description = nullptr;
	UILabel* license_title = nullptr;
	UILabel* license_description = nullptr;
	UISlider* license_slider = nullptr;

	// Credits slider
	bool sliding = false;
	int last_slide = 0;

	float timer = 0;

	bool print_title = false;
	bool blit_cat = true;
	bool is_invisible = true;

	bool from_settings = false;
	bool from_credits = false;
	bool camera_moved = false;

	// Screen parameters
	uint width = 0, height = 0;
	int scale = 0;

	iPoint camera_start_position = { 0,0 };
	bool camera_blit = false;

public:
	UIWindow* settings_window = nullptr;
};

#endif // __j1MENU_PAUSE_PAUSE_H__