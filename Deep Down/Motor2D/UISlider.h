#ifndef __UISlider_H__
#define __UISlider_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UISlider_Info {
	SDL_Rect tex_area = { 0,0,0,0 };
	SDL_Rect button_slider_area = { 0,0,0,0 };
	Tex_Names tex_name = Tex_Names::NO_TEX_;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;
	SDL_Color color = { 0,0,0,255 };
	iPoint slider_button_pos = { 0,0 };
	int offset = 0;
	int buggy_offset = 0;
	bool draggable = false;
};

// ---------------------------------------------------

class UISlider : public UIElement
{
public:
	UISlider(iPoint local_pos, UIElement* parent, UISlider_Info& info, j1Module* listener = nullptr);
	void Draw() const;
	void Update(float dt);
	void DebugDraw(iPoint blit_pos) const;
	void HandleInput();
	void SetColor(const SDL_Color color);
	SDL_Color GetColor();
	void SetNewRect(SDL_Rect& new_rect);
	SDL_Rect GetRect();

	uint GetPercent();

private:
	UISlider_Info slider;

	UIEvents UIevent = NO_EVENT_;
	bool next_event = false;

public:
	bool lets_move = false;
};

#endif // __UISlider_H__