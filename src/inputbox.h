#ifndef INPUTBOX_H
#define INPUTBOX_H

#include "common.h"
#include "raylib.h"
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

class Inputbox
{
private:
    char text[16] = "1e-9";
    const char* name;
    f32 value;
    bool valid;
    bool editing;
    Rectangle rect_text;
    Rectangle rect_label;
    bool should_be_positive;
    Vector2 offset;
    int fontsize = 11;
    void validate_text();
    
public:
    const f32 ratio;
    const Vector2 size;

    Inputbox(const char* name, Vector2 offset, Vector2 size,
         const char* initial_value, f32 ratio);
    Inputbox(const char* name, Vector2 offset, Vector2 size);

    void update_position(Vector2 new_offset);
    void draw();
    f32 get_value();
    bool is_valid();

    void set_fontize(int new_fontsize) { fontsize = new_fontsize; }
};


#endif