# Battle-C bot 

## Installation 

### Pré-requis:
- Docker 
- Meson

### Compilation 

#### Avec Docker
```
docker build -t battle_bot -f Dockerfile .
```

#### Sans Docker
```
meson setup builddir
meson compile -C builddir
```


### Lancement 

#### Avec Docker
``` 
docker run battle_bot
```

#### Sans Docker

``` 
./builddir/battle_c_bot
```


## Scoring

- Toucher un ennemi : 20 points 
- Détruire un ennemi: 50 points
- Récupérer un boost: 30 points



## Fonctionnalités du Bot

### Déplacement Automatique :
``` 
Le bot se dirige automatiquement vers le boost le plus proche pour maximiser ses capacités.
Évitement d'Obstacles :

Si un obstacle est détecté sur le chemin vers le boost, le bot change de direction pour l'éviter.
```

### Détection via le Radar :
``` 
Le bot utilise un radar pour identifier les objets autour de lui, tels que :
```
- Ennemis : Priorité à leur élimination.
- Ressources (boosts) : Pour augmenter ses capacités.
- Obstacles : Pour les contourner.

### Gestion des Actions :
``` 
Le bot décide de ses actions en fonction des données du radar :
```
- Tirer sur un ennemi.
- Changer de direction pour éviter un obstacle.
- Se diriger vers une ressource pour la récupérer.


### Stratégie Dynamique :
``` 
Le bot ajuste constamment sa trajectoire et ses actions en fonction de l'évolution de l'arène.
```

## Fichiers du projet

### Fichiers Sources dans src

- main.c 
- radar.c
- deplacement.c
- tir.c


### Fichiers d'En-tête dans le include 

- deplacement.h 
- main.h 
- radar.h 
- tir.h 