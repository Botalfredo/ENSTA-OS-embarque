#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

// VÃ©rifie si un nombre nb est premier
int isPrime(int nb) {
    if (nb <= 1) return 0;  // 0 et 1 ne sont pas des nombres premiers
    if (nb <= 3) return 1;  // 2 et 3 sont des nombres premiers
    if (nb % 2 == 0 || nb % 3 == 0) return 0;  // Divisibles par 2 ou 3 ne sont pas premiers

    for (int i = 5; i * i <= nb; i += 6) {
        if (nb % i == 0 || nb % (i + 2) == 0)
            return 0;
    }
    return 1;
}

// Compte le nombre de nombres premiers
int primeNumbers(int N) {
    int count = 0;
    for (int i = 2; i < N; i++) {
        count += isPrime(i);
    }
    return count;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s N\n", argv[0]);
        exit(0);
    }

    int N = atoi(argv[1]);
    printf("Le nombre de nombres premiers entre 2 et %d est : %d\n", N, primeNumbers(N));

    return EXIT_SUCCESS;
}

