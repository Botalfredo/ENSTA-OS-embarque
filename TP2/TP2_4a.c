#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

// ./TP2_4 1000 (temps en microseconde)

static timer_t timer = (timer_t) 0;		// Identifiant de timer init à 0 (avec un cast de type timer)
static int nb_mesures = 0;				// Compteur du nombre de fois ou le timer à expirer
static int nb_total_mesures = 0;		// Nb tot de musre a faire avant la fin

void handler_signal(int inutilise){
	// fonction à exécuter à chaque expiration du timer
	// (handler_signal demande un argument pais ici pas besoin)
	nb_mesures++;
}


int main(int argc, char * argv[]){		// argc = nb de param, argv = tableau contenant les param
	long int periode;
	struct sigevent event;
	struct itimerspec spec;
	
	// On verifie que l'utilisateur à bien mis 1 argument
	// et scanf convertir l'argument en long que l'on stocj dans periode
	if ((argc != 2) || (sscanf(argv[1], "%ld", & periode) != 1)){
		fprintf(stderr, "\nusage:\n\n ./4_a_aide (periode_en_microsec)\n");
		exit(EXIT_FAILURE);
	}
	
	// nombre de mesues réalisées en 5 secondes (par exemple)
	nb_total_mesures = 5000000 / periode; // 5 secondes / perdiode micro sec
	
	// Configurer le timer
	// On lie le signal SIGRTMIN à la fonction handler_signal, de sorte que chaque 
	// fois que le signal SIGRTMIN est reçu, la fonction est appelée.
	signal(SIGRTMIN, handler_signal);
	
	//*** a compléter --> nous souhaitons être notifié par signal
    event.sigev_notify = SIGEV_SIGNAL;  // Méthode de notification d'expiration de timer par signal
    event.sigev_signo = SIGRTMIN;       // Définition di signal d'expiration du timer
	
	periode = periode * 1000;  // Conversion de la periode en nanosec car on ne prends pas les micro
	
	//*** à compléter --> définir la partie entière et fractionnaire
    spec.it_interval.tv_sec = 0; 		// La periode n'est pas en sec donc 0
    spec.it_interval.tv_nsec = periode; // Période en nanosec prélablement convertie
	spec.it_value = spec.it_interval; 	// Répétition immédiate après chaque itération du timer
	
	
	// Creation du timer avec CLOCK_MONOTONIC qui ne change pas pendant l'execution.
	// Event définit la méthode de notification et le type de signal, et timer
	// indique l'id de ce timer que l'on crée
    if (timer_create(CLOCK_MONOTONIC, &event, &timer) != 0) {
        perror("Echec de la création du timer");
        exit(EXIT_FAILURE);
    } 
	
	
	// Mise en place du time rpour le démarrer. spec indique les infos sir la periode
    if (timer_settime(timer, 0, &spec, NULL) != 0) {
        perror("Echec de la mise en place du timer");
        exit(EXIT_FAILURE);
    }
	
	// Attendre le dernier signal tant que les mesures n'ont pas atteint le total attendus
	while (nb_mesures < nb_total_mesures)
		pause();
		
	// Afficher le nombre de mesures prises
    printf("Nombre de mesures prises: %d\n", nb_mesures);

	return EXIT_SUCCESS;
}