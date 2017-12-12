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
	bool quad = false;
	SDL_Color color = { 0,0,0,255 };

	bool draggable = false;
};

// ---------------------------------------------------

class UIImage : public UIElement
{
public:
	UIImage(iPoint local_pos, UIElement* parent, UIImage_Info& info, j1Module* listener = nullptr);
	void Draw() const;
	void DebugDraw(iPoint blit_pos) const;
	void SetColor(const SDL_Color color);

private:
	UIImage_Info image;
};

#endif // __UIImage_H__