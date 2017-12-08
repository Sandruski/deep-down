#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UIElement.h"
#include "p2nTree.h"

#include <map>

#define CURSOR_WIDTH 2

using namespace std;

struct _TTF_Font;

enum Tex_Names {
	NO_TEX_,
	ATLAS_,
	WOW_LOGO_,
	ESRB_,
	BLIZZARD_LOGO_,
	BUT_,
	CHECKBOX_,
	DIALOGBOX_,
	CURSOR_,
	DSUI_,
	MAX_TEXS_
};

enum Font_Names {
	DEFAULT_,
	ARIALN_,
	MORPHEUS12_, MORPHEUS18_, MORPHEUS100_,
	SKURRI_,
	FRIZQT_,
	MAX_FONTS_
};

struct UIImage_Info;
struct UILabel_Info;
struct UIButton_Info;
struct UICursor_Info;
struct UIWindow_Info;
struct UILifeBar_Info;
struct UIImage;
struct UILabel;
struct UIButton;
struct UICursor;
struct UIWindow;
struct UILifeBar;

// ---------------------------------------------------

class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Blit(float dt) const;

	// TODO 2: Create the factory methods
	// Gui creation functions
	UIImage* CreateUIImage(iPoint pos, UIImage_Info& info, j1Module* listener = nullptr, UIElement* parent = nullptr);
	UILabel* CreateUILabel(iPoint pos, UILabel_Info& info, j1Module* listener = nullptr, UIElement* parent = nullptr);
	UIButton* CreateUIButton(iPoint pos, UIButton_Info& info, j1Module* listener = nullptr, UIElement* parent = nullptr);
	UICursor* CreateUICursor(UICursor_Info& info, j1Module* listener = nullptr, UIElement* parent = nullptr);
	UIWindow* CreateUIWindow(iPoint pos, UIWindow_Info& info, j1Module* listener = nullptr, UIElement* parent = nullptr);
	UILifeBar* CreateUILifeBar(iPoint pos, UILifeBar_Info& info, j1Module* listener = nullptr, UIElement* parent = nullptr);

	const SDL_Texture* GetTexture(Tex_Names name);
	_TTF_Font* GetFont(Font_Names name);

	bool DestroyElement(UIElement* elem);
	bool ClearAllUI();
	bool ClearMapTextures();

	void SetUpDraggingChildren(UIElement* elem, bool dragging);
	void SetUpDraggingNode(bool drag);

private:
	p2SString atlas_file_name;

	nTree<UIElement*>* UI_elements_tree;

	p2List<UIElement*> UI_elements_list;

	p2List<UIElement*> to_spawn_UI_elements;
	p2List<UIElement*> to_spawn_UI_parents;

	map<Tex_Names, const SDL_Texture*> map_textures;
	map<Font_Names, _TTF_Font*> map_fonts;

public:
	bool debug_draw = false;

	bool drag_to_true = false;
	bool drag_to_false = false;
	bool update_drag = false;
};

#endif // __j1GUI_H__