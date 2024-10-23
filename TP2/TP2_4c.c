#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// gcc -o TP2_4c TP2_4c.c -lm
// ./TP2/TP2_4c


#define LG_LIGNE   1024

int main() {
    char ligne[LG_LIGNE];
    FILE *fichier;
    double mesure;
    int nb_mesures = 0;
    double minimum;
    double maximum;
    double moyenne  =  0.0;
    double variance =  0.0;
    double delta;
    double ecart_type;

    // Ouvrir le fichier temps.txt en mode lecture
    fichier = fopen("TP2/temps.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier temps.txt");
        exit(EXIT_FAILURE);
    }

    // Lire chaque ligne du fichier
    while (fgets(ligne, LG_LIGNE, fichier) != NULL) {
        if (sscanf(ligne, "%lf", &mesure) != 1) {
            fprintf(stderr, "Erreur de lecture à la ligne %d\n", nb_mesures + 1);
            exit(EXIT_FAILURE);
        }
        nb_mesures++;
        if ((nb_mesures == 1) || (minimum > mesure)) {
            minimum = mesure;
        }
        if ((nb_mesures == 1) || (maximum < mesure)) {
            maximum = mesure;
        }
        delta = mesure - moyenne;
        moyenne  += delta / nb_mesures;
        variance += delta * (mesure - moyenne);
    }

    // Calcul de la variance et de l'écart-type
    if (nb_mesures > 1) {
        variance = variance / (nb_mesures - 1);
    }
    ecart_type = sqrt(variance);

    // Affichage des résultats
    printf("Nb mesures = %d\n", nb_mesures);
    printf("Minimum = %.0lf\n", minimum);
    printf("Maximum = %.0lf\n", maximum);
    printf("Moyenne = %.0lf\n", moyenne);
    printf("Ecart-type = %.0lf\n", ecart_type);

    // Fermer le fichier
    fclose(fichier);

    return EXIT_SUCCESS;
}
