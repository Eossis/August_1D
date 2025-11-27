#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#include <stdio.h> 

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"
#define SRC_FOLDER   "src/"
#define TEMP_FOLDER   "temp/"

int get_nfiles(char* path)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) 
    {
        int j = 0;
        while ((dir = readdir(d)) != NULL) 
        {
            j =j + 1;
        }
    closedir(d);
    return j;
    }
    return 0;
}

void get_file(char* filename, const char* path, int i)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) 
    {
        int j = 0;
        while ((dir = readdir(d)) != NULL) 
        {
            if (j == i)
            {
                strcpy(filename, dir->d_name);
                return;
            }
            j =j + 1;
        }
    closedir(d);
    }
}

bool is_filetype(char* filename, const char* ext)
{
    char* point; 
    if((point = strrchr(filename,'.')) != NULL ) 
    {
        if(strcmp(point,".cpp") == 0) 
        {
            return true;
        }
    }
    return false;
}


int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    
    Cmd cmd = {0};
    Procs procs = {0};
    
    char* filename = malloc(sizeof(char) * 261);
    char* temp = malloc(sizeof(char) * 261);
    char* temp2 = malloc(sizeof(char) * 261);
    int nfiles = get_nfiles("./src");

    for (int i = 0; i < nfiles; i++)
    {
        get_file(filename, "./src", i);

        if (is_filetype(filename, ".cpp"))
        {
            cmd_append(&cmd, "c++");
            cmd_append(&cmd, "-c");
            strcpy(temp, SRC_FOLDER);
            strcpy(&(temp[sizeof(SRC_FOLDER) - 1]), filename);
            cmd_append(&cmd, temp,);

            cmd_append(&cmd, "-I./libs/raylib/include");
            cmd_append(&cmd, "-IC:/Program Files/HDF_Group/HDF5/2.0.1/include");

            cmd_append(&cmd, "--no-warnings");
            cmd_append(&cmd, "-std=c++23");
            cmd_append(&cmd, "-O0");
            cmd_append(&cmd, "-g3");

            cmd_append(&cmd, "-o");
            strcpy(temp2, TEMP_FOLDER);
            strcpy(&(temp2[sizeof(TEMP_FOLDER) - 1]), filename);
            temp2[strlen(temp2)-4] = '\0';
            strcat(temp2, ".o");
            cmd_append(&cmd, temp2,);

            if (!cmd_run(&cmd, .async = &procs)) return 1;
        }
    }

    if (!procs_flush(&procs)) return 1;

    cmd_append(&cmd, "c++");
    cmd_append(&cmd, TEMP_FOLDER"*.o",);

    cmd_append(&cmd, "-static");
    cmd_append(&cmd, "-LC:/Program Files/HDF_Group/HDF5/2.0.1/lib");
    cmd_append(&cmd, "-L./libs/raylib/lib");

    cmd_append(&cmd, "-lhdf5_cpp");
    cmd_append(&cmd, "-lhdf5");
    cmd_append(&cmd, "-l:libz.a");
    cmd_append(&cmd, "-lraylib");
    cmd_append(&cmd, "-lwinmm");
    cmd_append(&cmd, "-lgdi32");
    cmd_append(&cmd, "-lopengl32");

    // cmd_append(&cmd, "-Wall", "-Wextra");
    cmd_append(&cmd, "--no-warnings");
    cmd_append(&cmd, "-std=c++23");
    cmd_append(&cmd, "-O0");
    cmd_append(&cmd, "-g3");
    cmd_append(&cmd, "-o", BUILD_FOLDER"August");

    if (!cmd_run(&cmd)) return 1;

    // cmd_append(&cmd, "build\\August");

    // if (!cmd_run(&cmd)) return 1;

    return 0;
}