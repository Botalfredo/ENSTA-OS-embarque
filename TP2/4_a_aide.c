// ---------------------------------------------------------------------
// Fichier d'exemple pour le TP2 Question 4-a --> création des timers
// VO par Christophe BLAESS
// Adapté par Jalil Boukhobza
// ---------------------------------------------------------------------

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
	// fonction à exécuter à chaque expiration du timer 
	// *** à compléter --> incrémetner le nombre de mesures ici, nb_mesures, quelque part
}


int main(int argc, char * argv[])
{
	long int periode;
	struct sigevent event;
	struct itimerspec spec;
	
	if ((argc != 2) || (sscanf(argv[1], "%ld", & periode) != 1)){
		fprintf(stderr, "usage: %s periode_en_microsec\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	// nombre de mesues réalisées en 5 secondes (par exemple)
	nb_total_mesures = 5000000 / periode; // 5 secondes
	
	// Configurer le timer
	// à la reception du signal SIGRTMIN, executer la fonction handler_signal()
	signal(SIGRTMIN, handler_signal);
	
	//*** a compléter --> nous souhaitons être notifié par signal
	//event.sigev_notify = 
	//event.sigev_signo  = 
	
	periode = periode * 1000;  // en nanosec
	
	//*** à compléter --> définir la partie entière et fractionnaire
	//spec.it_interval.tv_sec  = 
	//spec.it_interval.tv_nsec = 
	spec.it_value = spec.it_interval;
	
	
	// *** à compléter --> allouer le timer
	//if (timer_create(      ) !=  ) {
	//	perror("timer_create");
	//	exit(EXIT_FAILURE);
	//} 
	
	
	// à compléter --> programmer le timer
	//if (timer_settime(    ) !=    ) {
	//	perror("timer_settime");
	//	exit(EXIT_FAILURE);
	//}
	
	
	// Attendre le dernier signal
	while (nb_mesures < nb_total_mesures)
		pause();
		
		
	return EXIT_SUCCESS;
}

