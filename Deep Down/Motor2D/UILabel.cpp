#include "UILabel.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"

UILabel::UILabel(iPoint local_pos, UIElement* parent, UILabel_Info& info, j1Module* listener) : UIElement(local_pos, parent, listener), label(info)
{
	type = UIElement_TYPE::LABEL_;

	is_draggable = info.is_draggable;
	is_interactable = info.is_interactable;
	horizontal = info.horizontal_orientation;
	vertical = info.vertical_orientation;
	font = App->gui->GetFont(label.font_name);
	color = label.normal_color;
	tex = App->font->Print(label.text.GetString(), color, font);
	App->font->CalcSize(label.text.GetString(), width, height, font);

	SetOrientation();
}

iPoint UILabel::GetSize() const 
{
	return { width,height };
}

void UILabel::Update(float dt)
{
	if (listener != nullptr && is_interactable)
		HandleInput();
}

void UILabel::HandleInput()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	switch (UIevent) {

	case UIEvents::NO_EVENT_:
		if (MouseHover()) {
			next_event = false;
			UIevent = UIEvents::MOUSE_ENTER_;
			SetColor(label.hover_color);
			listener->OnUIEvent((UIElement*)this, UIevent);	
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
			SetColor(label.pressed_color);
			UIevent = UIEvents::MOUSE_LEFT_CLICK_;
			listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED) {
			next_event = false;
			LOG("MOUSE R CLICK START");
			SetColor(label.pressed_color);

			mouse_click_pos.x = mouse_pos.x * App->win->GetScale() - GetLocalPos().x;
			mouse_click_pos.y = mouse_pos.y * App->win->GetScale() - GetLocalPos().y;

			if (is_draggable) {
				drag = true;
				App->gui->drag_to_true = true;
			}

			UIevent = UIEvents::MOUSE_RIGHT_CLICK_;
			listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}

		if (!next_event) {
			LOG("MOUSE ENTER");
			next_event = true;
		}

		break;
	case UIEvents::MOUSE_RIGHT_CLICK_:

		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED) {
			LOG("MOUSE R CLICK FINISH");

			if (is_draggable) {
				drag = false;
				App->gui->drag_to_false = true;
			}

			listener->OnUIEvent((UIElement*)this, UIevent);
			UIevent = UIEvents::MOUSE_ENTER_;
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
		SetColor(label.normal_color);
		listener->OnUIEvent((UIElement*)this, UIevent);
		UIevent = UIEvents::NO_EVENT_;
		break;
	}
}

void UILabel::DebugDraw(iPoint blit_pos) const
{
	Uint8 alpha = 80;

	SDL_Rect quad = { blit_pos.x, blit_pos.y, width, height };
	App->render->DrawQuad(quad, 10, 30, 5, alpha, false);
}

void UILabel::SetText(p2SString text)
{
	tex = App->font->Print(text.GetString(), color, font);
}

void UILabel::SetColor(SDL_Color color)
{
	this->color = color;
	tex = App->font->Print(label.text.GetString(), color, font);
}

UILabel::~UILabel()
{
	font = nullptr;
}