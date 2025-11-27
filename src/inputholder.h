#ifndef INPUTHOLDER_H
#define INPUTHOLDER_H

#include "common.h"
#include "inputbox.h"
#include "raylib.h"
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <functional>

class InputHolder
{
private:
    vector<Inputbox*> inputboxes;
    vector<std::function<bool()>> validation_checks;
    string name;
    Vector2 position;

    f32 padding = 0.05;
    f32 spacing = 0.04;
    f32 width = 0.14;
    f32 height = 0.035;
    bool valid = true;
    bool show = true;
    bool dragging = false;
    bool delete_button_enabled = false;
    bool marked_for_deletion = false;
    Vector2 dragOffset = { 0, 0 };
    Rectangle windowrect;
    Vector2 screenSize;
    Rectangle button_rect = Rectangle {0, screenSize.y * 0.04f, screenSize.x * 0.1f, screenSize.y * 0.04f};
    u32 button_index_x = 0;
    u32 button_index_y = 0;

    bool side_indicator = false;

public:
    string *name_override = NULL;

    InputHolder(string name, Vector2 position, Vector2 screenSize);
    ~InputHolder();

    void add_inputbox(Inputbox* inputbox);
    void add_validation_check(std::function<bool()> validation_check);
    bool is_valid();
    void draw();

    bool* get_show() { return &show; }
    void hide_window() { show = false; }
    void show_window() { show = true; }
    void set_button_index(u32 index_x, u32 index_y) { button_index_x = index_x; button_index_y = index_y; }
    void set_side_indicator() { side_indicator = true; }
    void set_bottom_indicator() { side_indicator = false; }
    void enable_delete_button() { delete_button_enabled = true; }
    void disable_delete_button() { delete_button_enabled = false; }
    bool is_marked_for_deletion() { return marked_for_deletion; }
};

#endif