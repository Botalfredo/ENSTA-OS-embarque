#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <sched.h>

// ==================================
//     Pour execution: sudo ./ex2
// ==================================

void set_priority(int priority) {
    pid_t pid = getpid();

    // On change la priorité du processus (indiqué par PRIO_PROCESS) en précisant qui priorisé avec pid avec la priorité priority
    if (setpriority(PRIO_PROCESS, pid, priority) == -1) {
        perror("Erreur lors de l'appel à setpriority");
        exit(EXIT_FAILURE);
    }

    printf("Priorité du processus (PID: %d) modifiée à: %d\n", pid, priority);
}

void get_priority_via_getpriority() {
    // On récupère le processus id du process
    pid_t pid = getpid();
    // On utilise getpriority() pour connaitre la priorité du processus en cours d'exécution avec PRIO_PROCESS en précisan l'id du processus à priorisé
    int priority = getpriority(PRIO_PROCESS, pid);

    // Gestion d'erreur
    if (priority == -1 && errno != 0) {
        perror("Erreur lors de l'appel à getpriority");
        exit(EXIT_FAILURE);
    }

    // Affichage (0 = priorité par défaut, -20 plus haute et 19 plus basse)
    printf("Méthode 1: Priorité du processus (PID: %d) via getpriority(): %d\n", pid, priority);
}

int main() {
    printf("Affichage de la priorité avant modification:\n");
    get_priority_via_getpriority();

    // Augmente la priorité à -10 (valeur plus élevée)
    set_priority(-10);

    printf("Affichage de la priorité après modification:\n");
    get_priority_via_getpriority();

    return 0;
}