#include "ASCIIOR/parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <ASCIIOR/console.h>
#include <ASCIIOR/geometry.h>
#include <ASCIIOR/prj_defines.h>

/*
Code Errors:
23 - failed to get STD_HANDLE (Windows only)
20 - failed to get old console mode (Windows only)
25 - failed to set new console mode (Windows only)
15 - failed to set old console mode (Windows only)
1 - function template return code on error (all)
30 - failed to read file (all)
*/

#define DEFAULT_SCALE 40.0f

float user_scale = DEFAULT_SCALE;

int main(void) {
    // windows console pre-initialization
#ifdef _WIN32
    if (windows_enable_ansi() != 0) {
        fprintf(stderr, "failed to enable ansi in CMD/Powershell!\n");
        return 1;
    }
#endif

    char path[128];

    printf("Enter file path: ");
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = '\0';

    printf("Enter render scale (recommended 40): ");
    char scale_buf[64];
    if (fgets(scale_buf, sizeof(scale_buf), stdin) != NULL) {
        float parsed_scale = strtof(scale_buf, NULL);
        if (parsed_scale > 0.0f) {
            user_scale = parsed_scale;
        } else {
            user_scale = 40.0f;
        }
    }

    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s!", path);
        return 1;
    }

    Point *vertices;
    Edge *edges;
    int v_count = parse_vertices(file, &vertices);
    int e_count = parse_edges(file, &edges);

    if (v_count <= 0 || vertices == NULL) {
        fprintf(stderr, "failed to parse vertices from file: %s\n", path);
        return 1;
    }
    if (e_count <= 0 || edges == NULL) {
        fprintf(stderr, "failed to parse edges from file: %s\n", path);
    }

    printf("\x1b[H");

    char screen_buffer[SCREEN_BUFFER_LENGTH];
    char return_point_symbol;

    while (1) {
        // clear screen
        memset(screen_buffer, ' ', TERMINAL_WIDTH * TERMINAL_HEIGHT);

        // points
        for (uint32_t i = 0; i < v_count; i++) {
            int idx = project_point(vertices[i].x, vertices[i].y, vertices[i].z, &return_point_symbol, user_scale);
            if (idx != -1) {
                screen_buffer[idx] = return_point_symbol;
            }
        }

        // OBJECT RENDERER

        printf("\x1b[H");

        for (uint16_t index = 0; index < TERMINAL_WIDTH * TERMINAL_HEIGHT; index++) {
            printf("%c", screen_buffer[index]);

            if ((index + 1) % TERMINAL_WIDTH == 0) // eol
                printf("\n");
        }

        rotate_radians(0.05);

        #ifdef _WIN32
            Sleep(30);
        #else
            usleep(30000);
        #endif
    }

    free(vertices);

#ifdef _WIN32
    if(windows_disable_ansi() != 0) {
        fprintf(stderr, "failed to disable ansi in CMD/Powershell!\n");
        return 1;
    }
#endif

    return 0;
}
