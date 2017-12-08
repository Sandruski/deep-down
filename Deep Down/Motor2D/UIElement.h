#ifndef __UIElement_H__
#define __UIElement_H__

#include "SDL\include\SDL.h"
#include "p2Defs.h"

#include "p2Point.h"

class j1Module;

enum UIElement_TYPE
{
	NO_TYPE_,
	IMAGE_,
	LABEL_,
	BUTTON_,
	WINDOW_,
	LIFEBAR_,
	MAX_TYPES_
};

enum UIElement_HORIZONTAL_POS {
	LEFT_,
	RIGHT_,
	CENTER_
};

enum UIElement_VERTICAL_POS {
	TOP_,
	BOTTOM_,
	MIDDLE_
};

enum UIEvents {
	NO_EVENT_,
	MOUSE_ENTER_,
	MOUSE_LEAVE_,
	MOUSE_RIGHT_CLICK_,
	MOUSE_LEFT_CLICK_,
	MOUSE_UP_,
	MAX_EVENTS_
};

// ---------------------------------------------------

class UIElement
{
protected:
	iPoint position = { 0,0 };
	UIElement_TYPE type = UIElement_TYPE::NO_TYPE_;
	UIElement_HORIZONTAL_POS horizontal = UIElement_HORIZONTAL_POS::LEFT_;
	UIElement_VERTICAL_POS vertical = UIElement_VERTICAL_POS::TOP_;
	j1Module* listener = nullptr;

	bool to_remove = false;
	//bool draggable = false;

	// Texture parameters
	const SDL_Texture* tex = nullptr;
	SDL_Rect tex_area = { 0,0,0,0 };
	int width = 0, height = 0;

public:
	UIElement(int x, int y, j1Module* listener);

	virtual ~UIElement();

	virtual void Update(float dt);

	virtual void Draw() const;

	virtual void HandleInput();

	virtual bool HasToBeRemoved() const;

	virtual UIElement_TYPE GetType() const;

	virtual void DebugDraw() const;

	bool MouseHover() const;

	void SetOrientation();

	virtual void UpdateDragging(float dt);

	iPoint GetPosition() const;

public:
	bool drag = false;
	iPoint mouse_click_pos = { 0,0 };

};

#endif // __UIElement_H__