#include "structures.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    while (1) {
        printf("\nMenu Principal:\n");
        printf("1. Ajouter de nouvelles données d'entraînement\n");
        printf("2. Afficher l'historique d'entraînement\n");
        printf("3. Afficher les statistiques de performance\n");
        printf("4. Choisir les athlètes pour les JO\n");
        printf("5. Consulter la progression des performances\n");
        printf("6. Afficher les informations d'un relais\n");
        printf("7. Quitter\n");

        int choix;
        do {
            printf("\nEntrez votre choix: ");
            if (scanf("%d", &choix) != 1) {
                printf("Entrée invalide. Veuillez entrer un nombre.\n");
                while (getchar() != '\n'); // Vider le buffer d'entrée
            } else {
                break; // Sort de la boucle si une entrée valide est donnée
            }
        } while (1);

        switch (choix) {
            case 1: {
                char prenom[LONGUEUR_MAX_NOM], nom[LONGUEUR_MAX_NOM], date[LONGUEUR_MAX_DATE], epreuve[LONGUEUR_MAX_EPREUVE];
                int temps_h, temps_m, temps_s, temps_ms, position;
                bool est_valide;


                // Demande et vérifie le prénom
                while (1) {
                    printf("Entrez le prénom de l'athlète: ");
                    scanf("%s", prenom);
                    if (verifie_prenom(prenom)) break;
                    printf("Erreur: le prénom ne doit contenir que des lettres.\n");
                }

                // Demande et vérifie le nom
                while (1) {
                    printf("Entrez le nom de l'athlète: ");
                    scanf("%s", nom);
                    if (verifie_prenom(nom)) break;
                    printf("Erreur: le nom ne doit contenir que des lettres.\n");
                }

                // Demande et vérifie la date
                while (1) {
                    printf("Entrez la date (JJ/MM/AAAA): ");
                    scanf("%s", date);
                    if (verifie_date(date)) break; 
                    printf("Erreur: la date doit être au format JJ/MM/AAAA et doit être valide.\n");
                }

                // Demande et vérifie l'épreuve
                while (1) {
                    printf("Entrez l'épreuve (100m, 400m, 5000m, marathon ou relais): ");
                    scanf("%s", epreuve);
                    if (verifier_epreuve(epreuve)) break;
                    printf("Erreur: épreuve invalide.\n");
                }

                // Demande et vérifie le temps
                while (1) {
                printf("Entrez le temps (h m s ms): ");
                    int nb_scanned = scanf("%d %d %d %d", &temps_h, &temps_m, &temps_s, &temps_ms);

                    // Assure que 4 entiers ont été correctement lus
                    if (nb_scanned == 4) {
                        verifier_format_temps(temps_h, temps_m, temps_s, temps_ms, &est_valide);
                        if (est_valide) break;
                        else printf("Erreur: format de temps invalide.\n");
                    } else {
                        printf("Erreur: veuillez entrer des chiffres pour h, m, s, et ms.\n");
                        while (getchar() != '\n'); // Vider le buffer d'entrée
                    }
                }

                Sportif sportif;
                strcpy(sportif.prenom, prenom);
                strcpy(sportif.nom, nom);
                sportif.nb_entrainements = 0;

                if (strcmp(epreuve, "relais") == 0) {
                    printf("Entrez la position dans le relais (1-4): ");
                    scanf("%d", &position);
                    if (position < 1 || position > 4) {
                        printf("Position invalide. La position doit être entre 1 et 4.\n");
                        break;
                    }
                } else {
                    position = 0;
                }

                Entrainement entrainement;
                strcpy(entrainement.date, date);
                strcpy(entrainement.epreuve, epreuve);
                entrainement.temps_h = temps_h;
                entrainement.temps_m = temps_m;
                entrainement.temps_s = temps_s;
                entrainement.temps_ms = temps_ms;
                entrainement.position_relais = position;

                ajouter_donnees_entrainement(&sportif, entrainement);
                break;
            }
            case 2: {
                char prenom[LONGUEUR_MAX_NOM], nom[LONGUEUR_MAX_NOM], epreuve[LONGUEUR_MAX_EPREUVE], date_debut[LONGUEUR_MAX_DATE], date_fin[LONGUEUR_MAX_DATE];

                // Demande et vérifie le prénom
                while (1) {
                    printf("Entrez le prénom de l'athlète: ");
                    scanf("%s", prenom);
                    if (verifie_prenom(prenom)) break;
                    printf("Erreur: le prénom ne doit contenir que des lettres.\n");
                }

                // Demande et vérifie le nom
                while (1) {
                    printf("Entrez le nom de l'athlète: ");
                    scanf("%s", nom);
                    if (verifie_prenom(nom)) break;
                    printf("Erreur: le nom ne doit contenir que des lettres.\n");
                }

                printf("Entrez l'épreuve - 100m, 400m, 5000m, marathon ou relais (facultatif, appuyez sur Entrée pour ignorer) : ");
                getchar(); // pour consommer le retour à la ligne laissé par le précédent scanf
                fgets(epreuve, sizeof(epreuve), stdin);
                epreuve[strcspn(epreuve, "\n")] = '\0';
                printf("Entrez la date de début (facultatif, appuyez sur Entrée pour ignorer) : ");
                fgets(date_debut, sizeof(date_debut), stdin);
                date_debut[strcspn(date_debut, "\n")] = '\0';
                printf("Entrez la date de fin (facultatif, appuyez sur Entrée pour ignorer) : ");
                fgets(date_fin, sizeof(date_fin), stdin);
                date_fin[strcspn(date_fin, "\n")] = '\0';

                Sportif sportif;
                strcpy(sportif.prenom, prenom);
                strcpy(sportif.nom, nom);
                sportif.nb_entrainements = 0;

                charger_donnees_entrainement(&sportif);

                afficher_historique_entrainement(&sportif, epreuve[0] != '\0' ? epreuve : NULL, date_debut[0] != '\0' ? date_debut : NULL, date_fin[0] != '\0' ? date_fin : NULL);
                break;
            }
            case 3: {
                char prenom[LONGUEUR_MAX_NOM], nom[LONGUEUR_MAX_NOM];

                // Demande et vérifie le prénom
                while (1) {
                    printf("Entrez le prénom de l'athlète: ");
                    scanf("%s", prenom);
                    if (verifie_prenom(prenom)) break;
                    printf("Erreur: le prénom ne doit contenir que des lettres.\n");
                }

                // Demande et vérifie le nom
                while (1) {
                    printf("Entrez le nom de l'athlète: ");
                    scanf("%s", nom);
                    if (verifie_prenom(nom)) break;
                    printf("Erreur: le nom ne doit contenir que des lettres.\n");
                }

                Sportif sportif;
                strcpy(sportif.prenom, prenom);
                strcpy(sportif.nom, nom);
                sportif.nb_entrainements = 0;

                afficher_statistiques_performance(&sportif);
                break;
            }
            case 4: {
                char epreuve[LONGUEUR_MAX_EPREUVE];

                // Demande et vérifie l'épreuve
                while (1) {
                    printf("Entrez l'épreuve (100m, 400m, 5000m, marathon ou relais): ");
                    scanf("%s", epreuve);
                    if (verifier_epreuve(epreuve)) break;
                    printf("Erreur: épreuve invalide.\n");
                }

                choisir_athletes_pour_JO(epreuve);
                break;
            }
            case 5: {
                char prenom[LONGUEUR_MAX_NOM], nom[LONGUEUR_MAX_NOM], epreuve[LONGUEUR_MAX_EPREUVE], date1[LONGUEUR_MAX_DATE], date2[LONGUEUR_MAX_DATE];

                // Demande et vérifie le prénom
                while (1) {
                    printf("Entrez le prénom de l'athlète: ");
                    scanf("%s", prenom);
                    if (verifie_prenom(prenom)) break;
                    printf("Erreur: le prénom ne doit contenir que des lettres.\n");
                }

                // Demande et vérifie le nom
                while (1) {
                    printf("Entrez le nom de l'athlète: ");
                    scanf("%s", nom);
                    if (verifie_prenom(nom)) break;
                    printf("Erreur: le nom ne doit contenir que des lettres.\n");
                }

                // Demande et vérifie l'épreuve
                while (1) {
                    printf("Entrez l'épreuve (100m, 400m, 5000m, marathon ou relais): ");
                    scanf("%s", epreuve);
                    if (verifier_epreuve(epreuve)) break;
                    printf("Erreur: épreuve invalide.\n");
                }

                // Demande et vérifie la première date
                while (1) {
                    printf("Entrez la première date (JJ/MM/AAAA): ");
                    scanf("%s", date1);
                    if (verifie_date(date1)) break;
                    printf("Erreur: la date doit être au format JJ/MM/AAAA et doit être valide.\n");
                }

                // Demande et vérifie la deuxième date
                while (1) {
                    printf("Entrez la deuxième date (JJ/MM/AAAA): ");
                    scanf("%s", date2);
                    if (verifie_date(date2)) break;
                    printf("Erreur: la date doit être au format JJ/MM/AAAA et doit être valide.\n");
                }

                Sportif sportif;
                strcpy(sportif.prenom, prenom);
                strcpy(sportif.nom, nom);
                sportif.nb_entrainements = 0;

                consulter_performance(&sportif, epreuve, date1, date2);
                break;
            }
            case 6: {
                char date_relais[LONGUEUR_MAX_DATE];
                while (1) {
                    printf("Entrez la date du relais (JJ/MM/AAAA): ");
                    scanf("%s", date_relais);
                    if (verifie_date(date_relais)) break;
                    printf("Date invalide. Veuillez entrer une date au format JJ/MM/AAAA et valide.\n");
                }
                afficher_relais(date_relais);
                break;
            }
            case 7: {
                printf("Au revoir!\n");
                exit(0);
            }
            default: {
                printf("Choix invalide. Veuillez réessayer.\n");
            }
        }
    }

    return 0;
}
