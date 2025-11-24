#include "inputholder.h"

InputHolder::InputHolder(const char* name, Vector2 position, Vector2 screenSize)
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
        Vector2{ (position.x) * screenSize.x, 
            (position.y) * screenSize.y});
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
        valid = validation_check();
    }

    return valid;
}

void InputHolder::draw()
{
    if (!show)
    {
        return;
    }

    GuiWindowBox(windowrect, name);
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, windowrect)) {
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
            for (auto inputbox : inputboxes)
            {
                inputbox->update_position(Vector2{ windowrect.x, 
            windowrect.y});
            }
        }
        else
        {
            dragging = false;
        }
    }

    for (auto inputbox : inputboxes)
    {
        inputbox->draw();
    }
}
