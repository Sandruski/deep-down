#ifndef __UICursor_H__
#define __UICursor_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "UIElement.h"
#include "j1App.h"
#include "j1Gui.h"

struct UICursor_Info {
	SDL_Rect default = { 0,0,0,0 };
	SDL_Rect on_click = { 0,0,0,0 };
	Tex_Names tex_name = Tex_Names::NO_TEX_;

};

// ---------------------------------------------------

class UICursor : public UIElement
{
public:
	UICursor(UICursor_Info& info, j1Module* listener = nullptr);
	void Update(float dt);
	void SetMousePosition();
	void HandleInput();

private:
	UIEvents UIevent = NO_EVENT_;
	UICursor_Info cursor;
};

#endif // __UIImage_H__