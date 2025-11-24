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

// inline Rectangle get_rect_screenspace(f32 x0, f32 y0, f32 x1, f32 y1)
// {
//     x0 = round(screenWidth * x0);
//     x1 = round(screenWidth * x1);
//     y0 = round(screenHeight * y0);
//     y1 = round(screenHeight * y1);
//     return Rectangle(x0, y0, x1, y1);
// }


// class Grid_handler //rewrite this to be easier to use externally - e.g., you add input boxes 
// {
// private:
//     f32 padding = 0.05;
//     f32 spacing = 0.04;
//     f32 y0 = 0.1;
//     f32 x0 = 0.01;
//     f32 width = 0.14;
//     f32 height = 0.035;
//     vector<Inputbox*> inputboxes;
//     bool valid = true;
//     bool show = true;

//     bool dragging = false;
//     Vector2 dragOffset = { 0, 0 };
//     Rectangle windowrect;

// public:
//     Grid_handler()
//     {
//         inputboxes.push_back(
//             new Inputbox("X [m]", "1e-6", get_rect_screenspace(x0 + padding, y0 + padding, width, height), true));

//         inputboxes.push_back(
//             new Inputbox("dx [m]", "20e-9", get_rect_screenspace(x0 + padding, y0 + spacing + padding, width, height), true));

//         inputboxes.push_back(
//             new Inputbox("T [s]", "300e-15", get_rect_screenspace(x0 + padding, y0 + spacing * 2 + padding, width, height), true));

//         inputboxes.push_back(
//             new Inputbox("cour", "2", get_rect_screenspace(x0 + padding, y0 + spacing * 3 + padding, width, height), true));
        
//         windowrect = get_rect_screenspace(x0, y0, width + 2 * padding, (spacing)*inputboxes.size() + 2 * padding);
//     }

//     ~Grid_handler()
//     {
//         for (u32 i = 0; i < inputboxes.size(); i++)
//         {
//             delete inputboxes[i];
//         }
//     }

//     void draw()
//     {
//         if (!show)
//         {
//             return;
//         }

//         GuiWindowBox(windowrect, "Grid settings");
//         Vector2 mouse = GetMousePosition();

//         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, windowrect)) {
//             dragging = true;
//             dragOffset.x = mouse.x - windowrect.x;
//             dragOffset.y = mouse.y - windowrect.y;
//         }

//         if (dragging) 
//         {
//             if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
//             {
//                 windowrect.x = mouse.x - dragOffset.x;
//                 windowrect.y = mouse.y - dragOffset.y;
//                 for (auto inputbox : inputboxes)
//                 {
//                     inputbox->update_position(windowrect.x - padding * screenWidth
//                         , windowrect.y - 2 * padding * screenHeight);
//                 }
//             }
//             else 
//             {
//                 dragging = false;
//             }
//         }

//         for (auto inputbox : inputboxes)
//         {
//             inputbox->draw();
//         }
//     }

//     bool is_valid()
//     {
//         valid = true;
//         //first stage check
//         for (auto inputbox : inputboxes)
//         {
//             if (inputbox->is_valid() == false)
//             {
//                 valid = false;
//             }
//         }

//         //second stage check
//         if (valid)
//         {
//             for (auto inputbox : inputboxes)
//             {
//                 if (inputbox->get_value() == 0)
//                 {
//                     valid = false;
//                 }
//             }
//             if (inputboxes[0]->get_value() < inputboxes[1]->get_value())
//             {
//                 valid = false;
//             }
//         }
//         return valid;
//     }

//     Grid get_grid()
//     {
//         if (!valid) { throw; }
//         return Grid(inputboxes[0]->get_value(),
//             inputboxes[1]->get_value(),
//             inputboxes[2]->get_value(),
//             inputboxes[3]->get_value());
//     }

//     bool* get_show() { return &show; }
// };

class GUI
{
private:
    Vector2 mousePosition;
    Vector2 windowPosition;
    Vector2 panOffset;
    bool dragWindow = false;
    bool exitWindow = false;
    const Vector2 screenSize = Vector2(800, 600);
    
    Vector2 to_screenspace(f32 x, f32 y);
public:
    GUI();
    ~GUI();
    void run();
    void draw();
};

#endif

