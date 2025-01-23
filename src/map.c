#include "map.h"
#include <stdio.h>

// Initialiser la carte
void init_map(CellType map[MAX_X][MAX_Y], int width, int height) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            map[x][y] = EMPTY;
        }
    }
}

// Mettre à jour la carte en fonction des données radar
void update_map(CellType map[MAX_X][MAX_Y], BC_List *objects) {
    while (objects) {
        BC_MapObject *object = (BC_MapObject *)bc_ll_value(objects);

        int x = object->position.x;
        int y = object->position.y;

        // Exemple : Détection d'obstacles, ennemis, ou ressources
        switch (object->type) {
            case 1:
                map[x][y] = OBSTACLE;
                break;
            case 2:
                map[x][y] = RESOURCE;
                break;
            case 3:
                map[x][y] = ENEMY;
                break;
            default:
                break;
        }

        objects = bc_ll_next(objects);
    }
}

// Afficher la carte pour le débogage
void print_map(CellType map[MAX_X][MAX_Y], int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (map[x][y]) {
                case EMPTY:
                    printf(".");
                    break;
                case OBSTACLE:
                    printf("#");
                    break;
                case RESOURCE:
                    printf("R");
                    break;
                case ENEMY:
                    printf("E");
                    break;
            }
        }
        printf("\n");
    }
}
