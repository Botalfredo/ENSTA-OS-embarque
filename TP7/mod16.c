#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_LEDS 4

const char *leds[] = {
    "beaglebone:green:heartbeat",
    "beaglebone:green:mmc0",
    "beaglebone:green:usr2",
    "beaglebone:green:usr3"
};

// Variables globales
volatile sig_atomic_t counter = 0; // Compteur partagé

// Fonctions
void set_led_brightness(const char *led_name, int value);
void update_leds(int value);
void signal_handler(int sig);

int main() {
    pid_t pid;

    // Créer un processus enfant pour envoyer des signaux
    pid = fork();
    if (pid < 0) {
        perror("Erreur lors du fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Processus enfant : Émetteur de signaux
        pid_t parent_pid = getppid(); // PID du parent
        while (1) {
            kill(parent_pid, SIGUSR1); // Envoyer SIGUSR1 au parent
            sleep(1);                  // Pause de 1 seconde
        }
        exit(EXIT_SUCCESS);
    } else {
        // Processus parent : Compteur
        struct sigaction sa;
        sa.sa_handler = signal_handler;
        sa.sa_flags = SA_RESTART; // Redémarrer les appels système interrompus
        sigemptyset(&sa.sa_mask);

        if (sigaction(SIGUSR1, &sa, NULL) == -1) {
            perror("Erreur lors de sigaction");
            exit(EXIT_FAILURE);
        }

        // Initialisation des LEDs
        for (int i = 0; i < NUM_LEDS; i++) {
            set_led_brightness(leds[i], 0); // Éteindre toutes les LEDs
        }

        printf("Processus compteur (PID: %d) en attente de signaux...\n", getpid());

        // Boucle principale
        while (1) {
            pause(); // Attendre un signal
        }

        // Nettoyage (éteindre les LEDs avant de quitter)
        for (int i = 0; i < NUM_LEDS; i++) {
            set_led_brightness(leds[i], 0);
        }

        // Attendre la fin de l'enfant
        wait(NULL);
    }

    return 0;
}

void signal_handler(int sig) {
    if (sig == SIGUSR1) {
        counter = (counter + 1) % 16; // Incrémenter le compteur modulo 16
        update_leds(counter);        // Mettre à jour les LEDs
    }
}

void update_leds(int value) {
    for (int i = 0; i < NUM_LEDS; i++) {
        int bit = (value >> i) & 1;   // Extraire le i-ème bit
        set_led_brightness(leds[i], bit);
    }
}

void set_led_brightness(const char *led_name, int value) {
    char path[100];
    snprintf(path, sizeof(path), "/sys/class/leds/%s/brightness", led_name);

    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Erreur d'ouverture du fichier brightness");
        exit(EXIT_FAILURE);
    }

    char val = value ? '1' : '0';
    if (write(fd, &val, 1) != 1) {
        perror("Erreur d'écriture dans brightness");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}

