#include "inputdropdown.h"

InputDropDown::InputDropDown(const char* name, Vector2 position, Vector2 screenSize)
    : name(name), position(position), screenSize(screenSize)
{

}

InputDropDown::~InputDropDown()
{
    for (auto holder : inputholders)
    {
        delete holder;
    }
}


void InputDropDown::draw()
{
    GuiToggle(
        Rectangle{button_rect.x + button_rect.width * button_index_x,
             button_rect.y + button_rect.height * button_index_y, button_rect.width, 
             button_rect.height}, name, &show);

    if (!show)
    {
        return;
    }

    int pressed = GuiButton(
        Rectangle{button_rect.x + button_rect.width * button_index_x, 
            button_rect.y + button_rect.height * (inputholders.size() + 1), button_rect.width, button_rect.height}
            , "Add");

    if (pressed == 1)
    {
        InputHolder* new_holder = func(button_index_x, inputholders.size() + 1);
        new_holder->hide_window();
        new_holder->set_side_indicator();
        inputholders.push_back(new_holder);
    }

    for (auto inputholder : inputholders)
    {
        inputholder->draw();
    }

    // if (is_valid())
    // {
    //     DrawRectangle(windowrect.x - 2, windowrect.y - 2, windowrect.width + 3, windowrect.height + 3, GREEN);
    // }
    // else
    // {
    //     DrawRectangle(windowrect.x - 2, windowrect.y - 2, windowrect.width + 3, windowrect.height + 3, RED);
    // }

    // int close_result = GuiWindowBox(windowrect, name);
    // if (close_result == 1)
    // {
    //     show = false;
    // }

    // Vector2 mouse = GetMousePosition();

    // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, windowrect)) {
    //     dragging = true;
    //     dragOffset.x = mouse.x - windowrect.x;
    //     dragOffset.y = mouse.y - windowrect.y;
    // }

    // if (dragging)
    // {
    //     if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    //     {
    //         windowrect.x = mouse.x - dragOffset.x;
    //         windowrect.y = mouse.y - dragOffset.y;
    //         u64 i = 0;
    //         for (auto inputbox : inputboxes)
    //         {
    //             inputbox->update_position(
    //                 Vector2{ windowrect.x + (padding/2) * screenSize.x, 
    //         windowrect.y + (padding + spacing * (i)) * screenSize.y});
    //             i = i + 1;
    //         }
    //     }
    //     else
    //     {
    //         dragging = false;
    //     }
    // }

    // for (auto inputbox : inputboxes)
    // {
    //     inputbox->draw();
    // }
}
