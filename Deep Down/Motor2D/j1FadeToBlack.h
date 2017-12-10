#ifndef __j1MODULEFADETOBLACK_H__
#define __j1MODULEFADETOBLACK_H__

#include "j1Module.h"

#include "Animation.h"

#include "SDL\include\SDL_rect.h"

enum fades
{
	no_fade,
	normal_fade,
	slider_fade,
	total_black_fade
};

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f, fades kind_of_fade = normal_fade);

	void NormalFade();
	void SliderFade();
	void BlackFade();
	bool IsFading() const;

	j1Module* off;
	j1Module* on;

private:
	
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	SDL_Rect Slider_rect;
	float dt = 0.0f;
	fades thisFade;

public:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	fade_step GetStep() { return current_step; }
};

#endif // __j1MODULEFADETOBLACK_H__
