#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct Point {
    float x, y, z;
} Point;

typedef struct Edge {
    int v1, v2; // indecies of the vertices in Point array
} Edge;

float rotateX(float i, float j, float k);
float rotateY(float i, float j, float k);
float rotateZ(float i, float j, float k);
int project_point(float px, float py, float pz, char *ps, float scale);
void rotate_radians(float v);

#endif // GEOMETRY_H
