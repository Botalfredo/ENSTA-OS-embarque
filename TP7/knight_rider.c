#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void set_led_brightness(const char *led_name, int value);

int main(int argc, char *argv[]) {
    const char *leds[] = {
        "beaglebone:green:heartbeat",
        "beaglebone:green:mmc0",
        "beaglebone:green:usr2",
        "beaglebone:green:usr3"
    };

    int num_leds = sizeof(leds) / sizeof(leds[0]);
    int delay = 200000; // Valeur par défaut : 200 ms

    // Vérifier les arguments
    if (argc > 1) {
        int user_delay = atoi(argv[1]);
        if (user_delay > 0) {
            delay = user_delay * 1000; // Convertir millisecondes en microsecondes
        } else {
            fprintf(stderr, "Usage : %s [délai en ms]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    printf("Effet Knight Rider sur les LEDs avec un délai de %d ms...\n", delay / 1000);

    while (1) {
        // Mouvement gauche → droite
        for (int i = 0; i < num_leds; i++) {
            set_led_brightness(leds[i], 1); // Allumer la LED
            usleep(delay);                 // Pause
            set_led_brightness(leds[i], 0); // Éteindre la LED
        }

        // Mouvement droite → gauche
        for (int i = num_leds - 2; i >= 0; i--) { // Éviter la LED extrême déjà allumée
            set_led_brightness(leds[i], 1); // Allumer la LED
            usleep(delay);                 // Pause
            set_led_brightness(leds[i], 0); // Éteindre la LED
        }
    }

    return 0;
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

