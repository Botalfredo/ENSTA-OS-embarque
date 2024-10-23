#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LG_LIGNE   1024

int main(int argc, char * argv[])
{
    char ligne[LG_LIGNE];
    int nb_mesures;
    int nb_echantillons;

    double mesure;
    double minimum = -1;
    double maximum = 0;
    double largeur_echantillon;
    int *echantillons;

    int i;

    // Vérification des arguments : nombre d'échantillons, minimum et maximum
    if ((argc != 4)
     || (sscanf(argv[1], "%d", &nb_echantillons) != 1)
     || (sscanf(argv[2], "%lf", &minimum) != 1)
     || (sscanf(argv[3], "%lf", &maximum) != 1)) {
        fprintf(stderr, "usage: %s nb_echantillons minimum maximum\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (nb_echantillons <= 0) {
        fprintf(stderr, "Valeur invalide : %d\n", nb_echantillons);
        exit(EXIT_FAILURE);
    }

    // Calcul de la largeur de chaque échantillon
    largeur_echantillon = (double)(maximum - minimum) / nb_echantillons;

    // Allocation dynamique de la mémoire pour les échantillons
    echantillons = calloc(nb_echantillons, sizeof(int));
    if (echantillons == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    nb_mesures = 0;

    // Lire les mesures depuis stdin (par exemple, redirection du fichier TP2/temps.txt)
    while (fgets(ligne, LG_LIGNE, stdin) != NULL) {
        if (sscanf(ligne, "%lf", &mesure) != 1) {
            fprintf(stderr, "Erreur de lecture, ligne %d\n", nb_mesures + 1);
            exit(EXIT_FAILURE);
        }
        nb_mesures++;
        i = (int)((mesure - minimum) / largeur_echantillon);
        if (i < 0 || i >= nb_echantillons) {
            fprintf(stderr, "Valeur invalide ligne %d\n", nb_mesures + 1);
            exit(EXIT_FAILURE);
        }
        echantillons[i]++;
    }

    // Afficher les résultats (l'histogramme)
    for (i = 0; i < nb_echantillons; i++) {
        printf("%.0lf %d\n", minimum + (i + 0.5) * largeur_echantillon, echantillons[i]);
    }

    // Libération de la mémoire
    free(echantillons);

    return EXIT_SUCCESS;
}
