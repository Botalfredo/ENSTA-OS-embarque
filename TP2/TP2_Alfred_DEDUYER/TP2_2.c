// TP2_2.c

// gcc -o TP2_2.c TP2_2.c
// sudo ./TP2/TP2_2

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

int main() {
    int priority;

    // Récupérer la priorité actuelle du processus
    priority = getpriority(PRIO_PROCESS, 0); // 0 pour le processus courant
    
    printf("Priorité actuelle : %d\n", priority);

    // Changer la priorité à une valeur plus élevée (par exemple -10)
    printf("Changement de priorité à -10...\n");
    if (setpriority(PRIO_PROCESS, 0, -10) == -1) {
        perror("Erreur avec setpriority");
        return 1;
    }

    // Récupérer la nouvelle priorité
    priority = getpriority(PRIO_PROCESS, 0);
    
    printf("Nouvelle priorité après changement : %d\n", priority);

    return 0;
}
