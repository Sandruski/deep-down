#include "UILabel.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Render.h"

UILabel::UILabel(int x, int y, UILabel_Info& info, j1Module* listener) : UIElement(x, y, listener), label(info)
{
	type = UIElement_TYPE::LABEL_;

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
	if (listener != nullptr)
		HandleInput();
}

void UILabel::HandleInput()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	switch (UIevent) {

	case NO_EVENT_:
		if (MouseHover()) {
			next_event = false;
			UIevent = MOUSE_ENTER_;
			break;
		}
		break;
	case MOUSE_ENTER_:

		if (!MouseHover()) {
			LOG("MOUSE LEAVE");
			next_event = false;
			UIevent = MOUSE_LEAVE_;
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED) {
			next_event = false;
			LOG("MOUSE L CLICK START");
			SetColor(label.pressed_color);
			UIevent = MOUSE_LEFT_CLICK_;
			listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED) {
			next_event = false;
			LOG("MOUSE R CLICK START");
			SetColor(label.pressed_color);

			mouse_click_pos.x = mouse_pos.x;
			mouse_click_pos.y = mouse_pos.y;

			drag = true;
			App->gui->drag_to_true = true;

			UIevent = MOUSE_RIGHT_CLICK_;
			listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}

		if (!next_event) {
			LOG("MOUSE ENTER");
			SetColor(label.hover_color);
			listener->OnUIEvent((UIElement*)this, UIevent);
			next_event = true;
		}

		break;
	case MOUSE_RIGHT_CLICK_:

		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED) {
			LOG("MOUSE R CLICK FINISH");

			drag = false;
			App->gui->drag_to_false = true;

			listener->OnUIEvent((UIElement*)this, UIevent);
			UIevent = MOUSE_ENTER_;
			break;
		}

		break;
	case MOUSE_LEFT_CLICK_:

		if (!MouseHover()) {
			LOG("MOUSE LEAVE");
			UIevent = MOUSE_LEAVE_;
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
	case MOUSE_LEAVE_:
		SetColor(label.normal_color);
		listener->OnUIEvent((UIElement*)this, UIevent);
		UIevent = NO_EVENT_;
		break;
	}
}

void UILabel::DebugDraw() const
{
	SDL_Rect quad = { position.x, position.y, width, height };
	App->render->DrawQuad(quad, 0, 255, 255, 255, false);
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