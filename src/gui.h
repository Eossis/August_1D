#ifndef GUI_H
#define GUI_H

#include "common.h"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <functional>
#include "raylib.h"
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <thread>
#include <functional>
#include "supervisor.h"
#include "gui.h"
#include "inputbox.h"
#include "inputholder.h"
#include "inputdropdown.h"

class GUI
{
private:
    Vector2 mousePosition;
    Vector2 windowPosition;
    Vector2 panOffset;
    bool dragWindow = false;
    bool exitWindow = false;
    const Vector2 screenSize = Vector2(800, 600);

    const f32 input_width = 0.18;
    const f32 input_height = 0.035;
    
    Vector2 to_screenspace(f32 x, f32 y);
public:
    GUI();
    ~GUI();
    void run();
    void draw();
};

#endif

