#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "UIImage.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UICursor.h"
#include "UIWindow.h"
#include "UILifeBar.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	bool ret = true;

	// Load textures
	map_textures[Tex_Names::ATLAS_] = App->tex->Load(atlas_file_name.GetString());
	map_textures[Tex_Names::CURSORTEX_] = App->tex->Load("textures/wow/Point.png");
	map_textures[Tex_Names::CHECKBOX_] = App->tex->Load("Assets/Sprites/UI/CheckBox.png");
	map_textures[Tex_Names::DSUI_] = App->tex->Load("Assets/Sprites/UI/DSUI.png");
	map_textures[Tex_Names::MENU_] = App->tex->Load("Assets/Sprites/UI/MenuPause.png");
	map_textures[Tex_Names::GENERAL_] = App->tex->Load("Assets/Sprites/Maps/GeneralTileset.png");

	// Load fonts
	map_fonts[Font_Names::MSMINCHO_] = App->font->Load("Assets/Fonts/MSMINCHO.TTF");
	map_fonts[Font_Names::ZELDA_] = App->font->Load("Assets/Fonts/Zelda.ttf", 60);
	map_fonts[Font_Names::SOBAD_] = App->font->Load("Assets/Fonts/So/SOBAD___.ttf");

	UI_elements_tree = new nTree<UIElement*>((UIElement*)App->win->window);

	return ret;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;

	p2List_item<UIElement*>* UI_elem_it = to_spawn_UI_elements.start;
	p2List_item<UIElement*>* UI_parent_it = to_spawn_UI_parents.start;

	while (UI_elem_it != nullptr && UI_parent_it != nullptr) {

		UI_elements_tree->insertChild(UI_elem_it->data, UI_parent_it->data);

		UI_elem_it = UI_elem_it->next;
		UI_parent_it = UI_parent_it->next;
	}

	to_spawn_UI_elements.clear();
	to_spawn_UI_parents.clear();
	UI_elements_list.clear();

	UI_elements_tree->recursivePreOrderList(UI_elements_tree->getRoot(), &UI_elements_list);

	if (drag_to_true) {
		SetUpDraggingNode(true);
		drag_to_true = false;
	}
	else if (drag_to_false) {
		SetUpDraggingNode(false);
		drag_to_false = false;
	}

	return ret;
}

bool j1Gui::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug_draw = !debug_draw;

	p2List_item<UIElement*>* UI_elem_it = UI_elements_list.start;

	while (UI_elem_it != nullptr) {
		UI_elem_it->data->Update(dt);

		if (UI_elem_it->data->drag && update_drag)
			UI_elem_it->data->UpdateDragging(dt);

		UI_elem_it = UI_elem_it->next;
	}

	UI_elem_it = UI_elements_list.start;


	return ret;
}

bool j1Gui::Blit(float dt) const
{
	bool ret = true;

	p2List_item<UIElement*>* UI_elem_it = UI_elements_list.start;

	while (UI_elem_it != nullptr) {
		UI_elem_it->data->Draw();
		UI_elem_it = UI_elem_it->next;
	}

	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;

	p2List_item<UIElement*>* UI_elem_it = UI_elements_list.start;

	while (UI_elem_it != nullptr) {

		if (UI_elem_it->data->HasToBeRemoved()) {
			RELEASE(UI_elem_it->data);
		}

		UI_elem_it = UI_elem_it->next;
	}

	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	bool ret = true;

	if (ClearAllUI())
		LOG("All GUI removed");

	// Remove textures
	if (ClearMapTextures())
		LOG("All GUI textures removed");

	return ret;
}

UIImage* j1Gui::CreateUIImage(iPoint local_pos, UIImage_Info& info, j1Module* listener, UIElement* parent)
{
	UIImage* image = new UIImage(local_pos, parent, info, listener);

	to_spawn_UI_elements.add(image);
	
	if (parent == nullptr)
		parent = (UIElement*)App->win->window;
	to_spawn_UI_parents.add(parent);

	return image;
}

UILabel* j1Gui::CreateUILabel(iPoint local_pos, UILabel_Info& info, j1Module* listener, UIElement* parent)
{
	UILabel* label = new UILabel(local_pos, parent, info, listener);

	to_spawn_UI_elements.add(label);

	if (parent == nullptr)
		parent = (UIElement*)App->win->window;
	to_spawn_UI_parents.add(parent);

	return label;
}

UIButton* j1Gui::CreateUIButton(iPoint local_pos, UIButton_Info& info, j1Module* listener, UIElement* parent)
{
	UIButton* button = new UIButton(local_pos, parent, info, listener);

	to_spawn_UI_elements.add(button);

	if (parent == nullptr)
		parent = (UIElement*)App->win->window;
	to_spawn_UI_parents.add(parent);

	return button;
}

UICursor* j1Gui::CreateUICursor(UICursor_Info& info, j1Module* listener, UIElement* parent)
{
	iPoint local_pos = { 0,0 };

	UICursor* cursor = new UICursor(local_pos, parent, info, listener);

	to_spawn_UI_elements.add(cursor);

	if (parent == nullptr)
		parent = (UIElement*)App->win->window;
	to_spawn_UI_parents.add(parent);

	return cursor;
}

UIWindow* j1Gui::CreateUIWindow(iPoint local_pos, UIWindow_Info& info, j1Module* listener, UIElement* parent)
{
	UIWindow* window = new UIWindow(local_pos, parent, info, listener);

	to_spawn_UI_elements.add(window);

	if (parent == nullptr)
		parent = (UIElement*)App->win->window;
	to_spawn_UI_parents.add(parent);

	return window;
}

UILifeBar* j1Gui::CreateUILifeBar(iPoint local_pos, UILifeBar_Info& info, j1Module* listener, UIElement* parent)
{
	UILifeBar* life_bar = new UILifeBar(local_pos, parent, info, listener);

	to_spawn_UI_elements.add(life_bar);

	if (parent == nullptr)
		parent = (UIElement*)App->win->window;
	to_spawn_UI_parents.add(parent);

	return life_bar;
}

bool j1Gui::DestroyElement(UIElement* elem)
{
	bool ret = false;

	UI_elements_tree->remove(elem);

	return ret;
}

bool j1Gui::ClearAllUI()
{
	bool ret = false;

	// Clear UI_elements list (active elements)
	UI_elements_tree->clear();

	// Clear to_spawn_UI_elements list (elements in queue)
	p2List_item<UIElement*>* UI_elem_it = UI_elements_list.start;

	while (UI_elem_it != nullptr) {
		ret = true;

		RELEASE(UI_elem_it->data);
		UI_elem_it = UI_elem_it->next;
	}
	UI_elements_list.clear();

	return ret;
}

bool j1Gui::ClearMapTextures() 
{
	bool ret = true;

	for (uint i = 0; i < map_textures.size(); ++i)
		App->tex->UnLoad((SDL_Texture*)map_textures[(Tex_Names)i]);

	return ret;
}

const SDL_Texture* j1Gui::GetTexture(Tex_Names name)
{
	return map_textures[name];
}

_TTF_Font* j1Gui::GetFont(Font_Names name)
{
	return map_fonts[name];
}

void j1Gui::SetUpDraggingChildren(UIElement* elem, bool drag)
{
	// List including to_drag element and all of its children
	p2List<UIElement*> to_drag;
	UI_elements_tree->recursivePreOrderList(UI_elements_tree->search(elem), &to_drag);

	// Don't drag elements which are not in the previous list
	p2List_item<UIElement*>* UI_elem_it = to_drag.start;
	UI_elem_it = UI_elements_list.start;
	
	while (UI_elem_it != nullptr && UI_elem_it->data != to_drag.start->data) {
		UI_elem_it->data->drag = false;
		UI_elem_it = UI_elem_it->next;
	}
	
	update_drag = drag;
}

void j1Gui::SetUpDraggingNode(bool drag) 
{
	UIElement* dragging_node = nullptr;
	int lowest_level = UI_elements_tree->getNumLevels(UI_elements_tree->getRoot()) + 1;

	// If several elements are clicked at once, select the element in the lowest level in the tree
	p2List_item<UIElement*>* UI_elem_it = UI_elements_list.start;

	while (UI_elem_it != nullptr) {

		if (UI_elem_it->data->drag == drag) {
			int level = UI_elements_tree->getNumLevels(UI_elements_tree->search(UI_elem_it->data));

			if (level <= lowest_level) {
				dragging_node = UI_elem_it->data;
			}
		}

		UI_elem_it = UI_elem_it->next;
	}

	SetUpDraggingChildren(dragging_node, drag);
}

// class Gui ---------------------------------------------------

