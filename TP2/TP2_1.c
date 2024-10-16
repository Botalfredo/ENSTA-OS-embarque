#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

int main() {
    // Part 1a: Utilisation de getpriority()
    int priority = getpriority(PRIO_PROCESS, 0); // 0 pour le processus courant
    if (priority == -1) {
        perror("Erreur avec getpriority");
    } else {
        printf("1a: Priorité actuelle du processus : %d\n", priority);
    }

    // Part 1b: Accéder au répertoire /proc/self 
    FILE *file = fopen("/proc/self/stat", "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier /proc/self/stat");
        return 1;
    }

    // Lire toute la ligne du fichier /proc/self/stat
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        int pid;
        char comm[256];
        char state;
        int priority;

        // Extraire les champs importants
        sscanf(buffer, "%d %s %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %d",
               &pid, comm, &state, &priority);

        printf("PID: %d\n", pid);
        printf("Nom du processus: %s\n", comm);
        printf("État du processus: %c\n", state);
        printf("Priorité actuelle (champ 18) : %d\n", priority);
    } else {
        perror("Erreur lors de la lecture du fichier /proc/self/stat");
    }

    fclose(file);
    return 0;
}