#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "c++");
    nob_cmd_append(&cmd, SRC_FOLDER"*.cpp",);
    nob_cmd_append(&cmd, "-I./libs/raylib/include");
    nob_cmd_append(&cmd, "-IC:/Program Files/HDF_Group/HDF5/2.0.1/include");
    nob_cmd_append(&cmd, "-LC:/Program Files/HDF_Group/HDF5/2.0.1/lib");
    nob_cmd_append(&cmd, "-L./libs/raylib/lib");
    nob_cmd_append(&cmd, "-lhdf5");
    nob_cmd_append(&cmd, "-lhdf5_hl");
    nob_cmd_append(&cmd, "-lhdf5_cpp");
    nob_cmd_append(&cmd, "-lraylib");
    nob_cmd_append(&cmd, "-lz");
    nob_cmd_append(&cmd, "-std=c++23");
    // nob_cmd_append(&cmd, "-Wall", "-Wextra");
    nob_cmd_append(&cmd, "--no-warnings");
    nob_cmd_append(&cmd, "-O0");
    nob_cmd_append(&cmd, "-g3");
    nob_cmd_append(&cmd, "-o", BUILD_FOLDER"August");

    if (!nob_cmd_run(&cmd)) return 1;

    nob_cmd_append(&cmd, "build\\August");

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}