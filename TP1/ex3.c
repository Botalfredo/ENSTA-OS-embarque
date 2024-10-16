#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*  Exemple: ./ex3 -p 6 -m -c   */

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

// Fonction pour afficher les informations sur le CPU
void afficher_info_cpu() {
    // cf ex1 

    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) {
        perror("Erreur lors de l'ouverture de /proc/cpuinfo");
        return;
    }
    
    char buffer[256];
    char modele[256] = "";
    char frequence[256] = "";
    char cache_size[256] = "";
    char address_size[256] = "";

    while (fgets(buffer, sizeof(buffer), f)) {
        if (strncmp(buffer, "model name", 10) == 0) {
            strcpy(modele, strchr(buffer, ':') + 2);
        } else if (strncmp(buffer, "cpu MHz", 7) == 0) {
            strcpy(frequence, strchr(buffer, ':') + 2);
        } else if (strncmp(buffer, "cache size", 10) == 0) {
            strcpy(cache_size, strchr(buffer, ':') + 2);
        } else if (strncmp(buffer, "address sizes", 13) == 0) {
            strcpy(address_size, strchr(buffer, ':') + 2);
        }
    }

    printf("=== Informations sur le CPU ===\n");
    printf("Modèle de CPU: %s", modele);
    printf("Fréquence: %s", frequence);
    printf("Taille du cache: %s", cache_size);
    printf("Taille de l'espace d'adressage: %s", address_size);

    fclose(f);
}

// Fonction pour afficher les informations de la mémoire
void afficher_info_memoire() {
    // cf ex1 

    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) {
        perror("Erreur lors de l'ouverture de /proc/meminfo");
        return;
    }
    
    char buffer[256];
    long mem_total = 0;
    long mem_free = 0;

    while (fgets(buffer, sizeof(buffer), f)) {
        if (strncmp(buffer, "MemTotal", 8) == 0) {
            sscanf(buffer, "MemTotal: %ld kB", &mem_total);
        } else if (strncmp(buffer, "MemFree", 7) == 0) {
            sscanf(buffer, "MemFree: %ld kB", &mem_free);
        }
    }

    printf("=== Informations sur la mémoire ===\n");
    printf("Mémoire totale: %ld Mo\n", mem_total / 1024);
    printf("Mémoire libre: %ld Mo\n", mem_free / 1024);

    fclose(f);
}

// Fonction pour afficher les informations sur un processus à partir de son PID
void afficher_info_processus(int pid) {
    // cf ex2

    char chemin[256];
    char contenu[256];

    printf("=== Informations sur le processus %d ===\n", pid);

    // Lire le nom de la commande exécutée (fichier /proc/<pid>/comm)
    // Cette fonction crée une chaîne de caractères formatée. On inscrit ainsi dans chemin le string contenu dans /proc/%d/comm" de taille max sizeof(chemin)
    snprintf(chemin, sizeof(chemin), "/proc/%d/comm", pid);
    // Ouverte du fichier comm en lecture uniquement et création d'un pointeur vers le fichier ouvert f.
    FILE *f = fopen(chemin, "r");
    if (!f) {
        perror("Erreur lors de l'ouverture de /proc/<pid>/comm");
        return;
    }
    // On inscrit dans contenu la premiière ligne du fichier f d'une maximale de sizeof(contenu)
    fgets(contenu, sizeof(contenu), f);
    fclose(f);
    printf("Nom de la commande : %s", contenu);

    // Lire le TGID, le PPID, le nombre de threads, l'UID et le GID (fichier /proc/<pid>/status)
    // Même fonctionnement que precedement
    snprintf(chemin, sizeof(chemin), "/proc/%d/status", pid);
    f = fopen(chemin, "r");
    if (!f) {
        perror("Erreur lors de l'ouverture de /proc/<pid>/status");
        return;
    }

    // Init des variables que l'on va cherché + buffer ou seront stocké temprairement les lignes
    int tgid = 0, ppid = 0, threads = 0, uid = 0, gid = 0;
    char buffer[256];
    // Tant qu'il reste des signe dans f, on la stock dans buffer sans depasser la taille de buffer
    while (fgets(buffer, sizeof(buffer), f)) {
        // Si les 5 premier termes de buffer correspondent au String entre guillement, alors strncmp = 0
        if (strncmp(buffer, "Tgid:", 5) == 0) {
            // On enrengistre un entier que l'on va venir recuperer avec sscanf en passant le mot + une tabulation (\t), la valeur se trouve après. 
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
    // Fermeture et affichage des info du fils
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

void help() {
    printf("Aide pour l'utilisation des options de ex3 :\n\n");

    printf("-h          Affiche cette aide\n");
    printf("-p [id]     Affiche des informatin relatives au pid avec l'indice [id]\n");
    printf("-c          Affiche les informations sur le CPU du PC\n");
    printf("-m          Affiche les informations sur la mémoire\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int pid = -1;

    // Analyse des options de ligne de commande avec getopt. Tant que getopt à des options non-parsé, la valeur de opt est differends 1
    // (elle représente les options pas encoré traité). argc = nb d'option et argv le tableau des arguments. Les options possible sont décrite avec
    // le troisieme argument de la fonction, ici h, c, m, p avec un argument à sa suite attendus.
    while ((opt = getopt(argc, argv, "hcmp:")) != -1) {
        switch (opt) {
            case 'h':
                help();
                return 0;
                break;
            case 'c':
                afficher_info_cpu();
                break;
            case 'm':
                afficher_info_memoire();
                break;
            case 'p':
            // optarg représente la valeur indiqué après p que l'on convertie ne int.
                pid = atoi(optarg);
                afficher_info_processus(pid);
                break;
            default:
                fprintf(stderr, "Usage: %s [-c] [-m] [-p <pid>]\n", argv[0]);
                help();
                exit(EXIT_FAILURE);
        }
    }

    return 0;
}