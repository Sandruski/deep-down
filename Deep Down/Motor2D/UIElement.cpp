#include "j1App.h"
#include "UIElement.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Window.h"

UIElement::UIElement(int x, int y, j1Module* listener) : position(x, y), listener(listener) 
{
	uint width = 0, height = 0, scale = 0;

	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();

	screen = { 0, 0, static_cast<int>(width * scale), static_cast<int>(height * scale) };

}

UIElement::~UIElement()
{
	if (tex != nullptr)
		App->tex->UnLoad((SDL_Texture*)tex);
	tex = nullptr;
}

void UIElement::Update(float dt) {}

void UIElement::UpdateDragging(float dt)
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	if (mouse_click_pos.x < 0)
		position.x = mouse_pos.x + abs(mouse_click_pos.x);
	else
		position.x = mouse_pos.x - mouse_click_pos.x;

	if (mouse_click_pos.y < 0)
		position.y = mouse_pos.y + abs(mouse_click_pos.y);
	else
		position.y = mouse_pos.y - mouse_click_pos.y;
}

void UIElement::Draw() const
{
	if (tex_area.w != 0)
		App->render->Blit(tex, position.x, position.y, &tex_area);
	else
		App->render->Blit(tex, position.x, position.y);

	if (App->gui->debug_draw)
		DebugDraw();
}

void UIElement::DebugDraw() const
{
	Uint8 alpha = 80;
	SDL_Rect quad = { position.x, position.y, width, height };
	App->render->DrawQuad(quad, 255, 255, 255, alpha, false);
}

void UIElement::HandleInput() {}

bool UIElement::HasToBeRemoved() const
{
	return to_remove;
}

UIElement_TYPE UIElement::GetType() const
{
	return type;
}

bool UIElement::MouseHover() const
{
	int x, y;
	int scale = App->win->GetScale();

	App->input->GetMousePosition(x, y);
	x -= App->render->camera.x/scale;
	y -= App->render->camera.y/scale;

	return x > position.x && x < position.x + width && y > position.y && y < position.y + height;
}

void UIElement::SetOrientation()
{
	switch (horizontal) {
	case UIElement_HORIZONTAL_POS::LEFT_:
		break;
	case UIElement_HORIZONTAL_POS::RIGHT_:
		position.x -= width;
		break;
	case UIElement_HORIZONTAL_POS::CENTER_:
		position.x -= width / 2;
		break;
	}

	switch (vertical) {
	case UIElement_VERTICAL_POS::TOP_:
		break;
	case UIElement_VERTICAL_POS::BOTTOM_:
		position.y -= height;
		break;
	case UIElement_VERTICAL_POS::MIDDLE_:
		position.y -= height / 2;
		break;
	}
}

iPoint UIElement::GetPosition() const
{
	return position;
}