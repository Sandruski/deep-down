#ifndef __UIButton_H__
#define __UIButton_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UIButton_Info {
	SDL_Rect normal_tex_area = { 0,0,0,0 };
	SDL_Rect hover_tex_area = { 0,0,0,0 };
	SDL_Rect pressed_tex_area = { 0,0,0,0 };

	Tex_Names tex_name = Tex_Names::NO_TEX_;
	bool checkbox = false;
	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;
};

// ---------------------------------------------------

class UIButton : public UIElement
{
public:
	UIButton(int x, int y, UIButton_Info& info, j1Module* listener = nullptr);
	void Update(float dt);
	void HandleInput();
	void DebugDraw() const;

	void ChangeSprite(SDL_Rect tex_area);
	SDL_Rect GetHoverSprite() const;
	SDL_Rect GetPressedSprite() const;
	SDL_Rect GetNormalSprite() const;
	UIEvents GetActualEvent() const;

private:
	UIButton_Info button;
	UIEvents UIevent = NO_EVENT_;
	bool checked = false;

public:
	bool tab = false;
	bool next_event = false;

};

#endif // __UIButton_H__