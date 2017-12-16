#include "UICursor.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"

UICursor::UICursor(iPoint local_pos, UIElement* parent, UICursor_Info& info, j1Module* listener) : UIElement(local_pos, parent, listener), cursor(info)
{
	type = UIElement_TYPE::CURSOR_;
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

void UICursor::HandleInput()
{
	switch (UIevent)
	{
	case UIEvents::NO_EVENT_:
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED) {
			UIevent = UIEvents::MOUSE_LEFT_CLICK_;
			tex_area = cursor.on_click;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED) {
			UIevent = UIEvents::MOUSE_RIGHT_CLICK_;
			tex_area = cursor.on_click;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		break;
	case UIEvents::MOUSE_LEFT_CLICK_:
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED) {
			UIevent = UIEvents::NO_EVENT_;
			tex_area = cursor.default;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		break;
	case UIEvents::MOUSE_RIGHT_CLICK_:
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED) {
			UIevent = UIEvents::NO_EVENT_;
			tex_area = cursor.default;
			listener->OnUIEvent(this, UIevent);
			break;
		}
		break;
	}
}

//---------------------------------------------------------------

void UICursor::SetMousePosition()
{
	int scale = App->win->GetScale();
	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	mouse_position.x *= scale;
	mouse_position.y *= scale;
	SetLocalPos(mouse_position);
}