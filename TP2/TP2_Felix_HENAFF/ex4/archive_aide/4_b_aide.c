// ---------------------------------------------------------------------
// Fichier d'exemple pour le TP2 Question 4-a --> création des timers
// VO par Christophe BLAESS
// Adapté par Jalil Boukhobza
// ---------------------------------------------------------------------

// usage: ./4_b_aide 1000 > data_4_b.txt

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>


static timer_t timer = (timer_t) 0;
static long int * mesure = NULL;
static int nb_mesures = 0;
static int nb_total_mesures = 0;

void handler_signal(int inutilise)
{
	struct timespec heure;
	static struct timespec precedente = { 0, 0} ;

	// printf("In handler");

	// *** a compléter --> mesurer le temps
	clock_gettime(CLOCK_MONOTONIC, &heure);
	
	if (precedente.tv_sec > 0) {
		// *** à compléter --> mesure[nb_mesures] doit récupérer la différence entre l'instant actuelle et la précédente mesure
		// ATTENTION ici à considérer la partie entière et fractionnaire

		// On calcule la différence entre les mesures actuelles et précédentes en nanosecondes
		long int diff_sec = heure.tv_sec - precedente.tv_sec; 		// Différence en secondes
		long int diff_nsec = heure.tv_nsec - precedente.tv_nsec; 	// Différence en nanosecondes
		
		// Convertir tout en nanosecondes et stocker dans mesure[nb_mesures]
		mesure[nb_mesures] = diff_sec * 1000000000L + diff_nsec;
	}
	
	precedente = heure;
	nb_mesures++;

	// Supprimer le timer si on a atteint le nombre total de mesures
	if (nb_mesures == nb_total_mesures)
		timer_delete(timer);
}


int main(int argc, char * argv[])
{
	long int periode;
	struct sigevent event;
	struct itimerspec spec;
	
	// Vérification des arguments
	if ((argc != 2) || (sscanf(argv[1], "%ld", & periode) != 1)){
		fprintf(stderr, "usage: ./4_b_aide [periode en microsecondes] > [file_name].txt");
		exit(EXIT_FAILURE);
	}
	
	// nombre de mesues réalisées en 5 secondes (par exemple)
	nb_total_mesures = 5000000 / periode; // 5 secondes
	
	// Allouer de la mémoire pour stocker les mesures
	mesure = (long int *) malloc(nb_total_mesures * sizeof(long int));
	if (mesure == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// Configurer le timer
	// à la reception du signal SIGRTMIN, executer la fonction handler_signal()
	signal(SIGRTMIN, handler_signal);
	
	//*** a compléter --> nous souhaitons être notifié par signal
	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo = SIGRTMIN;
	
	periode = periode * 1000;  // en nanosec
	
	//*** à compléter --> définir la partie entière et fractionnaire
	spec.it_interval.tv_sec = 0;
	spec.it_interval.tv_nsec = periode;
	spec.it_value = spec.it_interval;
	
	
	// *** à compléter --> allouer le timer
	if (timer_create(CLOCK_MONOTONIC, &event, &timer) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	}
	
	
	// à compléter --> programmer le timer
	if (timer_settime(timer, 0, &spec, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
	
	// Attendre le dernier signal
	while (nb_mesures < nb_total_mesures)
		pause();
		
	// Afficher les mesures en microsecondes
	for (int i = 0; i < nb_total_mesures; i++) {
		printf("%ld\n", mesure[i] / 1000); // Conversion nanosecondes en microsecondes
	}

	// Libérer la mémoire
	free(mesure);
		
	return EXIT_SUCCESS;
}

