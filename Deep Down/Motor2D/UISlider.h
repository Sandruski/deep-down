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

	bool draggable = false;
};

// ---------------------------------------------------

class UISlider : public UIElement
{
public:
	UISlider(iPoint local_pos, UIElement* parent, UISlider_Info& info, j1Module* listener = nullptr);
	void Draw() const;
	void DebugDraw(iPoint blit_pos) const;
	void SetColor(const SDL_Color color);
	SDL_Color GetColor();
	void SetNewRect(SDL_Rect& new_rect);
	SDL_Rect GetRect();

	void ResetFade();
	bool FromAlphaToAlphaFade(float from = 0.0f, float to = 0.0f, float seconds = 1.0f);

private:
	UISlider_Info slider;

	// Fade parameters
	float total_time = 0.0f;
	float start_time = 0.0f;
	bool reset = true;
};

#endif // __UISlider_H__