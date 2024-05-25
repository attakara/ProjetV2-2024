Fiche Technique : Suivi des Performances des Athlètes

Notre objectif est de développer un programme permettant à l’entraîneur de l’équipe d’athlétisme de France de suivre l’évolution des performances de ses sportifs. Les différentes performances des athlètes lors des entraînements seront stockées dans des fichiers. Notre programme doit permettre de les mettre à jour, de consulter certains résultats et de produire quelques statistiques sur les athlètes.

I) Fonction Principale
Dans la fonction principale, nous affichons un menu numéroté de 1 à 7. L’utilisateur entre un numéro correspondant à une fonction spécifique. Voici les options disponibles :

1.	Ajouter une de nouvelles données d’entrainement :
o	L’utilisateur saisit le nom, prénom, date, temps et épreuve. Si c’est un relais, il demande également la position du relais.
o	La saisie est vérifiée pour correspondre au format requis.

2.	Historique d’entrainement :
o	 Il demande le nom de l’athlète, s’il n’existe pas un message d’erreur s’affiche. Et il redemande tant que le nom ne correspond pas. Sinon il demande le type d’épreuve : et affiche les statistiques pour chaque date. Optionnellement si vous voulez afficher que pour une date spécifique, vous devez entrer la date spécifique, sinon vous appuyez sur entrée pour ignorer. Et il va exécuter la fonction consulter performance.

3.	Afficher les Statistiques de performances :
o	Il te demande le nom et prénom de l’athlète, et ils les intègrent dans la structure sportive, il initialise le nombre d’entrainement du sportif à zéro. Et il exécute la fonction affiche statistiques des performances.

4.	Classement des Athlètes :
o	L’utilisateur saisit le nom de l’épreuve, et il exécute la fonction choisir athlètes qui retourne les 3 meilleurs athlètes et l’affiche.

5.	Comparer les Performances :
o	L’utilisateur saisit le nom, prénom, épreuve et deux dates à comparer.
o	La fonction compare les performances pour connaître la progression de l’athlète.

6.	Afficher les informations d’un relais
L’utilisateur entre la date du relais en respectant le format et utilise la fonction affiche relais


II) Les Fonctions

1.	Afficher_statistiques_performance (Sportif *sportif) :
o	Compare les statistiques de performance (meilleur temps, pire temps et temps moyen) entre les différents jours d’entraînement pour une épreuve donnée.
o	Les données sont récupérées à partir du fichier texte correspondant à l’athlète.

2.	Consulter_performance (Sportif *sportif, char *epreuve, char *date1, char *date2) :
o	Retourne les temps des performances des joueurs.
o	Permet de voir la progression de l’athlète dans ses performances.

3.	Choisir_athletes_pour_JO (char *epreuve) :
o	Demande à l’utilisateur de choisir les athlètes qui iront aux Jeux Olympiques (JO) pour une épreuve donnée.
o	Compare les meilleures performances de chaque athlète et classe les trois meilleurs en fonction du temps.

4.	Afficher_relais (char *date_relais) :
o	Renvoi un relais à la date saisi par l’utilisateur et affiche les participants, leur position, leur temps et enfin le temps complet du relais.
o	Un maximum de 4 athlètes peut participer au relais par jour.

5.	Verifier_relais_existant (char *date) :
o	Empêche la création d’un autre relais pour le même athlète à la même date.
o	Vérifie si la position du relais est déjà occupée.

6.	Verifier_format_dates :
o	Vérifie le format des dates saisies.

7.	Verifier_format_valeur :
o	Vérifie si la valeur choisie correspond au format demandé (chaîne de caractères ou chiffres).

8.	Verifier_epreuve (const char *epreuve) :
o	Vérifie si l’épreuve correspond aux épreuves pouvant être implantées.

9.	Verifier_format_temps (int heures, int minutes, int secondes, int millisecondes) :
o	Vérifie si le format du temps est respecté.

10.	Ajouter_donnees_entrainement (Sportif *sportif, Entrainement entrainement) :
o	Ajoute les données d’entraînement dans le fichier texte de l’athlète.
o	Pour un relais, vérifie la position du joueur.

11.	Charger_donnees_entrainement (Sportif *sportif) :
on	Gère le nombre d’entrainement.
