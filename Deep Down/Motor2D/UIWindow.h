#ifndef __UIWindow_H__
#define __UIWindow_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UIWindow_Info {
	UIElement_Rect tex_area = UIElement_Rect::NO_ELEMENT_RECT_;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;

	bool draggable = false;
	bool interactive = true;
};

// ---------------------------------------------------

class UIWindow : public UIElement
{
public:
	UIWindow(iPoint local_pos, UIElement* parent, UIWindow_Info& info, j1Module* listener = nullptr);
	void Update(float dt);
	void HandleInput();
	void DebugDraw(iPoint blit_pos) const;

	bool SlideTransition(float dt, int end_pos_y, float speed = 10.0f, bool bounce = true, float bounce_interval = 1.0f, float bounce_speed = 2.0f, bool down = true);
	bool Bounce(float dt, float bounce_interval = 1.0f, float bounce_speed = 2.0f, bool down = true);
	void InitializeBounce(float bounce_interval = 1.0f, bool down = true);

private:
	UIWindow_Info window;
	UIEvents UIevent = NO_EVENT_;
	bool next_event = false;

	// Bounce parameters
	float bounce_value = 0.0f;
	iPoint start_pos = { 0,0 };
	bool first_bounce = true;
	bool reset = true;
	bool start_bouncing = false;
};

#endif // __UIWindow_H__