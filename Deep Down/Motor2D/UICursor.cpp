#include "UICursor.h"
#include "j1Input.h"
#include "j1Render.h"

UICursor::UICursor(UICursor_Info& info, j1Module* listener) : UIElement(0, 0, listener), cursor(info)
{
	type = UIElement_TYPE::IMAGE_;
	tex_area = info.default;
	tex = App->gui->GetTexture(cursor.tex_name);

	if (tex_area.w == 0)
		SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &width, &height);
	else {
		width = tex_area.w;
		height = tex_area.h;
	}

	SDL_ShowCursor(0);
}

void UICursor::Update(float dt)
{
	SetMousePosition();

	if (listener != nullptr)
		HandleInput();
}

void UICursor::SetMousePosition()
{
	App->input->GetMousePosition(position.x, position.y);
	position.x -= App->render->camera.x;
	position.y -= App->render->camera.y;

}

void UICursor::HandleInput()
{
	switch (UIevent)
	{
	case NO_EVENT_:
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED) {
			UIevent = MOUSE_LEFT_CLICK_;
			tex_area = cursor.on_click;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED) {
			UIevent = MOUSE_RIGHT_CLICK_;
			tex_area = cursor.on_click;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		break;
	case MOUSE_LEFT_CLICK_:
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED) {
			UIevent = NO_EVENT_;
			tex_area = cursor.default;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		break;
	case MOUSE_RIGHT_CLICK_:
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED) {
			UIevent = NO_EVENT_;
			tex_area = cursor.default;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		break;
	}



	enum UIEvents {
		NO_EVENT_,
		MOUSE_ENTER_,
		MOUSE_LEAVE_,
		MOUSE_RIGHT_CLICK_,
		MOUSE_LEFT_CLICK_,
		MOUSE_UP_,
		MAX_EVENTS_
	};

}