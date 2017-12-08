#include "UIButton.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Render.h"

UIButton::UIButton(int x, int y, UIButton_Info& info, j1Module* listener) : UIElement(x, y, listener), button(info)
{
	type = UIElement_TYPE::BUTTON_;

	horizontal = info.horizontal_orientation;
	vertical = info.vertical_orientation;
	tex_area = info.normal_tex_area;
	tex = App->gui->GetTexture(button.tex_name);

	if (tex_area.w == 0)
		SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &width, &height);
	else {
		width = tex_area.w;
		height = tex_area.h;
	}

	SetOrientation();
}

void UIButton::Update(float dt)
{
	if (listener != nullptr)
		HandleInput();
}

void UIButton::DebugDraw() const
{
	SDL_Rect quad = { position.x, position.y, width, height };
	App->render->DrawQuad(quad, 255, 255, 0, 255, false);
}

void UIButton::HandleInput()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	switch (UIevent) {

	case NO_EVENT_:
		if (MouseHover() || tab) {
			next_event = false;
			UIevent = MOUSE_ENTER_;
			break;
		}
		break;
	case MOUSE_ENTER_:

		if (!MouseHover() && !tab) {
			LOG("MOUSE LEAVE");
			next_event = false;
			UIevent = MOUSE_LEAVE_;
			break;
		}
		else if ((!tab && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED) || (tab && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)) {
			next_event = false;
			LOG("MOUSE L CLICK START");
			ChangeSprite(button.pressed_tex_area);
			UIevent = MOUSE_LEFT_CLICK_;

			if (button.checkbox)
				checked = !checked;
			//listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}
		else if ((!tab && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED) || (tab && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)) {
			next_event = false;
			LOG("MOUSE R CLICK START");
			ChangeSprite(button.pressed_tex_area);

			mouse_click_pos.x = mouse_pos.x;
			mouse_click_pos.y = mouse_pos.y;

			drag = true;
			App->gui->drag_to_true = true;

			UIevent = MOUSE_RIGHT_CLICK_;

			if (button.checkbox)
				checked = !checked;
			//listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}

		if (!next_event) {
			LOG("MOUSE ENTER");
			if (!checked)
				ChangeSprite(button.hover_tex_area);
			listener->OnUIEvent((UIElement*)this, UIevent);
			next_event = true;
		}

		break;
	case MOUSE_RIGHT_CLICK_:

		if ((!tab && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED) || (tab && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)) {
			LOG("MOUSE R CLICK FINISH");

			drag = false;
			App->gui->drag_to_false = true;

			listener->OnUIEvent((UIElement*)this, UIevent);
			UIevent = MOUSE_ENTER_;
			break;
		}

		break;
	case MOUSE_LEFT_CLICK_:

		if (!MouseHover() && !tab) {
			LOG("MOUSE LEAVE");
			UIevent = MOUSE_LEAVE_;
			break;
		}
		else if ((!tab && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED) || (tab && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)) {
			LOG("MOUSE L CLICK FINISH");
			listener->OnUIEvent((UIElement*)this, UIevent);
			UIevent = MOUSE_ENTER_;
			break;
		}

		break;
	case MOUSE_LEAVE_:
		if (!checked)
			ChangeSprite(button.normal_tex_area);
		//listener->OnUIEvent((UIElement*)this, UIevent);
		UIevent = NO_EVENT_;
		break;
	}
}

void UIButton::ChangeSprite(SDL_Rect tex_area)
{
	this->tex_area = tex_area;
}

SDL_Rect UIButton::GetHoverSprite() const
{
	return button.hover_tex_area;
}

SDL_Rect UIButton::GetPressedSprite() const
{
	return button.pressed_tex_area;
}

SDL_Rect UIButton::GetNormalSprite() const
{
	return button.normal_tex_area;
}

UIEvents UIButton::GetActualEvent() const
{
	return UIevent;
}