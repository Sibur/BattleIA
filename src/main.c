#include "battle_c.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    BC_Connection *conn = bc_connect("5.135.136.236", 8080);
    if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }

    // Obtenir les dimensions de la carte
    BC_WorldInfo world_info = bc_get_world_info(conn);
    int width = world_info.map_x;
    int height = world_info.map_y;

    // Initialiser la carte
    CellType map[MAX_X][MAX_Y];
    init_map(map, width, height);

    // Ping radar et mise à jour de la carte
    BC_List *list = bc_radar_ping(conn);
    update_map(map, list);

    // Afficher la carte
    print_map(map, width, height);

    return EXIT_SUCCESS;
}

