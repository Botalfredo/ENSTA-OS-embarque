#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// Handler du signal déclenché par le timer
void timer_handler(int sig) {
    printf("Timer déclenché\n");
}

int main() {
    struct sigevent sev;
    struct itimerspec its;
    timer_t timerid;
    struct sigaction sa;

    // Configuration du gestionnaire de signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("Erreur avec sigaction");
        exit(EXIT_FAILURE);
    }

    // Créer un timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN; // Utilise un signal en temps réel
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("Erreur lors de la création du timer");
        exit(EXIT_FAILURE);
    }

    // Configurer le timer pour une période de 1 seconde
    its.it_value.tv_sec = 1;   // Délai avant la première expiration
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1; // Période du timer
    its.it_interval.tv_nsec = 0;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("Erreur lors de la configuration du timer");
        exit(EXIT_FAILURE);
    }

    // Boucle infinie pour attendre le signal
    while (1) {
        pause(); // Attente du signal
    }

    return 0;
}
