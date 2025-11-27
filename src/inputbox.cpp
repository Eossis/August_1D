#include "inputbox.h"

void Inputbox::validate_text()
{
    char buffer[17] = { 0 };
    strncpy(buffer, text, 16);

    std::string str(buffer);
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) 
    {
        valid = false;
        return;
    }
    str = str.substr(start, end - start + 1);

    std::stringstream ss(str);
    ss >> value;

    if (!ss.fail() && ss.eof()) 
    {
        valid = true;
    }
}

Inputbox::Inputbox(const char* name, Vector2 offset, Vector2 size)
    :
    name(name),
    offset(offset),
    size(size),
    ratio(0.5f)
{
    rect_label = Rectangle(offset.x, offset.y, size.x * (1 - ratio), size.y);
    rect_text = Rectangle(offset.x + (1 - ratio) * size.x, offset.y, size.x * ratio, size.y);
}

Inputbox::Inputbox(const char* name, Vector2 offset, 
    Vector2 size, const char* initial_value = "", f32 ratio = 0.5)
    :
    name(name),
    offset(offset),
    size(size),
    ratio(ratio)
{
    strcpy(text, initial_value);
    rect_label = Rectangle(offset.x, offset.y, size.x * (1 - ratio), size.y);
    rect_text = Rectangle(offset.x + (1 - ratio) * size.x, offset.y, size.x * ratio, size.y);
}

void Inputbox::update_position(Vector2 new_offset)
{
    offset = new_offset;
    rect_label = Rectangle(offset.x, offset.y, size.x * (1 - ratio), size.y);
    rect_text = Rectangle(offset.x + (1 - ratio) * size.x, offset.y, size.x * ratio, size.y);
}

void Inputbox::draw()
{
    DrawText(name, rect_label.x, rect_label.y + rect_label.height / 4, fontsize, DARKGRAY);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        editing = CheckCollisionPointRec(mouse, rect_text);
    }
    GuiTextBox(rect_text, text, fontsize, editing);
}

f32 Inputbox::get_value() 
{ 
    validate_text();
    if (!valid) 
    {
        return 0;
        // throw std::runtime_error("Tried to retrieve invalid input in Inputbox"); 
    }
    return value; 
}

bool Inputbox::is_valid()
{ 
    validate_text();
    return valid; 
}