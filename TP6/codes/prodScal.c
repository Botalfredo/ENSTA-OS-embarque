#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#ifndef RAND_MAX
#define RAND_MAX ((int)((unsigned)~0 >> 1))
#endif

// Calcul du produit scalaire entre deux vecteurs
double produit(double *V1, double *V2, long TAILLE) {
    double res = 0.0;
    for (long i = 0; i < TAILLE; i++) {
        res += V1[i] * V2[i];
    }
    return res;
}

// Génération de deux vecteurs de valeurs aléatoires
void genVectors(double *V1, double *V2, long TAILLE) {
    for (long i = 0; i < TAILLE; i++) {
        V1[i] = (double)rand() / RAND_MAX;
        V2[i] = (double)rand() / RAND_MAX;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s Taille\n", argv[0]);
        exit(0);
    }

    long TAILLE = atol(argv[1]);

    // Allocation dynamique des vecteurs V1 et V2
    double *V1 = (double *)malloc(TAILLE * sizeof(double));
    double *V2 = (double *)malloc(TAILLE * sizeof(double));

    if (V1 == NULL || V2 == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }

    srand((int)time(NULL)); // Initialisation de la fonction rand
    genVectors(V1, V2, TAILLE);

    printf("%lf\n", produit(V1, V2, TAILLE));

    // Libération de la mémoire allouée
    free(V1);
    free(V2);

    return EXIT_SUCCESS;
}
