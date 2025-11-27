#include "inputholder.h"

InputHolder::InputHolder(string name, Vector2 position, Vector2 screenSize)
    : name(name), position(position), screenSize(screenSize)
{
    windowrect = Rectangle{position.x * screenSize.x, position.y * screenSize.y, 
        (width + 2 * padding) * screenSize.x, 
        ((spacing)*inputboxes.size() + 2 * padding + height) * screenSize.y};
}

InputHolder::~InputHolder()
{
    for (u32 i = 0; i < inputboxes.size(); i++)
    {
        delete inputboxes[i];
    }
}

void InputHolder::add_inputbox(Inputbox* inputbox)
{
    inputboxes.push_back(inputbox);
    inputbox->update_position(
        Vector2{ (position.x + padding / 2) * screenSize.x, 
            (position.y + padding + spacing * (inputboxes.size() - 1)) * screenSize.y});
    windowrect = Rectangle{position.x * screenSize.x, position.y * screenSize.y, (width + 2 * padding) * screenSize.x
        , ((spacing)*inputboxes.size() + 2 * padding + height) * screenSize.y};
}

void InputHolder::add_validation_check(std::function<bool()> validation_check)
{
    validation_checks.push_back(validation_check);
}

bool InputHolder::is_valid()
{
    valid = true;
    for (auto inputbox : inputboxes)
    {
        if (inputbox->is_valid() == false)
        {
            valid = false;
        }
    }

    for (auto validation_check : validation_checks)
    {
        valid = valid && validation_check();
    }

    return valid;
}

void InputHolder::draw()
{
    if (name_override != NULL)
    {
        name = *name_override;
    }
    
    if (side_indicator)
    {
        if (is_valid())
        {
            DrawRectangle(button_rect.x + button_rect.width * button_index_x, 
                button_rect.y + button_rect.height * button_index_y, 
                button_rect.width + 3, button_rect.height, GREEN);
        }
        else
        {
            DrawRectangle(button_rect.x + button_rect.width * button_index_x, 
                button_rect.y + button_rect.height * button_index_y, 
                button_rect.width + 3, button_rect.height, RED);
        }
    }
    else
    {
        if (is_valid())
        {
            DrawRectangle(button_rect.x + button_rect.width * button_index_x, 
                button_rect.y + button_rect.height * button_index_y, 
                button_rect.width, button_rect.height + 3, GREEN);
        }
        else
        {
            DrawRectangle(button_rect.x + button_rect.width * button_index_x, 
                button_rect.y + button_rect.height * button_index_y, 
                button_rect.width, button_rect.height + 3, RED);
        }
    }
    GuiToggle(
        Rectangle{button_rect.x + button_rect.width * button_index_x, 
            button_rect.y + button_rect.height * button_index_y, 
            button_rect.width, button_rect.height}
        , name.c_str(), &show);


    if (!show)
    {
        return;
    }

    if (is_valid())
    {
        DrawRectangle(windowrect.x - 2, windowrect.y - 2, windowrect.width + 3, windowrect.height + 3, GREEN);
    }
    else
    {
        DrawRectangle(windowrect.x - 2, windowrect.y - 2, windowrect.width + 3, windowrect.height + 3, RED);
    }

    int close_result = GuiWindowBox(windowrect, name.c_str());
    if (close_result == 1)
    {
        show = false;
    }

    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, windowrect)) 
    {
        dragging = true;
        dragOffset.x = mouse.x - windowrect.x;
        dragOffset.y = mouse.y - windowrect.y;
    }

    if (dragging)
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            windowrect.x = mouse.x - dragOffset.x;
            windowrect.y = mouse.y - dragOffset.y;
            u64 i = 0;
            for (auto inputbox : inputboxes)
            {
                inputbox->update_position(
                    Vector2{ windowrect.x + (padding/2) * screenSize.x, 
            windowrect.y + (padding + spacing * (i)) * screenSize.y});
                i = i + 1;
            }
        }
        else
        {
            dragging = false;
        }
    }

    if (delete_button_enabled)
    {
        Rectangle delete_button_rect = Rectangle{
            windowrect.x + windowrect.width - 105,
            windowrect.y + 3,
            80,
            20
        };

        int result = GuiButton(delete_button_rect, "Delete");
        if (result == 1)
        {
            marked_for_deletion = true;
        }
    }

    for (auto inputbox : inputboxes)
    {
        inputbox->draw();
    }
}
