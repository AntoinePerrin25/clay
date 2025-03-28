#define NOB_IMPLEMENTATION
#include <nob.h>

#define C99_IMPLEMENTATION 1
#define OPTIMIZATION 0

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    // Création des dossiers nécessaires

    nob_mkdir_if_not_exists("src");
    nob_mkdir_if_not_exists("lib");
    nob_mkdir_if_not_exists("include");


    printf("----------\n");
    // Compilation du client
    {
        // Check if the game is running and kill it before rebuilding
        Nob_Cmd kill_cmd = {0};
        nob_cmd_append(&kill_cmd, "taskkill", "/F", "/IM", "main.exe", "/T");
        // Ignore failure since the process might not be running
        nob_cmd_run_sync(kill_cmd);

        Nob_Cmd cmd = {0};
        
        nob_cmd_append(&cmd, "gcc");
        nob_cmd_append(&cmd, "-Wall", "-Wextra");
        #if C99_IMPLEMENTATION
        nob_cmd_append(&cmd, "-std=c99", "-pedantic", "-march=native");
        nob_cmd_append(&cmd, "-I./include",  "-L./lib");
        #endif
        #if OPTIMIZATION
        nob_cmd_append(&cmd, "-O3", "-fomit-frame-pointer", "-funroll-loops");
        #endif
        nob_cmd_append(&cmd, "./src/main.c");
        nob_cmd_append(&cmd, "-o", "./main");
        nob_cmd_append(&cmd, "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lws2_32");
        // in a cmd : gcc -Wall -Wextra -O3 -I./include -L./lib ./src/main.c -o ./build/game -lraylib -lenet -lopengl32 -lgdi32 -lwinmm -lws2_32
        if (!nob_cmd_run_sync(cmd)) return 1;
    }
    printf("----------\n");


    return 0;
}