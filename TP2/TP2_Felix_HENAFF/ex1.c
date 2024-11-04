#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <sched.h>

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

void get_priority_via_proc() {
    FILE *fp;
    char path[256];
    int priority;

    // On enrengistre dans le tableau path le String du chemin vers stat qui contiens la priorité.
    snprintf(path, sizeof(path), "/proc/self/stat");
    // Ouverture du fichier décris par path en lecture uniquement
    fp = fopen(path, "r");

    // Gestion de l'erreur
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture de /proc/self/stat");
        exit(EXIT_FAILURE);
    }

    // Lecture de la 18ème valeur qui correspond à la priorité
    for (int i = 0; i < 18; i++) {
        fscanf(fp, "%*s");
    }
    // On récupère un int, que l'on renregistre dans priority grace a l'addresse que on lui a donné
    fscanf(fp, "%d", &priority);

    fclose(fp);

    printf("Méthode 2: Priorité du processus via /proc/self/stat: %d\n", priority);
}

int main() {
    get_priority_via_getpriority();
    get_priority_via_proc();
    return 0;
}
