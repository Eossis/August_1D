#include "gui.h"


GUI::~GUI()
{

}

GUI::GUI()
    :
    mousePosition(Vector2(0, 0)),
    windowPosition(Vector2(200, 200)),
    panOffset(Vector2(0, 0))
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenSize.x, screenSize.y, "August");

    SetWindowPosition(windowPosition.x, windowPosition.y);
    SetTargetFPS(60);
}

void GUI::draw()
{
    //toggles for valid components
    // GuiToggle(get_rect_screenspace(0.0, 0.04, 0.1, 0.04), "Grid", grid_handler.get_show());

    // //draw the simulation line and ticks
    // DrawLine((int)round(screenWidth * 0.2), (int)round(screenHeight * 0.5)
    //     , (int)round(screenWidth * 0.8), (int)round(screenHeight * 0.5), BLACK);

    // if (grid_handler.is_valid() != NULL)
    // {
    //     Grid grid = grid_handler.get_grid();
    //     f32 perc = 0;
    //     DrawLine((int)round(screenWidth * (0.2 + perc * 0.6)), (int)round(screenHeight * 0.49)
    //         , (int)round(screenWidth * (0.2 + perc * 0.6)), (int)round(screenHeight * 0.51), BLACK);
    //     for (u64 i = 0; i < grid.Nx; i++)
    //     {
    //         perc = (f32)(i + 1) / (f32)(grid.Nx);
    //         DrawLine((int)round(screenWidth * (0.2 + perc * 0.6)), (int)round(screenHeight * 0.49)
    //             , (int)round(screenWidth * (0.2 + perc * 0.6)), (int)round(screenHeight * 0.51), BLACK);
    //     }
    // }
}

Vector2 GUI::to_screenspace(f32 x, f32 y)
{
    return Vector2(x * screenSize.x, y * screenSize.y);
}

void GUI::run()
{
    // f32 padding = 0.05;
    // f32 spacing = 0.04;
    // f32 y0 = 0.2;
    // f32 x0 = 0.1;
    const f32 input_width = 0.14;
    const f32 input_height = 0.035;

    Inputbox* inputbox = new Inputbox("X [m]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height));
    
    InputHolder inputholder("Grid", Vector2 {0.1, 0.1}, screenSize);
    inputholder.add_inputbox(inputbox);
    // auto i1 = new Inputbox("X [m]", "1e-6", get_rect_screenspace(x0 + padding, y0 + padding, width, height), true);
    // auto i2 = new Inputbox("dx [m]", "20e-9", get_rect_screenspace(x0 + padding, y0 + spacing + padding, width, height), true);
    // auto i3 = new Inputbox("T [s]", "300e-15", get_rect_screenspace(x0 + padding, y0 + spacing * 2 + padding, width, height), true);
    // auto i4 = new Inputbox("cour", "2", get_rect_screenspace(x0 + padding, y0 + spacing * 3 + padding, width, height), true);

    // inputholder.add_inputbox(i1);
    // inputholder.add_inputbox(i2);
    // inputholder.add_inputbox(i3);
    // inputholder.add_inputbox(i4);

    // auto check = [i1, i2]() { return i2->get_value() < i1->get_value(); };
    // inputholder.add_validation_check(check);


    //Main controls - top left
    while (!exitWindow && !WindowShouldClose())
    {
        mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow)
        {
            if (CheckCollisionPointRec(mousePosition, Rectangle(0, 0, screenSize.x, 20)))
            {
                windowPosition = GetWindowPosition();
                dragWindow = true;
                panOffset = mousePosition;
            }
        }

        if (dragWindow)
        {
            windowPosition.x += (mousePosition.x - panOffset.x);
            windowPosition.y += (mousePosition.y - panOffset.y);

            SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragWindow = false;
        }
        BeginDrawing();

        ClearBackground(RAYWHITE);
        exitWindow = GuiWindowBox(Rectangle{ 0, 0, (float)screenSize.x, (float)screenSize.y }, "#198# August 1D");

        inputholder.draw();

    draw();

    EndDrawing();
}
CloseWindow();        // Close window and OpenGL context
}
