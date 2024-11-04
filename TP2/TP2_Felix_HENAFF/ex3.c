#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

void set_realtime_priority() {
    struct sched_param param;
    pid_t pid = getpid();

    // Définir la priorité temps réel (priorité FIFO)
    param.sched_priority = 10; // Choisissez une priorité entre 1 et 99 pour SCHED_FIFO

    // Appliquer la politique d'ordonnancement FIFO avec la priorité spécifiée
    if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1) {
        perror("Erreur lors de l'appel à sched_setscheduler");
        exit(EXIT_FAILURE);
    }

    printf("Le processus (PID: %d) est maintenant en temps réel avec la politique FIFO et priorité: %d\n", pid, param.sched_priority);
}

void get_realtime_priority() {
    struct sched_param param;
    pid_t pid = getpid();

    // Récupérer la politique d'ordonnancement du processus (ici FIFO)
    int policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("Erreur lors de l'appel à sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    // Récupérer les paramètres de priorité
    if (sched_getparam(pid, &param) == -1) {
        perror("Erreur lors de l'appel à sched_getparam");
        exit(EXIT_FAILURE);
    }

    // Afficher la politique d'ordonnancement et la priorité
    const char *policy_name = (policy == SCHED_FIFO) ? "SCHED_FIFO" :
                              (policy == SCHED_RR) ? "SCHED_RR" :
                              (policy == SCHED_OTHER) ? "SCHED_OTHER" : "UNKNOWN";
    printf("Politique d'ordonnancement actuelle: %s, Priorité: %d\n", policy_name, param.sched_priority);
}

int main() {
    printf("Affichage des informations avant modification:\n");
    get_realtime_priority();

    // Mettre le processus en temps réel avec SCHED_FIFO
    set_realtime_priority();

    printf("Affichage des informations après modification:\n");
    get_realtime_priority();

    return 0;
}
