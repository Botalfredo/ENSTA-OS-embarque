#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NUM_RUNS 10
#define NUM_GOVERNORS 4

// Liste des gouverneurs à tester
const char *governors[NUM_GOVERNORS] = {
    "performance",
    "powersave",
    "ondemand",
    "conservative"
};

// Fonction pour calculer la moyenne
double calculate_mean(long long int *times, int num_runs) {
    double sum = 0.0;
    for (int i = 0; i < num_runs; i++) {
        sum += times[i];
    }
    return sum / num_runs;
}

// Fonction pour calculer l'écart type
double calculate_std_dev(long long int *times, int num_runs, double mean) {
    double variance = 0.0;
    for (int i = 0; i < num_runs; i++) {
        variance += pow(times[i] - mean, 2);
    }
    variance /= num_runs;
    return sqrt(variance);
}

// Fonction pour extraire le temps d'exécution depuis la sortie de measureWorkloadGovernor
long long extractExecutionTime(const char *command) {
    char buffer[256];
    FILE *fp;
    long long executionTime = -1;

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Erreur lors de l'exécution de la commande");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "Durée d'exécution :") != NULL) {
            sscanf(buffer, "Durée d'exécution : %lld microsecondes", &executionTime);
            break;
        }
    }

    pclose(fp);
    return executionTime;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <workload_path> <workload_params>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *workload_path = argv[1];  // Chemin de l'exécutable workload
    const char *workload_params = argv[2];  // Paramètres du workload

    printf("Starting workload execution with the following parameters:\n");
    printf("Workload path: %s\n", workload_path);
    printf("Workload parameters: %s\n\n", workload_params);

    long long int times[NUM_RUNS]; // Tableau pour stocker les temps d'exécution

    for (int g = 0; g < NUM_GOVERNORS; g++) {
        printf("Testing governor: %s\n", governors[g]);

        char filename[256];
        snprintf(filename, sizeof(filename), "results_%s.txt", governors[g]);

        FILE *outputFile = fopen(filename, "w");
        if (outputFile == NULL) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
            exit(EXIT_FAILURE);
        }

        fprintf(outputFile, "Run,ExecutionTime (microseconds)\n");

        for (int i = 0; i < NUM_RUNS; i++) {
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "./measureWorkloadGovernor %s %s %s", 
                     governors[g], workload_path, workload_params);

            printf("[DEBUG] Command to execute: %s\n", cmd);

            long long executionTime = extractExecutionTime(cmd);
            if (executionTime == -1) {
                fprintf(stderr, "Erreur lors de l'extraction du temps d'exécution pour le gouverneur %s, run %d\n", governors[g], i + 1);
                fclose(outputFile);
                exit(EXIT_FAILURE);
            }

            times[i] = executionTime;
            fprintf(outputFile, "%d,%lld\n", i + 1, times[i]);
            printf("Run %d: %lld microseconds\n", i + 1, times[i]);
        }

        double mean = calculate_mean(times, NUM_RUNS);
        double std_dev = calculate_std_dev(times, NUM_RUNS, mean);

        fprintf(outputFile, "Mean,%.2f\n", mean);
        fprintf(outputFile, "StandardDeviation,%.2f\n", std_dev);

        fclose(outputFile);

        printf("Governor: %s\n", governors[g]);
        printf("Mean execution time: %.2f microseconds\n", mean);
        printf("Standard deviation: %.2f microseconds\n", std_dev);
        printf("Results written to %s\n", filename);
        printf("-------------------------------\n");
    }

    printf("Workload testing completed successfully.\n");
    return EXIT_SUCCESS;
}
