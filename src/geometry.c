#include "ASCIIPCR/geometry.h"

#include <ASCIIPCR/prj_defines.h>

#include <math.h>

float a_X, a_Y, a_Z; // - alpha X/Y/Z (radians)
float distance_to_camera = 40.0f; // distance from camera to cube


float rotateX(float i, float j, float k) {
    return j*sin(a_X)*sin(a_Y)*cos(a_Z) - k*cos(a_X)*sin(a_Y)*cos(a_Z) + j*cos(a_X)*sin(a_Z) + k*sin(a_X)*sin(a_Z) + i*cos(a_Y)*cos(a_Z);
}

float rotateY(float i, float j, float k) {
    return j*cos(a_X)*cos(a_Z) + k*sin(a_X)*cos(a_Z) - j*sin(a_X)*sin(a_Y)*sin(a_Z) + k*cos(a_X)*sin(a_Y)*sin(a_Z) - i*cos(a_Y)*sin(a_Z);
}

float rotateZ(float i, float j, float k) {
    return k*cos(a_X)*cos(a_Y) - j*sin(a_X)*cos(a_Y) + i*sin(a_Y);
}

int project_point(float px, float py, float pz, char *ps, float scale) {
    float x = rotateX(px, py, pz);
    float y = rotateY(px, py, pz);
    float z = rotateZ(px, py, pz);

    // oox, ooy, ooz - screen coordinates
    float ooz = 1.0f / (z + distance_to_camera);
    float oox = x * ooz * scale + (TERMINAL_WIDTH / 2.0f);
    float ooy = y * ooz * (scale / 2.0f) + (TERMINAL_HEIGHT / 2.0f);

    if (z <= -5)     *ps = '#';
    else if (z <= 5) *ps = '*';
    else             *ps = '.';

    if (((int)oox >= 0 && (int)oox < TERMINAL_WIDTH) && ((int)ooy >= 0 && (int)ooy < TERMINAL_HEIGHT))   {
        return (int)ooy * TERMINAL_WIDTH + (int)oox; // return index in screen_buffer for renderer
    }

    return -1;
}

void rotate_radians(float v) {
    a_X += v;
    a_Y += v;
    a_Z += v;
}