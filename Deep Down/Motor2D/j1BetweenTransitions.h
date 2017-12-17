#ifndef __j1BETWEEN_TRANSITIONS_H__
#define __j1BETWEEN_TRANSITIONS_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1FadeToBlack.h"

#define QUAD_SECONDS 7

struct UILabel;
struct UICursor;
struct UIImage;

class j1BetweenTransitions : public j1Module
{
public:
	j1BetweenTransitions();

	// Destructor
	virtual ~j1BetweenTransitions();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void SetNextTransitionInfo(uint scene_index = 0, bool start = true);

	UICursor* CreateCursor();

private:
	UILabel* CreateLevelNameText(uint level = 0);
	UILabel* CreateCatsPickedText(uint cats_picked = 0);
	UILabel* CreateLevelCatsPickedText(iPoint position, uint level = 0, uint cats_picked = 0);
	UILabel* CreateTotalCatsPickedText(uint first_level_cats_picked = 0, uint second_level_cats_picked = 0);
	UILabel* CreateScoreText(int score = 0);
	UILabel* CreateYouDiedText(uint word = 0);

	UIImage* CreateBlackQuad();

public:
	// Camera
	uint width = 0;
	uint height = 0;
	uint scale = 0;

private:
	UICursor* game_cursor = nullptr;

	// Create labels
	bool level_name = true;
	bool cats_picked = true;
	bool level_cats_picked = true;
	bool level_cats_picked2 = true;
	bool total_cats_picked = true;
	bool score_text = true;
	bool you = true;
	bool died = true;

	// Game info
	uint scene_index = 0;
	bool start = true;

	bool player_death = false;
	bool bloody = false;
	float timer = 0.0f;

public:
	// Store labels
	UILabel* l_level_name = nullptr;
	UILabel* l_cats_picked = nullptr;
	UILabel* l_level_cats_picked = nullptr;
	UILabel* l_level_cats_picked2 = nullptr;
	UILabel* l_total_cats_picked = nullptr;
	UILabel* l_score_text = nullptr;
	UILabel* l_you = nullptr;
	UILabel* l_died = nullptr;
	UIImage* black_screen_image = nullptr;

	// General game info
	bool back_to_main_menu = false;
	int highscore = 0;
	bool continue_game = false;
	bool do_transition = false;
	bool create_black_quad = true;
};

#endif // ___j1BETWEEN_TRANSITIONS_H___