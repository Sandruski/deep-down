#ifndef __UILifeBar_H__
#define __UILifeBar_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UILifeBar_Info {
	SDL_Rect tex_area = { 0,0,0,0 };
	Tex_Names tex_name = Tex_Names::NO_TEX_;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;

	int life = 0;
	iPoint life_bar_position = { 0,0 };
	SDL_Rect bar = { 0,0,0,0 };

	bool is_draggable = false;
	bool is_interactable = true;
	
	void SetLifeBarPosition(iPoint pos) { life_bar_position = pos; }

};

// ---------------------------------------------------

class UILifeBar : public UIElement
{
public:
	UILifeBar(iPoint local_pos, UIElement* parent, UILifeBar_Info& info, j1Module* listener = nullptr);
	void DebugDraw(iPoint blit_pos) const;

	void Draw() const;
	void Update(float dt);
	void HandleInput();

	void SetLifeProgress(const int life);
	void IncreaseLifeProgress(const int life);
	void DecreaseLifeProgress(const int life);

private:
	UIEvents UIevent = NO_EVENT_;
	bool next_event = false;
	UILifeBar_Info life_bar;
};

#endif // __UILifeBar_H__