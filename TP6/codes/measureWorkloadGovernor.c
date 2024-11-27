#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int fd;
    char filename[256];
    long long int duree;
    struct timeval start, end;
    pid_t pid;

    if (argc != 4) {
        fprintf(stderr, "Usage : %s <governor> <workload> <params>\n", argv[0]);
        exit(1);
    }

    // Formation du chemin vers le fichier gouverneur pour le CPU 0
    sprintf(filename, "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");

    // Ouverture du fichier gouverneur
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror(filename);
        exit(2);
    }

    // Écriture du nom du gouverneur récupéré en paramètre dans le fichier
    if (write(fd, argv[1], strlen(argv[1])) < 0) {
        perror("Erreur lors de l'écriture du gouverneur");
        close(fd);
        exit(3);
    }

    close(fd);
    sleep(1); // Laisser le temps au système de prendre en compte le changement

    // Mesure du temps d'exécution
    gettimeofday(&start, NULL);

    pid = fork();
    if (pid == 0) {
        // Processus fils : lancement du workload
        execlp(argv[2], argv[2], argv[3], NULL); // argv[3] contient les paramètres du workload
        perror("Erreur lors de l'exécution du workload");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Processus parent : attente du fils
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Workload terminé avec le code : %d\n", WEXITSTATUS(status));
        } else {
            fprintf(stderr, "Erreur lors de l'exécution du fils\n");
        }
    } else {
        perror("Erreur lors de la création du processus fils");
        exit(4);
    }

    gettimeofday(&end, NULL);

    // Calcul de la durée en microsecondes
    duree = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    printf("Durée d'exécution : %lld microsecondes\n", duree);

    return EXIT_SUCCESS;
}
