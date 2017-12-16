#include "UISlider.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "p2Log.h"

UISlider::UISlider(iPoint local_pos, UIElement* parent, UISlider_Info& info, j1Module* listener) : UIElement(local_pos, parent, listener), slider(info)
{
	type = UIElement_TYPE::IMAGE_;

	draggable = info.draggable;
	interactive = info.interactive;
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

void UISlider::Update(float dt)
{
	if (interactive) {
		int scale = App->win->GetScale();
		int mouseX = 0;
		int mouseY = 0;
		App->input->GetMousePosition(mouseX, mouseY);

		if (App->input->GetMouseButtonDown((SDL_BUTTON_LEFT) == KEY_DOWN) && mouseX > GetScreenPos().x / scale + slider.offset && mouseX < (GetScreenPos().x / scale + tex_area.w - slider.button_slider_area.w - slider.offset) && mouseY >  GetScreenPos().y / scale && mouseY < GetScreenPos().y / scale + tex_area.h)
			lets_move = true;
		else if (App->input->GetMouseButtonDown((SDL_BUTTON_LEFT) == KEY_UP))
			lets_move = false;

		if (lets_move) {
			slider.slider_button_pos.x = mouseX - GetScreenPos().x / scale;
		}

		if (listener != nullptr)
			HandleInput();
	}
}

void UISlider::Draw() const
{
	iPoint blit_pos;
	int scale = App->win->GetScale();
	blit_pos.x = (GetScreenPos().x - App->render->camera.x) / scale;
	blit_pos.y = (GetScreenPos().y - App->render->camera.y) / scale;

	App->render->Blit(tex, blit_pos.x, blit_pos.y, &tex_area);
	App->render->Blit(tex, blit_pos.x + slider.slider_button_pos.x, blit_pos.y, &slider.button_slider_area);

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

void UISlider::HandleInput()
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
			UIevent = MOUSE_LEFT_UP_;
			listener->OnUIEvent((UIElement*)this, UIevent);
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

void UISlider::SetNewRect(SDL_Rect& new_rect)
{
	tex_area = new_rect;
}

SDL_Rect UISlider::GetRect()
{
	return tex_area;
}

uint UISlider::GetPercent()
{
	return 100 * (slider.slider_button_pos.x + slider.buggy_offset - slider.offset) / (tex_area.w - slider.offset);
}

bool UISlider::SlideTransition(float dt, int end_pos_y, float speed, bool bounce, float bounce_interval, float bounce_speed, bool down)
{
	bool ret = false;

	iPoint pos = GetLocalPos();

	if (down) {
		if (pos.y + height >= (int)end_pos_y - height / 2) {
			if (bounce && !start_bouncing)
				start_bouncing = true;
			else if (!bounce)
				ret = true;
		}
		else if (!start_bouncing)
			IncreasePos({ 0,(int)(speed * dt) });
	}
	else {
		if (pos.y + height <= (int)end_pos_y - height / 2) {
			if (bounce && !start_bouncing)
				start_bouncing = true;
			else if (!bounce)
				ret = true;
		}
		else if (!start_bouncing)
			DecreasePos({ 0,(int)(speed * dt) });
	}

	if (start_bouncing) {
		if (Bounce(dt, bounce_interval, bounce_speed, down)) {
			ret = true;
		}
	}

	return ret;
}

bool UISlider::Bounce(float dt, float bounce_interval, float bounce_speed, bool down)
{
	bool ret = false;

	iPoint pos = GetLocalPos();

	if (reset) {
		InitializeBounce(bounce_interval, down);
		reset = false;
	}

	if (bounce_value <= bounce_speed)
		ret = true;

	if (first_bounce) {
		if (pos.y >= start_pos.y + bounce_value) {
			bounce_value -= bounce_speed;
			first_bounce = false;
		}
		else
			IncreasePos({ 0, (int)(bounce_value * 10.0f * dt) });
	}
	else {
		if (pos.y <= start_pos.y - bounce_value) {
			bounce_value -= bounce_speed;
			first_bounce = true;
		}
		else
			DecreasePos({ 0, (int)(bounce_value * 10.0f * dt) });
	}

	return ret;
}

void UISlider::InitializeBounce(float bounce_interval, bool down)
{
	bounce_value = bounce_interval;
	start_pos = GetLocalPos();

	if (!down)
		first_bounce = false;
}