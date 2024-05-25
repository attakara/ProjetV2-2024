#include "structures.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

void ajouter_donnees_entrainement(Sportif *sportif, Entrainement entrainement) {
    FILE *fichier;
    char nom_fichier[LONGUEUR_NOM_FICHIER];

    // Construire le nom du fichier à partir du prénom et du nom de l'athlète
    snprintf(nom_fichier, sizeof(nom_fichier), "%s%s.txt", sportif->prenom, sportif->nom);

    // Ouvrir le répertoire courant
    DIR *dir;
    struct dirent *ent;
    int relais_count = 0;
    int positions[5] = {0}; // Positions 1 à 4

    if ((dir = opendir(".")) != NULL) {
        // Parcourir tous les fichiers du répertoire
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                // Ouvrir chaque fichier en mode lecture
                fichier = fopen(ent->d_name, "r");
                if (fichier == NULL) {
                    printf("Erreur lors de l'ouverture du fichier %s pour lecture.\n", ent->d_name);
                    perror("fopen");
                    continue;
                }

                // Lire chaque ligne du fichier
                char ligne[LONGUEUR_MAX_LIGNE];
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    Entrainement existant;
                    sscanf(ligne, "%10s %9s %d %d %d %d %d", existant.date, existant.epreuve, &existant.temps_h, &existant.temps_m, &existant.temps_s, &existant.temps_ms, &existant.position_relais);

                    if (strcmp(existant.date, entrainement.date) == 0 && strcmp(existant.epreuve, "relais") == 0) {
                        relais_count++;
                        if (existant.position_relais >= 1 && existant.position_relais <= 4) {
                            positions[existant.position_relais] = 1;
                        }
                    }
                }
                fclose(fichier);

                if (relais_count >= 4) {
                    printf("Erreur: Un maximum de quatre athlètes peut participer à un relais pour la même date (%s).\n", entrainement.date);
                    closedir(dir);
                    return;
                }
            }
        }
        closedir(dir);
    } else {
        printf("Erreur lors de l'ouverture du répertoire courant.\n");
        perror("opendir");
        return;
    }

    // Vérifier si la position est déjà occupée
    if (entrainement.position_relais < 1 || entrainement.position_relais > 4 || positions[entrainement.position_relais] == 1) {
        printf("Erreur: La position %d dans le relais est déjà occupée pour la date %s.\n", entrainement.position_relais, entrainement.date);
        return;
    }

    // Ouvrir le fichier en mode ajout (append)
    fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        // Afficher un message d'erreur si le fichier ne peut pas être ouvert
        printf("Erreur lors de l'ouverture du fichier %s pour écriture.\n", nom_fichier);
        perror("fopen");
        return;
    }

    // Ajouter les données d'entraînement au fichier
    if (strcmp(entrainement.epreuve, "relais") == 0) {
        // Si l'épreuve est un relais, inclure la position dans le relais
        fprintf(fichier, "%s %s %d %d %d %d %d\n", entrainement.date, entrainement.epreuve, 
                entrainement.temps_h, entrainement.temps_m, entrainement.temps_s, 
                entrainement.temps_ms, entrainement.position_relais);
    } else {
        // Sinon, ne pas inclure la position
        fprintf(fichier, "%s %s %d %d %d %d %d\n", entrainement.date, entrainement.epreuve, 
                entrainement.temps_h, entrainement.temps_m, entrainement.temps_s, 
                entrainement.temps_ms, 0);
    }

    // Fermer le fichier
    fclose(fichier);
}


void charger_donnees_entrainement(Sportif *sportif) {
    char nom_fichier[LONGUEUR_NOM_FICHIER];
    snprintf(nom_fichier, sizeof(nom_fichier), "%s%s.txt", sportif->prenom, sportif->nom);

    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s pour lecture.\n", nom_fichier);
        //perror("fopen");
        return;
    }

    // Initialiser le nombre d'entraînements à 0
    sportif->nb_entrainements = 0; // Modification : Initialise le nombre d'entraînements à 0

    char ligne[LONGUEUR_MAX_LIGNE];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Vérifier si le sportif a déjà atteint la limite de 100 entraînements
        if (sportif->nb_entrainements >= MAX_ENTREES) {
            printf("Le sportif a atteint la limite de 100 entraînements.\n");
            break;
        }

        Entrainement entrainement;
        sscanf(ligne, "%10s %9s %d %d %d %d %d", entrainement.date, entrainement.epreuve, &entrainement.temps_h, &entrainement.temps_m, &entrainement.temps_s, &entrainement.temps_ms, &entrainement.position_relais);

        // Ajouter l'entraînement à la liste des entraînements du sportif
        sportif->entrainement[sportif->nb_entrainements++] = entrainement; // Modification : Stocke l'entraînement dans la structure sportif
    }
    fclose(fichier);
}

void afficher_historique_entrainement(Sportif *sportif, char *epreuve, char *date_debut, char *date_fin) {
    charger_donnees_entrainement(sportif);

    printf("Historique d'entraînement pour %s %s:\n\n", sportif->prenom, sportif->nom);

    for (int i = 0; i < sportif->nb_entrainements; i++) {
        Entrainement *entrainement = &sportif->entrainement[i];

        if ((epreuve == NULL || strcmp(epreuve, entrainement->epreuve) == 0) && 
            (date_debut == NULL || strcmp(entrainement->date, date_debut) >= 0) && 
            (date_fin == NULL || strcmp(entrainement->date, date_fin) <= 0)) {

            printf("%s - %s : ", entrainement->date, entrainement->epreuve);

            // Afficher le temps en fonction des valeurs présentes
            if (entrainement->temps_h > 0) {
                printf("%dh ", entrainement->temps_h);
            }
            if (entrainement->temps_h > 0 || entrainement->temps_m > 0) {
                printf("%dm ", entrainement->temps_m);
            }
            if (entrainement->temps_h > 0 || entrainement->temps_m > 0 || entrainement->temps_s > 0) {
                printf("%ds ", entrainement->temps_s);
            }
            printf("%d", entrainement->temps_ms);

            if (entrainement->temps_h == 0 && entrainement->temps_m == 0 && entrainement->temps_s == 0 && entrainement->temps_ms == 0) {
                printf("0s 0");
            }

            if (entrainement->position_relais != 0) {
                printf(" (position %d)", entrainement->position_relais);
            }
            printf("\n\n"); // Ajout d'un saut de ligne après chaque entraînement
        }
    }
}

void afficher_statistiques_performance(Sportif *sportif) {
    charger_donnees_entrainement(sportif);

    Statistiques stats[LONGUEUR_MAX_EPREUVE] = {0};
    int nombre_epreuves = 0;

    for (int i = 0; i < sportif->nb_entrainements; i++) {
        Entrainement *entrainement = &sportif->entrainement[i];
        double temps_total = entrainement->temps_h * 3600 + entrainement->temps_m * 60 + entrainement->temps_s + entrainement->temps_ms / 100.0;

        int j;
        for (j = 0; j < nombre_epreuves; j++) {
            if (strcmp(stats[j].epreuve, entrainement->epreuve) == 0) {
                break;
            }
        }

        if (j == nombre_epreuves) {
            strcpy(stats[nombre_epreuves].epreuve, entrainement->epreuve);
            stats[nombre_epreuves].meilleur_temps = temps_total;
            stats[nombre_epreuves].pire_temps = temps_total;
            stats[nombre_epreuves].total_temps = temps_total;
            stats[nombre_epreuves].nombre_courses = 1;
            nombre_epreuves++;
        } else {
            if (temps_total < stats[j].meilleur_temps) {
                stats[j].meilleur_temps = temps_total;
            }
            if (temps_total > stats[j].pire_temps) {
                stats[j].pire_temps = temps_total;
            }
            stats[j].total_temps += temps_total;
            stats[j].nombre_courses++;
        }
    }

    printf("Statistiques de performance pour %s %s:\n", sportif->prenom, sportif->nom);
    for (int i = 0; i < nombre_epreuves; i++) {
        int heures_meilleur = (int)stats[i].meilleur_temps / 3600;
        int minutes_meilleur = ((int)stats[i].meilleur_temps % 3600) / 60;
        int secondes_meilleur = (int)stats[i].meilleur_temps % 60;
        int millisecondes_meilleur = (int)(stats[i].meilleur_temps * 100) % 100;

        int heures_pire = (int)stats[i].pire_temps / 3600;
        int minutes_pire = ((int)stats[i].pire_temps % 3600) / 60;
        int secondes_pire = (int)stats[i].pire_temps % 60;
        int millisecondes_pire = (int)(stats[i].pire_temps * 100) % 100;

        double temps_moyen = stats[i].total_temps / stats[i].nombre_courses;
        int heures_moyen = (int)temps_moyen / 3600;
        int minutes_moyen = ((int)temps_moyen % 3600) / 60;
        int secondes_moyen = (int)temps_moyen % 60;
        int millisecondes_moyen = (int)(temps_moyen * 100) % 100;

        printf("\n%s:\n", stats[i].epreuve);
        printf("\tMeilleur temps: %02d:%02d:%02d.%02d\n", heures_meilleur, minutes_meilleur, secondes_meilleur, millisecondes_meilleur);
        printf("\tPire temps: %02d:%02d:%02d.%02d\n", heures_pire, minutes_pire, secondes_pire, millisecondes_pire);
        printf("\tTemps moyen: %02d:%02d:%02d.%02d\n", heures_moyen, minutes_moyen, secondes_moyen, millisecondes_moyen);
    }
}


#include <dirent.h>

// Structure definitions and constants would go here (omitted for brevity)

void choisir_athletes_pour_JO(char *epreuve) {
    Sportif athletes[MAX_ATHLETES]; // Utilisation de la structure Sportif pour stocker les athlètes
    int nombre_athletes = 0;

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                FILE *fichier = fopen(ent->d_name, "r");
                if (fichier == NULL) {
                    printf("Erreur lors de l'ouverture du fichier %s pour lecture.\n", ent->d_name);
                    perror("fopen");
                    continue;
                }

                double total_temps = 0;
                int nombre_courses = 0;

                char nom_athlete[LONGUEUR_MAX_NOM];
                strncpy(nom_athlete, ent->d_name, strlen(ent->d_name) - 4);
                nom_athlete[strlen(ent->d_name) - 4] = '\0';

                char ligne[LONGUEUR_MAX_LIGNE];
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    char date[LONGUEUR_MAX_DATE], event[LONGUEUR_MAX_EPREUVE];
                    int temps_h, temps_m, temps_s, temps_ms, position;
                    sscanf(ligne, "%10s %9s %d %d %d %d %d", date, event, &temps_h, &temps_m, &temps_s, &temps_ms, &position);

                    if (strcmp(event, epreuve) == 0) {
                        total_temps += temps_h * 3600 + temps_m * 60 + temps_s + temps_ms / 100.0;
                        nombre_courses++;
                    }
                }
                fclose(fichier);

                if (nombre_courses > 0) {
                    // Ajoute les données de l'athlète à la structure Sportif
                    strcpy(athletes[nombre_athletes].prenom, ""); // Remplir avec les informations de l'athlète si nécessaire
                    strcpy(athletes[nombre_athletes].nom, nom_athlete);
                    athletes[nombre_athletes].temps_moyen = total_temps / nombre_courses;
                    nombre_athletes++;
                }
            }
        }
        closedir(dir);
    } else {
        printf("Erreur lors de l'ouverture du répertoire courant.\n");
        perror("opendir");
    }

    // Tri des athlètes par temps moyen croissant
    for (int i = 0; i < nombre_athletes - 1; i++) {
        for (int j = 0; j < nombre_athletes - i - 1; j++) {
            if (athletes[j].temps_moyen > athletes[j + 1].temps_moyen) {
                Sportif temp = athletes[j];
                athletes[j] = athletes[j + 1];
                athletes[j + 1] = temp;
            }
        }
    }

    printf("\nLes trois meilleurs athlètes pour l'épreuve %s sont:\n", epreuve);
    for (int i = 0; i < 3 && i < nombre_athletes; i++) {
        double temps_moyen = athletes[i].temps_moyen;
        int heures = (int)temps_moyen / 3600;
        int minutes = ((int)temps_moyen % 3600) / 60;
        int secondes = (int)temps_moyen % 60;
        int millisecondes = (int)(temps_moyen * 100) % 100;

        printf("\n%s %s avec un temps moyen de %02d:%02d:%02d.%02d\n", athletes[i].prenom, athletes[i].nom, heures, minutes, secondes, millisecondes);
    }
}

void consulter_performance(Sportif *sportif, char *epreuve, char *date1, char *date2) {
    charger_donnees_entrainement(sportif);

    double temps1 = -1, temps2 = -1;

    for (int i = 0; i < sportif->nb_entrainements; i++) {
        Entrainement *entrainement = &sportif->entrainement[i];
        double temps_total = entrainement->temps_h * 3600 + entrainement->temps_m * 60 + entrainement->temps_s + entrainement->temps_ms / 100.0;

        if (strcmp(entrainement->epreuve, epreuve) == 0) {
            if (strcmp(entrainement->date, date1) == 0) {
                temps1 = temps_total;
            } else if (strcmp(entrainement->date, date2) == 0) {
                temps2 = temps_total;
            }
        }
    }

    if (temps1 == -1 || temps2 == -1) {
        printf("Les temps pour les dates spécifiées ne sont pas disponibles.\n");
        return;
    }

    printf("Comparaison des performances pour %s %s dans l'épreuve %s entre %s et %s:\n", sportif->prenom, sportif->nom, epreuve, date1, date2);
    printf("Temps à %s: %02d:%02d:%02d.%02d\n", date1, (int)temps1 / 3600, ((int)temps1 % 3600) / 60, (int)temps1 % 60, (int)(temps1 * 100) % 100);
    printf("Temps à %s: %02d:%02d:%02d.%02d\n", date2, (int)temps2 / 3600, ((int)temps2 % 3600) / 60, (int)temps2 % 60, (int)(temps2 * 100) % 100);

    double difference = temps2 - temps1;
    if (difference < 0) {
        difference = -difference;
        int heures = (int)difference / 3600;
        int minutes = ((int)difference % 3600) / 60;
        int secondes = (int)difference % 60;
        int millisecondes = (int)((difference - ((int)difference)) * 100);
        printf("L'athlète a progressé de %02d:%02d:%02d.%02d.\n", heures, minutes, secondes, millisecondes);
    } else if (difference > 0) {
        int heures = (int)difference / 3600;
        int minutes = ((int)difference % 3600) / 60;
        int secondes = (int)difference % 60;
        int millisecondes = (int)((difference - ((int)difference)) * 100);
        printf("L'athlète a régressé de %02d:%02d:%02d.%02d.\n", heures, minutes, secondes, millisecondes);
    } else {
        printf("L'athlète a maintenu sa performance.\n");
    }
}



void afficher_relais(char *date_relais) {
    DIR *dir;
    struct dirent *ent;
    int participants_relais[4] = {0}; // Pour vérifier les positions 1 à 4
    char noms_fichiers_relais[4][LONGUEUR_NOM_FICHIER]; // Pour stocker les noms de fichiers des participants
    Entrainement temps_participants[4]; // Pour stocker les temps des participants
    double temps_total_relais = 0; // Temps total du relais

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                FILE *fichier = fopen(ent->d_name, "r");
                if (fichier == NULL) {
                    printf("Erreur lors de l'ouverture du fichier %s pour lecture.\n", ent->d_name);
                    perror("fopen");
                    continue;
                }

                char ligne[LONGUEUR_MAX_LIGNE];
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    Entrainement entrainement;
                    sscanf(ligne, "%10s %9s %d %d %d %d %d", entrainement.date, entrainement.epreuve, 
                           &entrainement.temps_h, &entrainement.temps_m, &entrainement.temps_s, 
                           &entrainement.temps_ms, &entrainement.position_relais);

                    if (strcmp(entrainement.date, date_relais) == 0 && strcmp(entrainement.epreuve, "relais") == 0) {
                        if (entrainement.position_relais >= 1 && entrainement.position_relais <= 4) {
                            strcpy(noms_fichiers_relais[entrainement.position_relais - 1], ent->d_name);
                            participants_relais[entrainement.position_relais - 1] = 1;
                            temps_participants[entrainement.position_relais - 1] = entrainement;
                            temps_total_relais += entrainement.temps_h * 3600 + entrainement.temps_m * 60 + entrainement.temps_s + entrainement.temps_ms / 1000.0;
                        }
                    }
                }
                fclose(fichier);
            }
        }
        closedir(dir);
    } else {
        printf("Erreur lors de l'ouverture du répertoire courant.\n");
        perror("opendir");
    }

    printf("Relais du %s:\n", date_relais);
    int relais_complet = 1;
    for (int i = 0; i < 4; i++) {
        if (participants_relais[i]) {
            Entrainement *entrainement = &temps_participants[i];
            char prenom[LONGUEUR_NOM_FICHIER];
            sscanf(noms_fichiers_relais[i], "%[^.].txt", prenom);

            printf("Participant %d: %s, Temps: %02d:%02d:%02d.%03d\n",
                   i + 1,
                   prenom,
                   entrainement->temps_h,
                   entrainement->temps_m,
                   entrainement->temps_s,
                   entrainement->temps_ms);
        } else {
            relais_complet = 0;
            printf("Participant %d: Information non disponible\n", i + 1);
        }
    }

    if (relais_complet) {
        int total_h = (int)temps_total_relais / 3600;
        int total_m = ((int)temps_total_relais % 3600) / 60;
        int total_s = (int)temps_total_relais % 60;
        int total_ms = (int)(temps_total_relais * 1000) % 1000;
        printf("Temps total du relais: %02d:%02d:%02d.%03d\n", total_h, total_m, total_s, total_ms);
    } else {
        printf("Le relais n'est pas complet.\n");
    }
}



// Fonction pour vérifier si une chaîne ne contient que des lettres
int verifie_prenom(char *prenom) {
    for (int i = 0; i < strlen(prenom); i++) {
        if (!isalpha(prenom[i])) {
            return 0; // Si un caractère n'est pas une lettre, retourner 0
        }
    }
    return 1; // Si tous les caractères sont des lettres, retourner 1
}

// Fonction pour vérifier si une date est valide (JJ/MM/AAAA)
int date_valide(int jour, int mois, int annee) {
    // Vérifie les mois de 30 jours
    if (mois == 4 || mois == 6 || mois == 9 || mois == 11) {
        return jour >= 1 && jour <= 30;
    }
    // Février
    if (mois == 2) {
        // Année bissextile
        if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
            return jour >= 1 && jour <= 29;
        } else {
            return jour >= 1 && jour <= 28;
        }
    }
    // Tous les autres mois
    return jour >= 1 && jour <= 31;
}

// Fonction pour vérifier si une chaîne correspond à un format de date "JJ/MM/AAAA"
int verifie_date(char *date) {
    if (strlen(date) != 10) return 0; // Vérifie la longueur

    // Vérifie le format "JJ/MM/AAAA"
    if (date[2] != '/' || date[5] != '/') return 0;

    // Vérifie si les parties du jour, du mois et de l'année sont toutes des chiffres
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue; // Ignore les slashes
        if (!isdigit(date[i])) return 0;
    }

    // Extraire jour, mois, année
    int jour = atoi(&date[0]);
    int mois = atoi(&date[3]);
    int annee = atoi(&date[6]);

    // Vérifie si la date est valide
    if (mois < 1 || mois > 12 || !date_valide(jour, mois, annee)) {
        return 0;
    }

    return 1;
}

int verifier_epreuve(const char *epreuve) {
    const char *epreuves[5] = {"100m", "400m", "5000m", "marathon", "relais"};

    for (int i = 0; i < 5; i++) {
        if (strcmp(epreuve, epreuves[i]) == 0) {
            return 1; // L'épreuve existe
        }
    }

    return 0; // L'épreuve n'existe pas
}

int verifier_format_temps(int heures, int minutes, int secondes, int millisecondes) {
    return (heures >= 0 && heures <= 23) &&
           (minutes >= 0 && minutes <= 59) &&
           (secondes >= 0 && secondes <= 59) &&
           (millisecondes >= 0 && millisecondes <= 999);
}








