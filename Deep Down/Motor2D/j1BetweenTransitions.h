#ifndef __j1BETWEEN_TRANSITIONS_H__
#define __j1BETWEEN_TRANSITIONS_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1FadeToBlack.h"

struct UILabel;
struct UICursor;

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

public:
	// Camera
	uint width = 0;
	uint height = 0;
	uint scale = 0;

private:
	UICursor* game_cursor;

	// Fade
	fade_step current_step = fade_step::none;

	// Create labels
	bool level_name = true;
	bool cats_picked = true;
	bool level_cats_picked = true;
	bool level_cats_picked2 = true;
	bool total_cats_picked = true;
	bool score_text = true;
	bool you = true;
	bool died = true;

	// Store labels
	UILabel* l_level_name = nullptr;
	UILabel* l_cats_picked = nullptr;
	UILabel* l_level_cats_picked = nullptr;
	UILabel* l_level_cats_picked2 = nullptr;
	UILabel* l_total_cats_picked = nullptr;
	UILabel* l_score_text = nullptr;
	UILabel* l_you = nullptr;
	UILabel* l_died = nullptr;

	// Game info
	uint scene_index = 0;
	bool start = true;

	bool bloody = false;

public:
	bool back_to_main_menu = false;
	int highscore = 0;
};

#endif // ___j1BETWEEN_TRANSITIONS_H___