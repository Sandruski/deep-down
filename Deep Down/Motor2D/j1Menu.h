#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"

#include "Animation.h"

struct SDL_Texture;
struct UILifeBar;

class j1Menu : public j1Module
{
public:
	j1Menu();

	// Destructor
	virtual ~j1Menu();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

};

#endif // __j1MENU_H__