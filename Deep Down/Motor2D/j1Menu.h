#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"

#include "Animation.h"

#define MENU_THEME_VOLUME 100
#define MAX_VOLUME 128
#define CAMERA_OFFSET 10
#define MAX_ALPHA 255
#define SLIDE_SLOW_SPEED 500
#define SLIDE_FAST_SPEED 800
#define INTERMITENT_FADE 1.2
#define CS_INTERMITENT_FADE 1

struct SDL_Texture;
struct Cat;
struct UILabel;
struct UIImage;
struct UIWindow;
struct UIButton;
struct UISlider;
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

	// UI elements
	// Main menu
	UILabel* title_letters[8] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	UILabel* main_menu_options[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };
	UILabel* highscore_text = nullptr;
	UILabel* highscore_value = nullptr;
	UILabel* press_any_button = nullptr;
	UILabel* skip = nullptr;

	UIButton* main_menu_buttons[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };

	UIImage* black_screen_image = nullptr;

	// Settings
	UIWindow* settings_window = nullptr;

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

	UILabel* authors_title = nullptr;
	UILabel* authors_description = nullptr;
	UILabel* website_title = nullptr;
	UILabel* website_description = nullptr;
	UILabel* license_title = nullptr;
	UILabel* license_description = nullptr;

	UIButton* back_to_main_menu_from_credits = nullptr;
	UIButton* website_button = nullptr;

	UISlider* license_slider = nullptr;
	//_UI_elements

	// Screen parameters
	uint width = 0, height = 0;
	int scale = 0;

	// Camera
	iPoint cam_start_pos = { 0,0 };
	iPoint camera_start_position = { 0,0 };
	float camera_speed = 0.0f;

	// Time variables
	float skip_button_speed = 0.0f;
	float press_any_button_speed = 0.0f;
	float press_any_button_seconds = 0.0f;
	float scene_fade_seconds = 0.0f;
	float start_fade_seconds = 0.0f;
	float continue_fade_seconds = 0.0f;
	float options_seconds = 0.0f;
	float title_opaque_seconds = 0.0f;
	float timer = 0.0f;

	// Cat variables
	iPoint cat_final_position = { 627,190 };
	float cat_run = 0.0f;
	fPoint cat_jump = { 0.0f, 0.0f };
	fPoint cat_width_roll = { 0.0f, 0.0f };
	fPoint cat_height_roll = { 0.0f, 0.0f };
	fPoint cat_roll = { 0.0f, 0.0f };
	float seconds_stop_jumping = 0.0f;
	float cat_seconds = 0.0f;

	// Other
	iPoint title_pos = { 0,0 };
	iPoint title_track = { 0,0 };

	bool swap_music = false;
	bool swap_fx = false;

	bool sliding = false;
	int last_slide = 0;

	bool print_title = false;
	bool blit_cat = true;

	bool from_settings = false;
	bool from_credits = false;
	bool settings_done = false;
	bool credits_done = false;
	bool camera_moved = false;

	bool continue_button = false;
};

#endif // __j1MENU_H__