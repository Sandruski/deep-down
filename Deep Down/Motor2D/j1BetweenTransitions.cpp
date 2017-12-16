#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"
#include "j1BetweenTransitions.h"

#include "j1Scene.h"
#include "j1Window.h"
#include "j1EntityFactory.h"
#include "Player.h"

#include "j1Gui.h"
#include "UILabel.h"
#include "UICursor.h"

#include"Brofiler\Brofiler.h"

j1BetweenTransitions::j1BetweenTransitions() : j1Module()
{
	name.create("trans");
}

// Destructor
j1BetweenTransitions::~j1BetweenTransitions()
{}

// Called before render is available
bool j1BetweenTransitions::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1BetweenTransitions::Start()
{
	bool ret = true;

	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();

	CreateCursor();

	return ret;
}

// Called each loop iteration
bool j1BetweenTransitions::Update(float dt)
{
	bool ret = true;

	if (App->fade->IsFading()) {

		Uint32 total_time = App->fade->GetTotalTime();
		Uint32 now = App->fade->GetNow();

		bool player_death = false;

		if (App->entities->playerData != nullptr) {
			if (died && App->entities->playerData->player_is_dead) {
				player_death = true;
			}
		}

		switch (App->fade->GetStep()) {

		case fade_step::fade_to_black:

			if (now >= total_time - total_time / 2) {

				if (level_name && start) {
					l_level_name = CreateLevelNameText(scene_index);
					level_name = false;
				}

				if (player_death && you) {
					l_you = CreateYouDiedText(0);
					you = false;
				}
			}

			if (now >= total_time - total_time / 10) {

				if (!player_death) {
					if (cats_picked && start) {

						if (scene_index == 0)
							l_cats_picked = CreateCatsPickedText(0);
						else
							l_cats_picked = CreateCatsPickedText(0);

						cats_picked = false;
					}

					if (level_cats_picked && scene_index == 1) {
						if (start)
							l_level_cats_picked = CreateLevelCatsPickedText({ (int)width / 2, (int)height / 2 - 80 }, 1, App->scene->cats_first_map);
						else {
							l_level_cats_picked = CreateLevelCatsPickedText({ (int)width / 2, (int)height / 2 - 30 }, 1, App->scene->cats_first_map);
						}
					}
				}
				else {
					if (died) {
						l_died = CreateYouDiedText(1);
						died = false;
					}
				}
			}

			if (now >= total_time - total_time / 12) {

				if (level_cats_picked2 && scene_index == 1 && !start && !player_death) {
					l_level_cats_picked2 = CreateLevelCatsPickedText({ (int)width / 2, (int)height / 2 }, 2, App->scene->cats_second_map);
					level_cats_picked2 = false;
				}
			}

			if (now >= total_time - total_time / 14) {

				if (total_cats_picked && scene_index == 1 && !start && !player_death) {
					l_total_cats_picked = CreateTotalCatsPickedText(App->scene->cats_first_map, App->scene->cats_second_map);
					total_cats_picked = false;
				}

				if (player_death)
					bloody = true;
			}

			if (now >= total_time - total_time / 16) {

				if (score_text && scene_index == 1 && !start && !player_death) {
					l_score_text = CreateScoreText(1000);
					score_text = false;
				}
			}

			break;

		case fade_step::fade_from_black:

			if (now >= total_time - total_time / 3) {
				if (l_level_name != nullptr) {
					App->gui->DestroyElement(l_level_name);
					l_level_name = nullptr;
				}
				if (l_cats_picked != nullptr) {
					App->gui->DestroyElement(l_cats_picked);
					l_cats_picked = nullptr;
				}
				if (l_level_cats_picked != nullptr) {
					App->gui->DestroyElement(l_level_cats_picked);
					l_level_cats_picked = nullptr;
				}
				if (l_level_cats_picked2 != nullptr) {
					App->gui->DestroyElement(l_level_cats_picked2);
					l_level_cats_picked2 = nullptr;
				}
				if (l_total_cats_picked != nullptr) {
					App->gui->DestroyElement(l_total_cats_picked);
					l_total_cats_picked = nullptr;
				}
				if (l_score_text != nullptr) {
					App->gui->DestroyElement(l_score_text);
					l_score_text = nullptr;
				}
				if (l_you != nullptr) {
					App->gui->DestroyElement(l_you);
					l_you = nullptr;
				}
				if (l_died != nullptr) {
					App->gui->DestroyElement(l_died);
					l_died = nullptr;
				}

				bloody = false;
			}			

			break;

		case fade_step::none:
		default:
			break;
		}

		if (l_level_name != nullptr) {
			l_level_name->Draw();
			l_level_name->RandomAlphaPainting(dt, WarmYellow_, 255 / 2, 255 / 5, 255 / 2 + 255 / 3, 0.1f);
		}
		if (l_cats_picked != nullptr) {
			l_cats_picked->Draw();
		}
		if (l_level_cats_picked != nullptr) {
			l_level_cats_picked->Draw();
		}
		if (l_level_cats_picked2 != nullptr) {
			l_level_cats_picked2->Draw();
		}
		if (l_total_cats_picked != nullptr) {
			l_total_cats_picked->Draw();
		}
		if (l_score_text != nullptr) {
			l_score_text->Draw();
			l_score_text->RandomAlphaPainting(dt, WarmYellow_, 255 / 2, 255 / 5, 255 / 2 + 255 / 3, 0.1f);
		}
		if (l_you != nullptr) {
			if (bloody)
				l_you->RandomAlphaPainting(dt, BloodyRed_, 255 / 2, 255 / 5, 255 / 2 + 255 / 3, 0.1f);
			l_you->Draw();
		}
		if (l_died != nullptr) {
			if (bloody)
				l_died->RandomAlphaPainting(dt, BloodyRed_, 255 / 2, 255 / 5, 255 / 2 + 255 / 3, 0.1f);
			l_died->Draw();
		}
	}

	if (game_cursor != nullptr)
		game_cursor->DrawAbove();

	return ret;
}

// Called before quitting
bool j1BetweenTransitions::CleanUp()
{
	bool ret = true;

	return ret;
}

void j1BetweenTransitions::SetNextTransitionInfo(uint scene_index, bool start)
{
	this->scene_index = scene_index;
	this->start = start;

	level_name = true;
	cats_picked = true;
	level_cats_picked = true;
	level_cats_picked2 = true;
	total_cats_picked = true;
	score_text = true;
	you = true;
	died = true;
}

UILabel* j1BetweenTransitions::CreateLevelNameText(uint level)
{
	UILabel_Info label;
	label.draggable = false;
	label.interactive = false;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.vertical_orientation = UIElement_VERTICAL_POS::MIDDLE_;
	label.font_name = Font_Names::MSMINCHO_;
	label.normal_color = WarmYellow_;

	if (level == 0)
		label.text = "CONSUMED KING'S GARDEEP";
	else if (level == 1)
		label.text = "ANOR LONDEEP";

	return App->gui->CreateUILabel({ (int)width / 2,(int)height / 2 }, label);
}

UILabel* j1BetweenTransitions::CreateCatsPickedText(uint cats_picked)
{
	UILabel_Info label;
	label.draggable = false;
	label.interactive = false;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.font_name = Font_Names::MSMINCHO_;

	p2SString tmp("%s%d%s", "Cats picked ", cats_picked, "/5");
	label.text = tmp.GetString();

	return App->gui->CreateUILabel({ (int)width / 2,(int)height / 2 + 30 }, label);
}

UILabel* j1BetweenTransitions::CreateLevelCatsPickedText(iPoint position, uint level, uint cats_picked)
{
	UILabel_Info label;
	label.draggable = false;
	label.interactive = false;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.font_name = Font_Names::MSMINCHO_;
	label.normal_color = WarmYellow_;

	p2SString tmp("%s%d%s%d%s", "Level ", level, " Cats picked ", cats_picked, "/5");
	label.text = tmp.GetString();

	return App->gui->CreateUILabel({ position }, label);
}

UILabel* j1BetweenTransitions::CreateTotalCatsPickedText(uint first_level_cats_picked, uint second_level_cats_picked)
{
	UILabel_Info label;
	label.draggable = false;
	label.interactive = false;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.font_name = Font_Names::MSMINCHO_;

	p2SString tmp("%s%d%s", "Total ", first_level_cats_picked + second_level_cats_picked, "/10");
	label.text = tmp.GetString();

	return App->gui->CreateUILabel({ (int)width / 2,(int)height / 2 + 30 }, label);
}

UILabel* j1BetweenTransitions::CreateScoreText(int score)
{
	UILabel_Info label;
	label.draggable = false;
	label.interactive = false;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.font_name = Font_Names::MSMINCHO_;
	label.normal_color = WarmYellow_;

	p2SString tmp("%s%d", "SCORE ", 1000);
	label.text = tmp.GetString();

	return App->gui->CreateUILabel({ (int)width / 2,(int)height / 2 + 80 }, label);
}

UILabel* j1BetweenTransitions::CreateYouDiedText(uint word)
{
	UILabel_Info label;
	label.draggable = false;
	label.interactive = false;
	label.horizontal_orientation = UIElement_HORIZONTAL_POS::CENTER_;
	label.font_name = Font_Names::MSMINCHO_;

	if (word == 0) {
		label.text = "Y O U ";
		return App->gui->CreateUILabel({ (int)width / 2 - 100,(int)height / 2 }, label);
	}
	else {
		label.text = " D I E D";
		return App->gui->CreateUILabel({ (int)width / 2 + 100,(int)height / 2 }, label);
	}
}

UICursor* j1BetweenTransitions::CreateCursor() 
{
	// Cursor
	UICursor_Info cursor;
	cursor.tex_name = CURSORTEX_;
	cursor.default = { 0,0,16,16 };
	cursor.on_click = { 16,0,16,16 };

	game_cursor = App->gui->CreateUICursor(cursor, this);

	return game_cursor;
}