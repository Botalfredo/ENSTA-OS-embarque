// TP2_4c.c
// Compilation: gcc TP2/TP2_4c.c -o TP2/TP2_4c -lm
// Exécution: ./TP2/TP2_4c < TP2/temps.txt

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LG_LIGNE 1024

int main(int argc, char *argv[])
{
    // Variables pour stocker les valeurs et calculs
    char ligne[LG_LIGNE];
    double mesure;
    int nb_mesures = 0;
    double minimum;
    double maximum;
    double moyenne = 0.0;
    double variance = 0.0;
    double delta;
    double ecart_type;

    // Lecture des mesures depuis l'entrée standard
    while (fgets(ligne, LG_LIGNE, stdin) != NULL) {
        if (sscanf(ligne, "%lf", &mesure) != 1) {
            fprintf(stderr, "Erreur de lecture, ligne %d\n", nb_mesures + 1);
            exit(EXIT_FAILURE);
        }

        nb_mesures++;
        
        // Mise à jour des valeurs minimales et maximales
        if ((nb_mesures == 1) || (minimum > mesure))
            minimum = mesure;
        if ((nb_mesures == 1) || (maximum < mesure))
            maximum = mesure;
        
        // Calcul incrémental de la moyenne et de la variance
        delta = mesure - moyenne;
        moyenne += delta / nb_mesures;
        variance += delta * (mesure - moyenne);
    }

    // Calcul de la variance finale et de l'écart-type
    if (nb_mesures > 1)
        variance = variance / (nb_mesures - 1);
    
    ecart_type = sqrt(variance);

    // Affichage des résultats
    printf(" Nb mesures = %d\n Minimum = %.0lf\n Maximum = %.0lf\n"
           " Moyenne = %.0lf\n Ecart-type = %.0lf\n",
           nb_mesures, minimum, maximum, moyenne, ecart_type);

    return EXIT_SUCCESS;
}
