#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <thread>
#include "common.h"
#include "supervisor.h"
#include "gui.h"

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		Supervisor supervisor("simulation.h5", true);
		supervisor.run();
	}
	else
	{
		GUI gui;
		gui.run();
	}

	return 0;
}