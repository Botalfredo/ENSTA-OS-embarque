// TP2_4c.c
// Compilation: gcc TP2_4c.c -o 4_c_analysis -lm
// Exécution: .TP2/TP2_4c.c < temsp.txt

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LG_LIGNE   1024

int main(int argc, char * argv[])
{
	// L'algorithme  implemente par ce programme est
	// presente sur la page i Wikipedia  "Algorithms
	// for Calculating Variance". Il  est  construit 
	// autour d'une idee de  Donald E. Knuth decrite
	// dans le second volume de "The Art of Computer
	// Programming".

	char ligne[LG_LIGNE];

	double mesure;
	int nb_mesures  = 0;
	double minimum;
	double maximum;
	double moyenne  =  0.0;
	double variance =  0.0;
	double delta;
	double ecart_type;

	while (fgets(ligne, LG_LIGNE, stdin) != NULL) {
		if (sscanf(ligne, "%lf", & mesure) != 1) {
			fprintf(stderr, "Erreur de lecture, ligne %d\n",
			        nb_mesures+1);
			exit(EXIT_FAILURE);
		}
		nb_mesures ++;
		if ((nb_mesures == 1) || (minimum > mesure))
			minimum = mesure;
		if ((nb_mesures == 1) || (maximum < mesure))
			maximum = mesure;
		delta = mesure - moyenne;
		moyenne  += delta / nb_mesures;
		variance += delta * (mesure - moyenne);
	}
	if (nb_mesures > 1)
		variance = variance / (nb_mesures - 1);

	ecart_type = sqrt(variance);

	printf(" Nb mesures = %d\n Minimum = %.0lf\n Maximum = %.0lf\n"
	       " Moyenne = %.0lf\n Ecart-type = %.0lf\n",
	        nb_mesures, minimum, maximum, moyenne, ecart_type);

	return EXIT_SUCCESS;
}

