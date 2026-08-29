#include "ASCIIOR/parser.h"
#include "ASCIIOR/geometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_vertices(FILE *file, Point **points_out) {
    if (!file) return -1;

    rewind(file);

    char str[128];
    int v_count = 0;

    while (fgets(str, sizeof(str), file)) {
        if (str[0] == 'v' && str[1] == ' ') v_count++;
    }

    if (v_count == 0) {
        fclose(file);
        return 0;
    }

    Point *vertices = malloc(v_count * sizeof(Point));
    if (!vertices) return -1;
    rewind(file);

    int i = 0;
    while (fgets(str, sizeof(str), file)) {
        if (str[0] == 'v' && str[1] == ' ') {
            sscanf(str, "v %f %f %f", &vertices[i].x, &vertices[i].y, &vertices[i].z);
            i++;
        }
    }

    fclose(file);
    *points_out = vertices;
    return v_count;
}

int parse_edges(FILE *file, Edge **edges_out) {
    if (!file) return -1;

    rewind(file);

    char str[128];
    int e_count = 0;

    while (fgets(str, sizeof(str), file)) {
        if (str[0] == 'f' && str[1] == ' ') {
            char *ptr = str + 2;
            while (*ptr != '\0' && *ptr != '\n') {
                while (*ptr == ' ') ptr++;
                if (*ptr != '\0' && *ptr != '\n') {
                    e_count++;
                    while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n') ptr++;
                }
            }
        }
    }

    if (e_count == 0) {
        fclose(file);
        return 0;
    }

    Edge *edges = malloc(e_count * sizeof(Edge));
    if (!edges) return -1;

    rewind(file);

    int e_idx = 0;
    while (fgets(str, sizeof(str), file)) {
        if (str[0] == 'f' && str[1] == ' ') {
            int face_v[16];
            int v_in_face = 0;
            char *ptr = str + 2;

            while (*ptr != '\0' && *ptr != '\n' && v_in_face < 16) {
                while (*ptr == ' ') ptr++;
                if (*ptr == '\0' || *ptr == '\n') break;

                face_v[v_in_face++] = strtol(ptr, &ptr, 10) - 1; // .obj index to C index
                while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n') ptr++;
            }

            // link vertices
            for (int i = 0; i < v_in_face; i++) {
                edges[e_idx].v1 = face_v[i];
                edges[e_idx].v2 = face_v[(i + 1) % v_in_face];
                e_idx++;
            }
        }
    }

    fclose(file);
    *edges_out = edges;
    return e_count;
}
