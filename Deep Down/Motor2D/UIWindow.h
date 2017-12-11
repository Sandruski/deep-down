#ifndef __UIWindow_H__
#define __UIWindow_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UIWindow_Info {
	SDL_Rect tex_area = { 0,0,0,0 };
	Tex_Names tex_name = Tex_Names::NO_TEX_;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;

	bool is_draggable = false;
};

// ---------------------------------------------------

class UIWindow : public UIElement
{
public:
	UIWindow(iPoint local_pos, UIElement* parent, UIWindow_Info& info, j1Module* listener = nullptr);
	void Update(float dt);
	void HandleInput();
	void DebugDraw(iPoint blit_pos) const;

private:
	UIWindow_Info window;
	UIEvents UIevent = NO_EVENT_;
	bool next_event = false;
};

#endif // __UIWindow_H__