#include "radar.h"
#include "tir.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "deplacement.h"
#include <pthread.h>

int main(int argc, char *argv[])
{
    // Établissement de la connexion avec le serveur
    BC_Connection *conn = bc_connect("5.135.136.236", 8080);
    printf("Connexion réussie\n");
    fflush(stdout);

    // Récupération des informations de la carte
    BC_WorldInfo info_monde = bc_get_world_info(conn);
    printf("Dimensions de l'arène\nX: %ld, Y: %ld\n", info_monde.map_x, info_monde.map_y);
    fflush(stdout);

    // Initialisation des données du joueur
    BC_PlayerData data = bc_get_player_data(conn);
    fflush(stdout);

    // Initialisation des listes pour le radar
    Radar *joueurs = NULL;    // Liste des joueurs détectés
    Radar *murs = NULL;       // Liste des murs détectés
    Radar *boosts = NULL;     // Liste des boosts détectés

    // Initialisation des données du bot
    int health = 100;
    int score = 0;

    // Initialisation de la vitesse du joueur
    float speed = 20.0;

    while (!data.is_dead)
    {
        fflush(stdout);

        // Mise à jour des listes d'objets détectés par le radar
        scan(conn, info_monde, data, &joueurs, &murs, &boosts);

        // Gestion du déplacement vers le boost le plus proche
        BC_MapObject boost_proche = proximite(boosts, data);
        if (boost_proche.position.x < 1 && boost_proche.position.y < 1) // Vérification de l'existence d'un boost
        {
            fflush(stdout);
            // Déplacement aléatoire si aucun boost n'est trouvé
            deplacement(conn, rand() % info_monde.map_x, rand() % info_monde.map_y, data, &speed);
        }
        else
        {
            fflush(stdout);
            deplacement(conn, boost_proche.position.x, boost_proche.position.y, data, &speed);
        }

        // Gestion du tir sur les joueurs
        if (joueurs == NULL)
        {
            fflush(stdout);
        }
        else
        {
            BC_MapObject joueur_proche = proximite(joueurs, data);
            fflush(stdout);
            // Tir dirigé vers le joueur
            Tir(conn, data.position.x, joueur_proche.position.x, data.position.y, joueur_proche.position.y);
        }
        
        BC_PlayerData bot = bc_get_player_data(conn);
        if (bot.health < health) {
            printf("Vie : %d\n", bot.health);
            health = bot.health;
        }
        if (bot.score > score) {
            printf("score : %d\n", bot.score);
            score = bot.score;
        }
    }

    // Nettoyage mémoire et déconnexion
    free_list(joueurs);
    free_list(murs);
    free_list(boosts);

    bc_disconnect(conn);

    printf("Fin du script\n");
    return EXIT_SUCCESS;
}