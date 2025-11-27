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
    //Grid
    InputHolder gridInput("Grid", Vector2 {0.1, 0.1}, screenSize);
    {
        gridInput.set_button_index(0, 0);

        Inputbox* X_inputbox = new Inputbox("X [m]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height), "10e-6", 0.5);
        Inputbox* dx_inputbox = new Inputbox("dx [m]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height), "50e-9", 0.5);
        Inputbox* T_inputbox = new Inputbox("T [s]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height), "500e-15", 0.5);
        Inputbox* cour_inputbox = new Inputbox("cour [arb.]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height), "2", 0.5);
        
        gridInput.add_inputbox(X_inputbox);
        gridInput.add_inputbox(dx_inputbox);
        gridInput.add_inputbox(T_inputbox);
        gridInput.add_inputbox(cour_inputbox);

        auto check_1 = [X_inputbox, dx_inputbox]() 
            { return X_inputbox->get_value() > dx_inputbox->get_value(); };
            
        auto check_2 = [cour_inputbox]() 
            { return ceilf(cour_inputbox->get_value()) == cour_inputbox->get_value(); };
        
        auto check_3 = [cour_inputbox]() 
            { return ceilf(cour_inputbox->get_value()) > 0; };
        
        gridInput.add_validation_check(check_1);
        gridInput.add_validation_check(check_2);
        gridInput.add_validation_check(check_3);
    }

    //Source
    InputDropDown sourceDropDown("Sources", Vector2 {0.1, 0.1}, screenSize);
    sourceDropDown.set_button_index(1, 0);
    
    auto source_setup = [this](u32 subbutton_index_x, u32 subbutton_index_y) -> InputHolder*
    {
        InputHolder *sourceInput = new InputHolder("Source", Vector2 {0.1, 0.1}, Vector2(800, 600));
        sourceInput->set_button_index(subbutton_index_x, subbutton_index_y);
        Inputbox* duration_inputbox = new Inputbox("FWHM [s]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height), "100e-15", 0.5);
        Inputbox* power_inputbox = new Inputbox("P [GW/cm2]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height), "10", 0.5);
        Inputbox* wl_inputbox = new Inputbox("Wavelength [m]", to_screenspace(0.1, 0.1), to_screenspace(input_width, input_height), "1300e-9", 0.5);
        
        sourceInput->add_inputbox(duration_inputbox);
        sourceInput->add_inputbox(power_inputbox);
        sourceInput->add_inputbox(wl_inputbox);

        auto check_1 = [duration_inputbox]() 
            { return duration_inputbox->get_value() > 0; };
            
        auto check_2 = [power_inputbox]() 
            { return ceilf(power_inputbox->get_value()) > 0; };
        
        auto check_3 = [wl_inputbox]() 
            { return ceilf(wl_inputbox->get_value()) > 0; };
        
        sourceInput->add_validation_check(check_1);
        sourceInput->add_validation_check(check_2);
        sourceInput->add_validation_check(check_3);

        return sourceInput;
    };

    sourceDropDown.set_generating_function(source_setup);

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
        
        gridInput.draw();
        sourceDropDown.draw();

    draw();

    EndDrawing();
}
CloseWindow();        // Close window and OpenGL context
}
