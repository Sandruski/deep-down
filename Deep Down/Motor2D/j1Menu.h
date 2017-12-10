#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"

#include "Animation.h"

struct SDL_Texture;
struct UILabel;
struct UIImage;
struct Cat;

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

private:
	iPoint camera_start_position = { 0,0 };

	// Game title
	UILabel* letters[8];
	UIImage* black_screen_image;
	uint i = 0;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	float timer = 0;
	bool print_game_title = false;
	bool next_phase = false;
	bool visible_again = false;
	bool all_visible = false;
	//_game_title

	// Cat
	Cat* cat = nullptr;

public:
	bool left_transition = false;
	bool right_transition = false;

};

#endif // __j1MENU_H__