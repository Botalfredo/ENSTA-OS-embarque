#!/bin/bash

# Liste des gouverneurs à tester
governors=("performance" "powersave" "ondemand" "conservative" "userspace")

# Chemin vers l'exécutable de measureWorkloadWithGovernor
measure_cmd="./measureWorkloadWithGovernor"

# Chemin vers le workload à tester (par exemple : primeNumbers, prodScal, writeFile)
workload="./primeNumbers"

# Paramètre d'entrée du workload (exemple : taille de calcul)
workload_param="100000"

# Nombre de répétitions
num_runs=10

# Fichier de résultats
results_file="results.txt"
echo "Governor,Mean(us),StdDev(us)" > $results_file

# Fonction pour calculer la moyenne et l'écart-type
calculate_stats() {
    local values=("$@")
    local sum=0
    local sum_sq=0
    local n=${#values[@]}

    for value in "${values[@]}"; do
        sum=$((sum + value))
        sum_sq=$((sum_sq + value * value))
    done

    local mean=$((sum / n))
    local variance=$((sum_sq / n - mean * mean))
    local stddev=$(echo "sqrt($variance)" | bc -l)

    echo "$mean $(printf "%.0f" $stddev)"
}

# Exécution des tests
for governor in "${governors[@]}"; do
    echo "Testing governor: $governor"
    runtimes=()

    for ((i = 1; i <= num_runs; i++)); do
        # Exécuter measureWorkloadWithGovernor et récupérer la durée
        runtime=$($measure_cmd $governor $workload $workload_param 2>/dev/null)
        echo "Run $i: $runtime µs"
        runtimes+=($runtime)
    done

    # Calcul de la moyenne et de l'écart-type
    stats=($(calculate_stats "${runtimes[@]}"))
    mean=${stats[0]}
    stddev=${stats[1]}

    echo "$governor,$mean,$stddev" >> $results_file
done

# Afficher les résultats
echo "Test completed. Results saved to $results_file."
cat $results_file
