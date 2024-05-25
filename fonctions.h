#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <stdbool.h>
#include "structures.h"

void ajouter_donnees_entrainement(Sportif *sportif, Entrainement entrainement);
void afficher_historique_entrainement(Sportif *sportif, char *epreuve, char *date_debut, char *date_fin);
void afficher_statistiques_performance(Sportif *sportif);
void choisir_athletes_pour_JO(char *epreuve);
void charger_donnees_entrainement(Sportif *sportif);
void consulter_performance(Sportif *sportif, char *epreuve, char *date1, char *date2);
void afficher_relais(char *date_relais);
int verifier_relais_existant(char *date);
int verifie_prenom(char *prenom);
int date_valide(int jour, int mois, int annee);
int verifie_date(char *date);
int verifier_epreuve(const char *epreuve);
void verifier_format_temps(int heures, int minutes, int secondes, int millisecondes, bool *resultat);


#endif










