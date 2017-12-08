#include "UILifeBar.h"
#include "j1Render.h"

UILifeBar::UILifeBar(int x, int y, UILifeBar_Info& info, j1Module* listener) : UIElement(x, y, listener), life_bar(info)
{
	type = UIElement_TYPE::LIFEBAR_;

	horizontal = info.horizontal_orientation;
	vertical = info.vertical_orientation;
	tex_area = info.tex_area;
	tex = App->gui->GetTexture(life_bar.tex_name);

	if (tex_area.w == 0)
		SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &width, &height);
	else {
		width = tex_area.w;
		height = tex_area.h;
	}

	SetOrientation();
}

void UILifeBar::Update(float dt)
{
	life_bar.bar.w = life_bar.life;
}

void UILifeBar::DebugDraw() const
{
	SDL_Rect quad = { position.x, position.y, width, height };
	App->render->DrawQuad(quad, 255, 0, 255, 255, false);
}

void UILifeBar::Draw() const
{
	App->render->Blit(tex, life_bar.life_bar_position.x, life_bar.life_bar_position.y, &life_bar.bar);
	App->render->Blit(tex, position.x, position.y, &tex_area);
}

void UILifeBar::SetLifeProgress(const int life)
{
	life_bar.life = life;
}
void UILifeBar::DecreaseLifeProgress(const int life)
{
	life_bar.life -= life;
}