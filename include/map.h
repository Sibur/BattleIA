#ifndef MAP_H
#define MAP_H

#include "battle_c.h"

#define MAX_X 100
#define MAX_Y 100

typedef enum {
    EMPTY,
    OBSTACLE,
    RESOURCE,
    ENEMY
} CellType;

void init_map(CellType map[MAX_X][MAX_Y], int width, int height);
void update_map(CellType map[MAX_X][MAX_Y], BC_List *objects);
void print_map(CellType map[MAX_X][MAX_Y], int width, int height);

#endif // MAP_H
