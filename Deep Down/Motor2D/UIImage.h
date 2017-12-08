#ifndef __UIImage_H__
#define __UIImage_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UIImage_Info {
	SDL_Rect tex_area = { 0,0,0,0 };
	Tex_Names tex_name = Tex_Names::NO_TEX_;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;
};

// ---------------------------------------------------

class UIImage : public UIElement
{
public:
	UIImage(int x, int y, UIImage_Info& info, j1Module* listener = nullptr);
	void DebugDraw() const;

private:
	UIImage_Info image;
};

#endif // __UIImage_H__