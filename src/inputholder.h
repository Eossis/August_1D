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
    const char *name;
    Vector2 position;

    f32 padding = 0.05;
    f32 spacing = 0.04;
    f32 width = 0.14;
    f32 height = 0.035;
    bool valid = true;
    bool show = true;
    bool dragging = false;
    Vector2 dragOffset = { 0, 0 };
    Rectangle windowrect;
    Vector2 screenSize;

public:
    InputHolder(const char* name, Vector2 position, Vector2 screenSize);
    ~InputHolder();

    void add_inputbox(Inputbox* inputbox);
    void add_validation_check(std::function<bool()> validation_check);
    bool is_valid();
    void draw();

    bool* get_show() { return &show; }

};

#endif