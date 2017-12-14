#include "UISlider.h"
#include "j1Render.h"
#include "j1Window.h"

UISlider::UISlider(iPoint local_pos, UIElement* parent, UISlider_Info& info, j1Module* listener) : UIElement(local_pos, parent, listener), slider(info)
{
	type = UIElement_TYPE::IMAGE_;

	draggable = info.draggable;
	horizontal = info.horizontal_orientation;
	vertical = info.vertical_orientation;
	tex_area = info.tex_area;
	tex = App->gui->GetTexture(slider.tex_name);

	if (tex_area.w == 0)
		SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &width, &height);
	else {
		width = tex_area.w;
		height = tex_area.h;
	}

	SetOrientation();
}

void UISlider::Draw() const
{
	iPoint blit_pos;
	int scale = App->win->GetScale();
	blit_pos.x = (GetScreenPos().x - App->render->camera.x) / scale;
	blit_pos.y = (GetScreenPos().y - App->render->camera.y) / scale;

	App->render->Blit(tex, blit_pos.x, blit_pos.y, &tex_area);
	App->render->Blit(tex, blit_pos.x, blit_pos.y, &slider.button_slider_area);

	if (App->gui->debug_draw)
		DebugDraw(blit_pos);
}

void UISlider::DebugDraw(iPoint blit_pos) const
{
	Uint8 alpha = 80;

	SDL_Rect quad = { blit_pos.x, blit_pos.y, width, height };
	App->render->DrawQuad(quad, 255, 0, 40, alpha, false);
}

//---------------------------------------------------------------

void UISlider::SetColor(const SDL_Color color)
{
	slider.color = color;
}

SDL_Color UISlider::GetColor()
{
	return slider.color;
}

bool UISlider::FromAlphaToAlphaFade(float from, float to, float seconds)
{
	bool ret = false;

	if (reset) {
		start_time = SDL_GetTicks();
		reset = false;
	}

	// Math operations
	total_time = (Uint32)(seconds * 0.5f * 1000.0f);

	Uint32 now = (SDL_GetTicks() - start_time);
	float normalized = MIN(1.0f, (float)now / (float)total_time);
	float normalized2 = MIN(1.0f, (float)now / (float)total_time);
	normalized2 = 1 - normalized2;

	float alpha = (to - from) * normalized;
	float alpha2 = (from - to) * normalized2;

	// Color change
	if (from > to) {
		SetColor({ GetColor().r,GetColor().g,GetColor().b,(Uint8)alpha2 });
		if (alpha == from) {
			ResetFade();
			ret = true;
		}
	}
	else {
		SetColor({ GetColor().r,GetColor().g,GetColor().b,(Uint8)alpha });
		if (alpha2 == from) {
			ResetFade();
			ret = true;
		}
	}

	return ret;
}

void UISlider::ResetFade()
{
	reset = true;
}

void UISlider::SetNewRect(SDL_Rect& new_rect)
{
	tex_area = new_rect;
}

SDL_Rect UISlider::GetRect()
{
	return tex_area;
}