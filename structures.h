#ifndef STRUCTURES_H
#define STRUCTURES_H

#define LONGUEUR_MAX_NOM 50
#define LONGUEUR_MAX_EPREUVE 10
#define LONGUEUR_MAX_DATE 11
#define LONGUEUR_NOM_FICHIER 100
#define LONGUEUR_MAX_LIGNE 200
#define MAX_ENTREES 100
#define MAX_ATHLETES 100

typedef struct {
    char date[LONGUEUR_MAX_DATE];
    char epreuve[LONGUEUR_MAX_EPREUVE];
    int temps_h;
    int temps_m;
    int temps_s;
    int temps_ms;
    int position_relais;
} Entrainement;

typedef struct {
    char prenom[LONGUEUR_MAX_NOM];
    char nom[LONGUEUR_MAX_NOM];
    int nb_entrainements;
    Entrainement entrainement[MAX_ENTREES];
    double temps_moyen;
} Sportif;

typedef struct {
    char epreuve[LONGUEUR_MAX_EPREUVE];
    double meilleur_temps;
    double pire_temps;
    double total_temps;
    int nombre_courses;
} Statistiques;

#endif
