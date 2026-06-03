#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
    #include <windows.h>

    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING (0x0004)
#else
    #include <unistd.h>
#endif

/*
Code Errors:
23 - failed to get STD_HANDLE (Windows only)
20 - failed to get old console mode (Windows only)
25 - failed to set new console mode (Windows only)
15 - failed to set old console mode (Windows only)
1 - function template return code on error (all) 
*/

#ifdef _WIN32

HANDLE h_std_out; // initializing only on start
DWORD old_console_mode;
DWORD new_console_mode;

// enable ANSI escape
int windows_enable_ansi() {
    h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_std_out == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "failed to get standard output handle!\n");
        return 23;
    }

    // get old console mode (for returning at the end of program)
    if (!GetConsoleMode(h_std_out, &old_console_mode)) {
        fprintf(stderr, "failed to get old console mode!\n");
        return 20;
    }

    // set new console mode
    new_console_mode = old_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING; // add virtual_terminal_processing to old mode
    if (!SetConsoleMode(h_std_out,  new_console_mode)) {
        fprintf(stderr, "failed to set new console mode!\n");
        return 25;
    }

    return 0;
}

int windows_disable_ansi() {
    if (!SetConsoleMode(h_std_out, old_console_mode)) {
        fprintf(stderr, "failed to restore console mode on exit! Good luck with ANSI-symbols :)\n");
        return 15;
    }

    return 0;
}
#endif

// terminal screen has 80x40 "pixels"
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 40
#define SCREEN_BUFFER_LENGTH (TERMINAL_WIDTH * TERMINAL_HEIGHT)
#define SCALE (40.0f)

float a_X, a_Y, a_Z; // - alpha X/Y/Z (radians)
float distance_to_camera = 40.0f; // distance from camera to cube

typedef struct Point {
    int8_t x, y, z;
} Point;

Point vertices[8] = {
    { 10,  10,  10},
    {-10,  10,  10},
    { 10, -10,  10},
    {-10, -10,  10},
    { 10,  10, -10},
    {-10,  10, -10},
    { 10, -10, -10},
    {-10, -10, -10},
};

float rotateX(float i, float j, float k) {
    return j*sin(a_X)*sin(a_Y)*cos(a_Z) - k*cos(a_X)*sin(a_Y)*cos(a_Z) + j*cos(a_X)*sin(a_Z) + k*sin(a_X)*sin(a_Z) + i*cos(a_Y)*cos(a_Z);
}

float rotateY(float i, float j, float k) {
    return j*cos(a_X)*cos(a_Z) + k*sin(a_X)*cos(a_Z) - j*sin(a_X)*sin(a_Y)*sin(a_Z) + k*cos(a_X)*sin(a_Y)*sin(a_Z) - i*cos(a_Y)*sin(a_Z);
}

float rotateZ(float i, float j, float k) {
    return k*cos(a_X)*cos(a_Y) - j*sin(a_X)*cos(a_Y) + i*sin(a_Y);
}

int project_point(float px, float py, float pz, char *ps) {
    float x = rotateX(px, py, pz);
    float y = rotateY(px, py, pz);
    float z = rotateZ(px, py, pz);

    // oox, ooy, ooz - screen coordinates
    float ooz = 1.0f / (z + distance_to_camera);
    float oox = x * ooz * SCALE + (TERMINAL_WIDTH / 2.0f);
    float ooy = y * ooz * (SCALE / 2.0f) + (TERMINAL_HEIGHT / 2.0f);

    if (z <= -5)     *ps = '#';
    else if (z <= 5) *ps = '*';
    else             *ps = '.';

    if (((int)oox >= 0 && (int)oox < TERMINAL_WIDTH) && ((int)ooy >= 0 && (int)ooy < TERMINAL_HEIGHT))   {
        return (int)ooy * TERMINAL_WIDTH + (int)oox; // return index in screen_buffer for renderer
    }

    return -1;
}

int main(void) {
    // windows console pre-initialization
#ifdef _WIN32
    if (windows_enable_ansi() != 0) {
        fprintf(stderr, "failed to enable ansi in CMD/Powershell!\n");
        return 1;
    }
#endif

    printf("\x1b[H");

    char screen_buffer[SCREEN_BUFFER_LENGTH];
    char return_point_symbol;

    while (1) {
        // clear screen
        memset(screen_buffer, ' ', TERMINAL_WIDTH * TERMINAL_HEIGHT);

        // points
        for (uint8_t i = 0; i < 8; i++) {
            int idx = project_point(vertices[i].x, vertices[i].y, vertices[i].z, &return_point_symbol);
            if (idx != -1) {
                screen_buffer[idx] = return_point_symbol;
            }
        }

        // cube renderer
        printf("\x1b[H");

        for (uint16_t index = 0; index < TERMINAL_WIDTH * TERMINAL_HEIGHT; index++) {
            printf("%c", screen_buffer[index]);

            if ((index + 1) % TERMINAL_WIDTH == 0) // end of line
                printf("\n");
        }
        
        a_X += 0.05;
        a_Y += 0.05;
        a_Z += 0.05;

        #ifdef _WIN32
            Sleep(30);
        #else
            usleep(30000);
        #endif
    }

#ifdef _WIN32
    if(windows_disable_ansi() != 0) {
        fprintf(stderr, "failed to disable ansi in CMD/Powershell!\n");
        return 1;
    }
#endif

    return 0;
}