#include "UIImage.h"
#include "j1Render.h"

UIImage::UIImage(int x, int y, UIImage_Info& info, j1Module* listener) : UIElement(x, y, listener), image(info)
{
	type = UIElement_TYPE::IMAGE_;

	horizontal = info.horizontal_orientation;
	vertical = info.vertical_orientation;
	tex_area = info.tex_area;
	tex = App->gui->GetTexture(image.tex_name);

	if (tex_area.w == 0)
		SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &width, &height);
	else {
		width = tex_area.w;
		height = tex_area.h;
	}

	SetOrientation();
}

void UIImage::Draw() const
{
	if (image.quad) {
		SDL_SetRenderDrawColor(App->render->renderer, image.color.r, image.color.g, image.color.b, image.color.a);
		SDL_RenderFillRect(App->render->renderer, &image.tex_area);

	}
	else {
		if (tex_area.w != 0)
			App->render->Blit(tex, position.x, position.y, &tex_area);
		else
			App->render->Blit(tex, position.x, position.y);
	}

	if (App->gui->debug_draw)
		DebugDraw();
}

void UIImage::DebugDraw() const
{
	SDL_Rect quad = { position.x, position.y, width, height };
	App->render->DrawQuad(quad, 255, 0, 255, 255, false);
}

void UIImage::SetColor(const SDL_Color color) {	image.color = color; }