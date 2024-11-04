// TP2_4b.c
// Compilation: gcc TP2_4b.c -o 4_b -lrt
// Exécution: sudo .TP2/TP2_4b 1000 > temps.txt

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

static timer_t timer = (timer_t) 0;          // Identifiant du timer
static long int *mesures = NULL;             // Tableau pour stocker les différences de temps
static int nb_mesures = 0;                   // Compteur du nombre de mesures effectuées
static int nb_total_mesures = 0;             // Nombre total de mesures à réaliser

// Fonction appelée lors de chaque expiration du timer
void handler_signal(int inutilise) {
    struct timespec heure_actuelle;
    static struct timespec heure_precedente = {0, 0}; // Dernière heure à laquelle le timer a expiré

    // Obtenir le temps actuel
    clock_gettime(CLOCK_MONOTONIC, &heure_actuelle);

    // Si c'est la première exécution, on initialise l'heure précédente
    if (heure_precedente.tv_sec > 0) {
        // Calculer la différence entre l'instant actuel et la précédente expiration
        long diff_sec = heure_actuelle.tv_sec - heure_precedente.tv_sec;
        long diff_nsec = heure_actuelle.tv_nsec - heure_precedente.tv_nsec;

        // Ajustement si la différence en nanosecondes est négative
        if (diff_nsec < 0) {
            diff_sec -= 1;
            diff_nsec += 1000000000; // Ajouter 1 seconde en nanosecondes
        }

        // Stocker la différence en nanosecondes dans le tableau de mesures
        mesures[nb_mesures] = diff_sec * 1000000000L + diff_nsec;
        nb_mesures++;
    }

    // Mettre à jour l'heure précédente
    heure_precedente = heure_actuelle;

    // Si toutes les mesures sont prises, supprimer le timer
    if (nb_mesures == nb_total_mesures) {
        timer_delete(timer);
    }
}

int main(int argc, char *argv[]) {
    long int periode;
    struct sigevent event;
    struct itimerspec spec;

    // Vérifier que l'utilisateur a bien passé un argument pour la période
    if ((argc != 2) || (sscanf(argv[1], "%ld", &periode) != 1)) {
        fprintf(stderr, "usage: %s periode_en_microsec\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Nombre de mesures à réaliser en 5 secondes (par exemple)
    nb_total_mesures = 5000000 / periode; // 5 secondes / période en microsecondes

    // Allouer de la mémoire pour stocker les mesures
    mesures = malloc(nb_total_mesures * sizeof(long int));
    if (mesures == NULL) {
        perror("Erreur lors de l'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Configurer le timer pour recevoir des signaux SIGRTMIN
    signal(SIGRTMIN, handler_signal);

    // Notification par signal
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGRTMIN;

    // Conversion de la période de microsecondes en nanosecondes
    periode = periode * 1000; // Conversion en nanosecondes

    // Définir la période du timer
    spec.it_interval.tv_sec = 0;
    spec.it_interval.tv_nsec = periode;
    spec.it_value = spec.it_interval;

    // Créer le timer
    if (timer_create(CLOCK_MONOTONIC, &event, &timer) != 0) {
        perror("Erreur lors de la création du timer");
        free(mesures);
        exit(EXIT_FAILURE);
    }

    // Programmer le timer
    if (timer_settime(timer, 0, &spec, NULL) != 0) {
        perror("Erreur lors de la programmation du timer");
        free(mesures);
        exit(EXIT_FAILURE);
    }

    // Attendre que toutes les mesures soient prises
    while (nb_mesures < nb_total_mesures) {
        pause();
    }

    // Écrire les mesures en microsecondes dans le fichier de sortie
    for (int i = 0; i < nb_total_mesures; i++) {
        printf("%ld\n", mesures[i] / 1000); // Convertir les nanosecondes en microsecondes
    }

    // Libérer la mémoire allouée
    free(mesures);

    return EXIT_SUCCESS;
}
