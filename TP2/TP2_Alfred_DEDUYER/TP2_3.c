// TP2_3.c

//sudo ./TP2/TP2_3.c

#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    struct sched_param param;
    int policy;

    // Def de prioritée (0 - 99)
    param.sched_priority = 50;

    // FIFO pour processus courant
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("Erreur lors du passage en politique FIFO");
        return 1;
    }

    // Affichage de lordonancement 
    policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("Erreur lors de la récupération de la politique d'ordonnancement");
        return 1;
    }

    switch (policy) {
        case SCHED_FIFO:
            printf("Politique d'ordonnancement actuelle : FIFO\n");
            break;
        case SCHED_RR:
            printf("Politique d'ordonnancement actuelle : Round Robin\n");
            break;
        case SCHED_OTHER:
            printf("Politique d'ordonnancement actuelle : Autre\n");
            break;
        default:
            printf("Politique d'ordonnancement inconnue\n");
            break;
    }

    // Récupérer et afficher la priorité 
    if (sched_getparam(0, &param) == -1) {
        perror("Erreur lors de la récupération de la priorité");
        return 1;
    }

    printf("Priorité actuelle du processus : %d\n", param.sched_priority);

    return 0;
}
