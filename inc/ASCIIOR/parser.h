#ifndef PARSER_H
#define PARSER_H

#include <ASCIIOR/geometry.h>
#include <stdio.h>

int parse_vertices(FILE *file, Point **points_out);

int parse_edges(FILE *file, Edge **edges_out);

#endif // PARSER_H
