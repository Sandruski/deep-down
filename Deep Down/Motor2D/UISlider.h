#ifndef __UISlider_H__
#define __UISlider_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UISlider_Info {
	SDL_Rect pointer_area = { 0,0,0,0 };
	SDL_Rect size_area = { 0,0,0,0 };
	SDL_Rect ticks_area = { 0,0,0,0 };
	Tex_Names tex_name = Tex_Names::NO_TEX_;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;
	uint size;
	uint level;

	bool is_draggable = false;
};

// ---------------------------------------------------

class UISlider : public UIElement
{
public:
	UISlider(iPoint local_pos, UIElement* parent, UISlider_Info& info, j1Module* listener = nullptr);
	void Draw() const;
	void DebugDraw(iPoint blit_pos) const;

	SDL_Rect total_tex_area;

private:
	UISlider_Info slider;
};

#endif // __UISlider_H__