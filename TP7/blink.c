#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void set_led_brightness(const char *led_name, int value);

int main() {
    const char *led = "beaglebone:green:heartbeat";
    int delay_on = 500000; // 500 ms
    int delay_off = 500000; // 500 ms

    printf("Clignotement de la LED %s...\n", led);

    while (1) {
        // Allumer la LED
        set_led_brightness(led, 1);
        usleep(delay_on); // Pause de 500 ms

        // Éteindre la LED
        set_led_brightness(led, 0);
        usleep(delay_off); // Pause de 500 ms
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

