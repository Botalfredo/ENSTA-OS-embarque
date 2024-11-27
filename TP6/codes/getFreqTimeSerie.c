#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#define NANO 1000000000

static timer_t timer = (timer_t)0;

// curFreq : Fichier de fréquence courante
// output : Fichier de trace en sortie
FILE *curFreq, *output;
struct timespec t0;

void handler_signal(int inutilise) {
    static struct timespec instant = {0, 0};
    char freq[20], towrite[100];
    long int timestamp;

    // Lecture de l'instant présent avec la fonction clock_gettime
    clock_gettime(CLOCK_REALTIME, &instant);

    // Lecture de la fréquence courante freq depuis le fichier /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
    curFreq = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
    if (!curFreq) {
        perror("Erreur lors de l'ouverture du fichier de fréquence courante");
        exit(EXIT_FAILURE);
    }
    if (fgets(freq, sizeof(freq), curFreq) == NULL) {
        perror("Erreur lors de la lecture de la fréquence courante");
        fclose(curFreq);
        exit(EXIT_FAILURE);
    }
    fclose(curFreq);

    // Calcul du timestamp : instant - t0
    timestamp = (instant.tv_sec - t0.tv_sec) * 1000000 + (instant.tv_nsec - t0.tv_nsec) / 1000;

    // Écriture dans le fichier output
    sprintf(towrite, "%ld\t%s\n", timestamp, freq);
    if (fputs(towrite, output) < 0) {
        perror("Erreur lors de l'écriture dans le fichier output");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    int periode;
    struct sigevent event;
    struct itimerspec spec;

    if ((argc != 3) || (sscanf(argv[1], "%d", &periode) != 1)) {
        fprintf(stderr, "usage: %s periode(ms) output\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Ouverture du fichier output
    output = fopen(argv[2], "w");
    if (!output) {
        perror("Erreur lors de l'ouverture du fichier output");
        exit(EXIT_FAILURE);
    }

    // Lecture de l'instant t0 à partir duquel sera calculé le timestamp
    clock_gettime(CLOCK_REALTIME, &t0);

    // Configurer le timer : à la réception du signal SIGRTMIN, exécuter handler_signal()
    signal(SIGRTMIN, handler_signal);

    // Notification par signal
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGRTMIN;

    // Conversion de la période en nanosecondes
    periode = periode * 1000;

    spec.it_interval.tv_sec = periode / NANO;
    spec.it_interval.tv_nsec = periode % NANO;
    spec.it_value = spec.it_interval;

    if (timer_create(CLOCK_REALTIME, &event, &timer) != 0) {
        perror("Erreur lors de la création du timer");
        exit(EXIT_FAILURE);
    }

    if (timer_settime(timer, 0, &spec, NULL) != 0) {
        perror("Erreur lors de la configuration du timer");
        exit(EXIT_FAILURE);
    }

    while (1)
        pause();

    fclose(output);

    return EXIT_SUCCESS;
}

