#include "UILifeBar.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Window.h"

UILifeBar::UILifeBar(iPoint local_pos, UIElement* parent, UILifeBar_Info& info, j1Module* listener) : UIElement(local_pos, parent, listener), life_bar(info)
{
	type = UIElement_TYPE::LIFEBAR_;

	draggable = info.draggable;
	interactive = info.interactive;
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
	if (listener != nullptr && interactive)
		HandleInput();

	life_bar.bar.w = life_bar.life;
}

void UILifeBar::Draw() const
{
	iPoint blit_pos;
	int scale = App->win->GetScale();
	blit_pos.x = (GetScreenPos().x - App->render->camera.x) / scale;
	blit_pos.y = (GetScreenPos().y - App->render->camera.y) / scale;

	App->render->Blit(tex, blit_pos.x + life_bar.life_bar_position.x, blit_pos.y + life_bar.life_bar_position.y, &life_bar.backgorund_life_bar);
	App->render->Blit(tex, blit_pos.x + life_bar.life_bar_position.x, blit_pos.y + life_bar.life_bar_position.y, &life_bar.bar);
	App->render->Blit(tex, blit_pos.x, blit_pos.y, &tex_area);

	if (App->gui->debug_draw)
		DebugDraw(blit_pos);
}

void UILifeBar::DebugDraw(iPoint blit_pos) const
{
	Uint8 alpha = 80;

	SDL_Rect quad = { blit_pos.x, blit_pos.y, width, height };
	App->render->DrawQuad(quad, 10, 100, 255, alpha, false);
}

void UILifeBar::HandleInput()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	switch (UIevent) {

	case UIEvents::NO_EVENT_:
		if (MouseHover()) {
			next_event = false;
			UIevent = UIEvents::MOUSE_ENTER_;
			break;
		}
		break;
	case UIEvents::MOUSE_ENTER_:

		if (!MouseHover()) {
			LOG("MOUSE LEAVE");
			next_event = false;
			UIevent = UIEvents::MOUSE_LEAVE_;
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED) {
			next_event = false;
			LOG("MOUSE L CLICK START");
			UIevent = UIEvents::MOUSE_LEFT_CLICK_;
			listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED) {
			next_event = false;
			LOG("MOUSE R CLICK START");

			mouse_click_pos.x = mouse_pos.x * App->win->GetScale() - GetLocalPos().x;
			mouse_click_pos.y = mouse_pos.y * App->win->GetScale() - GetLocalPos().y;

			if (draggable) {
				drag = true;
				App->gui->drag_to_true = true;
			}

			UIevent = UIEvents::MOUSE_RIGHT_CLICK_;
			listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}

		if (!next_event) {
			LOG("MOUSE ENTER");
			listener->OnUIEvent((UIElement*)this, UIevent);
			next_event = true;
		}

		break;
	case UIEvents::MOUSE_RIGHT_CLICK_:

		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED) {
			LOG("MOUSE R CLICK FINISH");

			if (draggable) {
				drag = false;
				App->gui->drag_to_false = true;
			}

			listener->OnUIEvent((UIElement*)this, UIevent);
			UIevent = MOUSE_ENTER_;
			break;
		}

		break;
	case UIEvents::MOUSE_LEFT_CLICK_:

		if (!MouseHover()) {
			LOG("MOUSE LEAVE");
			UIevent = UIEvents::MOUSE_LEAVE_;
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED) {
			LOG("MOUSE L CLICK FINISH");
			// Uncomment next line and create a new UIEvent if you want to execute a function when mouse button is released
			//listener->OnUIEvent((UIElement*)this, UIevent);
			UIevent = MOUSE_ENTER_;
			break;
		}

		break;
	case UIEvents::MOUSE_LEAVE_:
		listener->OnUIEvent((UIElement*)this, UIevent);
		UIevent = UIEvents::NO_EVENT_;
		break;
	}
}

//---------------------------------------------------------------

void UILifeBar::SetLifeProgress(const int life)
{
	life_bar.life = life;
}

void UILifeBar::IncreaseLifeProgress(const int life)
{
	life_bar.life += life;
	if (life_bar.life > life_bar.backgorund_life_bar.w)
		life_bar.life = life_bar.backgorund_life_bar.w;
}

void UILifeBar::DecreaseLifeProgress(const int life)
{
	life_bar.life -= life;
	if (life_bar.life < 0)
		life_bar.life = 0;

}