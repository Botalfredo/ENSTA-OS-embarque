#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Ecrivez un programme qui prend en paramètre un numéro de pid et qui affiche pour ce processus :
// 
// 1) Lire les PID -> ps aux ou top
// 2) ./ex2 PID

// Fonction pour lire le contenu d'un fichier et le retourner sous forme de chaîne
void lire_fichier(const char *chemin, char *contenu, size_t taille_max) {
    FILE *f = fopen(chemin, "r");
    if (!f) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    fgets(contenu, taille_max, f);
    fclose(f);
}

// Fonction pour afficher les informations sur un processus à partir de son PID
void afficher_info_processus(int pid) {
    char chemin[256];
    char contenu[256];

    // Afficher le PID du processus
    printf("PID : %d\n", pid);

    // Lire le nom de la commande exécutée (fichier /proc/<pid>/comm)
    snprintf(chemin, sizeof(chemin), "/proc/%d/comm", pid);
    lire_fichier(chemin, contenu, sizeof(contenu));
    printf("Nom de la commande : %s", contenu);

    // Lire le TGID (fichier /proc/<pid>/status) (TGID = Thread group id)
    snprintf(chemin, sizeof(chemin), "/proc/%d/status", pid);
    FILE *f = fopen(chemin, "r");
    if (!f) {
        perror("Erreur lors de l'ouverture du fichier /proc/<pid>/status");
        exit(EXIT_FAILURE);
    }

    int tgid = 0, ppid = 0, threads = 0, uid = 0, gid = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strncmp(buffer, "Tgid:", 5) == 0) {
            sscanf(buffer, "Tgid:\t%d", &tgid);
        } else if (strncmp(buffer, "PPid:", 5) == 0) {
            sscanf(buffer, "PPid:\t%d", &ppid);
        } else if (strncmp(buffer, "Threads:", 8) == 0) {
            sscanf(buffer, "Threads:\t%d", &threads);
        } else if (strncmp(buffer, "Uid:", 4) == 0) {
            sscanf(buffer, "Uid:\t%d", &uid);
        } else if (strncmp(buffer, "Gid:", 4) == 0) {
            sscanf(buffer, "Gid:\t%d", &gid);
        }
    }
    fclose(f);

    printf("TGID : %d\n", tgid);
    printf("PPID : %d\n", ppid);

    // Lire le nom de la commande du père (fichier /proc/<ppid>/comm)
    snprintf(chemin, sizeof(chemin), "/proc/%d/comm", ppid);
    lire_fichier(chemin, contenu, sizeof(contenu));
    printf("Nom de la commande du père : %s", contenu);

    printf("Nombre de threads fils : %d\n", threads);
    printf("UID : %d\n", uid);
    printf("GID : %d\n", gid);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int pid = atoi(argv[1]);
    afficher_info_processus(pid);

    return 0;
}