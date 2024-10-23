#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

// ./TP2/TP2_4b


static timer_t timer = (timer_t) 0;
static long int *mesure = NULL;    // Tableau pour stocker les temps entre chaque expiration
static int nb_mesures = 0;         // Compteur du nombre de fois que le timer a expiré
static int nb_total_mesures = 0;   // Nombre total de mesures à réaliser
static FILE *fichier = NULL;       // Fichier pour écrire les temps

// Fonction appelée lors de chaque expiration du timer
void handler_signal(int inutilise)
{
    struct timespec heure;         // Temps actuel
    static struct timespec precedente = {0, 0}; // Dernière heure où le timer a expiré

    // *** Mesurer le temps courant avec clock_gettime
    clock_gettime(CLOCK_MONOTONIC, &heure);
    
    if (precedente.tv_sec > 0) {
        // Calculer la différence entre l'instant actuel et la précédente mesure
        long diff_sec = heure.tv_sec - precedente.tv_sec;
        long diff_nsec = heure.tv_nsec - precedente.tv_nsec;
        
        // Ajuster pour les cas où la différence en nanosecondes est négative
        if (diff_nsec < 0) {
            diff_sec -= 1;
            diff_nsec += 1000000000; // Ajouter 1 seconde en nanosecondes
        }
        
        // Stocker la différence (temps écoulé) en nanosecondes dans le tableau mesure
        mesure[nb_mesures] = diff_sec * 1000000000L + diff_nsec;
        nb_mesures++;  // Incrémenter le nombre de mesures
    }

    precedente = heure;  // Mettre à jour le temps précédent
    
    // Si toutes les mesures ont été prises, supprimer le timer
    if (nb_mesures == nb_total_mesures)
        timer_delete(timer);
}

int main(int argc, char * argv[])
{
    long int periode;
    struct sigevent event;
    struct itimerspec spec;

    // Vérification du nombre d'arguments
    if ((argc != 2) || (sscanf(argv[1], "%ld", &periode) != 1)) {
        fprintf(stderr, "usage: %s periode_en_microsec\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Nombre total de mesures réalisées en 5 secondes (par exemple)
    nb_total_mesures = 5000000 / periode; // 5 secondes

    // Allouer de la mémoire pour stocker les mesures
    mesure = malloc(nb_total_mesures * sizeof(long int));
    if (mesure == NULL) {
        perror("Erreur lors de l'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier pour écrire les résultats
    fichier = fopen("TP2/temps.txt", "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier temps.txt");
        free(mesure);
        exit(EXIT_FAILURE);
    }

    // Configurer le timer pour recevoir des signaux SIGRTMIN
    signal(SIGRTMIN, handler_signal);
    
    // *** Notification par signal
    event.sigev_notify = SIGEV_SIGNAL;  // Notification par signal
    event.sigev_signo = SIGRTMIN;       // Signal à envoyer lors de l'expiration du timer
    
    // Conversion de la période de microsecondes en nanosecondes
    periode = periode * 1000;  // Conversion en nanosecondes

    // *** Définir la période (partie entière et fractionnaire)
    spec.it_interval.tv_sec = 0;           // Intervalle de répétition en secondes
    spec.it_interval.tv_nsec = periode;    // Intervalle en nanosecondes
    spec.it_value = spec.it_interval;      // La première expiration est la même que l'intervalle
    
    // *** Créer le timer
    if (timer_create(CLOCK_MONOTONIC, &event, &timer) != 0) {
        perror("Erreur lors de la création du timer");
        free(mesure);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    // *** Programmer le timer
    if (timer_settime(timer, 0, &spec, NULL) != 0) {
        perror("Erreur lors de la programmation du timer");
        free(mesure);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    // Attendre que toutes les mesures soient prises
    while (nb_mesures < nb_total_mesures)
        pause();

    // Écrire les mesures en microsecondes dans le fichier
    for (nb_mesures = 0; nb_mesures < nb_total_mesures; nb_mesures++)
        fprintf(fichier, "%ld\n", mesure[nb_mesures] / 1000); // Convertir les nanosecondes en microsecondes et écrire dans le fichier
    
    // Libérer la mémoire allouée et fermer le fichier
    free(mesure);
    fclose(fichier);

    printf("Les temps ont été écrits dans le fichier temps.txt\n");

    return EXIT_SUCCESS;
}
