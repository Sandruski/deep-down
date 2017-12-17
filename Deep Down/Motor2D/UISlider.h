#ifndef __UISlider_H__
#define __UISlider_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UISlider_Info {
	UIElement_Rect tex_area = UIElement_Rect::NO_ELEMENT_RECT_;
	UIElement_Rect button_slider_area = UIElement_Rect::NO_ELEMENT_RECT_;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;
	SDL_Color color = { 0,0,0,255 };
	iPoint slider_button_pos = { 0,0 };
	int offset = 0;
	int buggy_offset = 0;

	bool draggable = false;
	bool interactive = true;
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
	void SetNewRect(SDL_Rect& new_rect);
	SDL_Rect GetRect();
	uint GetPercent();

	bool SlideTransition(float dt, int end_pos_y, float speed = 10.0f, bool bounce = true, float bounce_interval = 1.0f, float bounce_speed = 2.0f, bool down = true);
	bool Bounce(float dt, float bounce_interval = 1.0f, float bounce_speed = 2.0f, bool down = true);
	void InitializeBounce(float bounce_interval = 1.0f, bool down = true);

private:
	UISlider_Info slider;

	UIEvents UIevent = NO_EVENT_;
	bool next_event = false;

	SDL_Rect button_slider_area = { 0,0,0,0 };

public:
	bool lets_move = false;

	// Bounce parameters
	float bounce_value = 0.0f;
	iPoint start_pos = { 0,0 };
	bool first_bounce = true;
	bool reset = true;
	bool start_bouncing = false;
};

#endif // __UISlider_H__