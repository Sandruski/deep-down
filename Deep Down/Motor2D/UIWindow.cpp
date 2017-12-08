#include "UIWindow.h"
#include "j1Input.h"
#include "p2Log.h"

UIWindow::UIWindow(int x, int y, UIWindow_Info& info, j1Module* listener) : UIElement(x, y, listener), window(info)
{
	type = UIElement_TYPE::WINDOW_;

	horizontal = info.horizontal_orientation;
	vertical = info.vertical_orientation;
	tex_area = info.tex_area;
	tex = App->gui->GetTexture(window.tex_name);

	if (tex_area.w == 0)
		SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &width, &height);
	else {
		width = tex_area.w;
		height = tex_area.h;
	}

	SetOrientation();
}

void UIWindow::Update(float dt)
{
	if (listener != nullptr)
		HandleInput();
}

void UIWindow::HandleInput()
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
			UIevent = MOUSE_LEFT_CLICK_;
			listener->OnUIEvent((UIElement*)this, UIevent);
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED) {
			next_event = false;
			LOG("MOUSE R CLICK START");

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
		listener->OnUIEvent((UIElement*)this, UIevent);
		UIevent = NO_EVENT_;
		break;
	}
}