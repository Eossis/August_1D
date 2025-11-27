#ifndef INPUTDROPDOWN_H
#define INPUTDROPDOWN_H

#include "common.h"
#include "inputbox.h"
#include "inputholder.h"
#include "raylib.h"
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <functional>

class InputDropDown
{
private:
    vector<InputHolder*> inputholders;
    std::function<InputHolder*(u32, u32)> func;

    const char *name;
    Vector2 position;

    f32 padding = 0.05;
    f32 spacing = 0.04;
    f32 width = 0.14;
    f32 height = 0.035;
    bool valid = true;
    bool show = true;
    Vector2 screenSize;
    Rectangle button_rect = Rectangle {0, screenSize.y * 0.04f, screenSize.x * 0.1f, screenSize.y * 0.04f};
    u32 button_index_x = 0;
    u32 button_index_y = 0;

public:
    InputDropDown(const char* name, Vector2 position, Vector2 screenSize);
    ~InputDropDown();

    void draw();

    void set_button_index(u32 index_x, u32 index_y) { button_index_x = index_x; button_index_y = index_y; }
    void set_generating_function(std::function<InputHolder*(u32, u32)> func){this->func = func;}
};

#endif