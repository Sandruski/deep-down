#ifndef __j1MENU_PAUSE_PAUSE_H__
#define __j1MENU_PAUSE_PAUSE_H__

#include "j1Module.h"

#include "Animation.h"

struct SDL_Texture;
struct UILabel;
struct UIImage;
struct Cat;
struct UIWindow;

enum MenuState {
	NO_MENU_PAUSE_PAUSE_,
	TITLE_TO_START_,
	PRESS_START_,
	TITLE_TO_MENU_PAUSE_PAUSE_,
	APPEAR_MENU_PAUSE_OPTIONS_,
	IN_MAIN_MENU_,
	IN_SETTINGS_,
	IN_CREDITS_
};

enum MenuOptions {
	START_,
	CONTINUE_,
	SETTINGS_,
	CREDITS_,
	EXIT_,
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

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnUIEvent(UIElement* UIelem, UIEvents UIevent);

private:
	MenuState menuState = MenuState::NO_MENU_PAUSE_PAUSE_;
	uint catPosition = 0;

	iPoint camera_start_position = { 0,0 };

	// Game title
	UILabel* letters[8];
	UILabel* press_any_button = nullptr;
	UIButton* MENU_PAUSE_buttons[5];
	UILabel* MENU_PAUSE_options[5];

	bool is_invisible = true;

	UIImage* black_screen_image;
	uint i = 0;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	float timer = 0;

	bool print_title = false;

	bool next_phase = false;
	bool visible_again = false;
	//_game_title

	// Cat
	Cat* cat = nullptr;


	uint width = 0, height = 0;
	int scale = 0;

	UIWindow* window = nullptr;

	bool camera_blit = false;

public:
	bool left_transition = false;
	bool right_transition = false;

	bool clouds_transition = false;

};

#endif // __j1MENU_PAUSE_PAUSE_H__