#ifndef __UILabel_H__
#define __UILabel_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"
#include "p2SString.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct _TTF_Font;
struct SDL_Color;

struct UILabel_Info {
	p2SString text;
	Font_Names font_name = Font_Names::DEFAULT_;
	SDL_Color normal_color = { 255,255,255,255 };
	SDL_Color hover_color = { 255,255,255,255 };
	SDL_Color pressed_color = { 255,255,255,255 };

	UIElement_HORIZONTAL_POS horizontal_orientation = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical_orientation = UIElement_VERTICAL_POS::TOP_;

	bool draggable = false;
	bool interactive = true;
};

// ---------------------------------------------------

class UILabel : public UIElement
{
public:
	UILabel(iPoint local_pos, UIElement* parent, UILabel_Info& info, j1Module* listener = nullptr);
	~UILabel();
	void Update(float dt);
	void HandleInput();
	void DebugDraw(iPoint blit_pos) const;

	void SetText(p2SString text);
	void SetColor(SDL_Color color, bool normal = false, bool hover = false, bool pressed = false);
	SDL_Color GetColor(bool normal = true, bool hover = false, bool pressed = false);

private:
	UILabel_Info label;
	_TTF_Font* font = nullptr;
	SDL_Color color = { 255,255,255,255 };
	UIEvents UIevent = NO_EVENT_;
	bool next_event = false;
};

#endif // __UILabel_H__