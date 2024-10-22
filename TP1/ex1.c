#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour afficher les informations du CPU
void afficher_info_cpu() {

    // Ouverture du fichier /proc/cpuinfo contenant toutes les information dont on a besoin dans cette fonction en mode read (d'ou le r),
    // fopen renvoir un pointeur vers le fichier ainsi ouvert pour  y acceder via d'autres fonctions.
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) {
        // Affichage d'une erreur si ouverture imposible
        perror("Erreur lors de l'ouverture de /proc/cpuinfo");
        return;
    }
    
    // Declaration des tableau pour enregistrer les informations lu dans le fichier
    char buffer[256];
    char modele[256] = "";
    char frequence[256] = "";
    char cache_size[256] = "";
    char address_size[256] = "";

    // Lecture ligne par ligne du fichier f avec fgets qui permet de lire les lignes une par une en les stockants dans une variable
    // donnée (ici buffer)
    while (fgets(buffer, sizeof(buffer), f)) {
        // Ici ou vien verifier pour chaque ligne si celle-ci contiens l'un des mots clé recherché avec strncmp (string compare). Si tout les
        // char comparés sont les mêmes alors on à 0 différence donc d'ou le == 0, et donc les 2 mots sont identiques
        if (strncmp(buffer, "model name", 10) == 0) {
            // Si les deux mots sont identiques, on cherche dans le buffer le premier char ':' que l'on inore avec +2 pour ne copier avec strcpy
            // dans le tableau model que ce qui reste à partir de cette position.
            strcpy(modele, strchr(buffer, ':') + 2);
        } else if (strncmp(buffer, "cpu MHz", 7) == 0) {
            // On répète l'opération précédente à chaque fois.
            strcpy(frequence, strchr(buffer, ':') + 2);
        } else if (strncmp(buffer, "cache size", 10) == 0) {
            strcpy(cache_size, strchr(buffer, ':') + 2);
        } else if (strncmp(buffer, "address sizes", 13) == 0) {
            strcpy(address_size, strchr(buffer, ':') + 2);
        }
    }

    // Affichage des Strings avec %s 
    printf("Modèle de CPU: %s", modele);
    printf("Fréquence: %s", frequence);
    printf("Taille du cache: %s", cache_size);
    printf("Taille de l'espace d'adressage: %s", address_size);

    // On refecrme le fichier f
    fclose(f);
}

// Fonction pour afficher les informations de la mémoire
void afficher_info_memoire() {
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

    printf("Mémoire totale: %ld Mo\n", mem_total / 1024);
    printf("Mémoire libre: %ld Mo\n", mem_free / 1024);

    fclose(f);
}

// Fonction pour afficher les informations des partitions
void afficher_info_partitions() {
    FILE *f = fopen("/proc/partitions", "r");
    if (!f) {
        perror("Erreur lors de l'ouverture de /proc/partitions");
        return;
    }

    char buffer[256];
    printf("Partitions:\n");
    fgets(buffer, sizeof(buffer), f); // Ignorer la première ligne (en-têtes)
    fgets(buffer, sizeof(buffer), f); // Ignorer la deuxième ligne

    // La boucle while lit chaque ligne de f avec fgets jusqu'a la fin
    while (fgets(buffer, sizeof(buffer), f)) {
        int major, minor;
        long blocks;
        char name[32];
        // Ici les informations de chaque ligne son trié avec sscanf et sont enrengistré en fonction du format.
        sscanf(buffer, "%d %d %ld %s", &major, &minor, &blocks, name);
        printf("- %s : %ld Mo\n", name, blocks / 1024);
    }

    fclose(f);
}

// Fonction pour afficher la version du noyau
void afficher_info_version() {
    FILE *f = fopen("/proc/version", "r");
    if (!f) {
        perror("Erreur lors de l'ouverture de /proc/version");
        return;
    }

    char buffer[256];
    // Ici rien a trié, on affiche toite la ligne.
    fgets(buffer, sizeof(buffer), f);
    printf("Version du noyau: %s", buffer);

    fclose(f);
}

int main() {
    printf("=== Informations sur la machine et le système ===\n");
    afficher_info_cpu();
    printf("\n");
    afficher_info_memoire();
    printf("\n");
    afficher_info_partitions();
    printf("\n");
    afficher_info_version();

    return 0;
}