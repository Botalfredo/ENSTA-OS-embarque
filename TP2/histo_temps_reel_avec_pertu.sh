#!/bin/sh

# Lancer 4_b_aide et 4_e_pert en parallèle
echo "Exécution de ./ex5 1000 > data_4_b.txt et ./4_e_pert en parallèle"
sudo ./ex5 1000 > data_4_b.txt &  # Exécute ex5 en arrière-plan
PID_5=$!  # Sauvegarde le PID de 4_b_aide pour attendre plus tard
./4_e_pert &  # Exécute 4_e_pert en arrière-plan
PID_4_E_PERT=$!  # Sauvegarde le PID de 4_e_pert pour attendre plus tard

# Attendre que 4_b_aide et 4_e_pert se terminent
wait $PID_5
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./ex5"
    exit 1
fi

wait $PID_4_E_PERT
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./4_e_pert"
    exit 1
fi

# Exécuter la deuxième commande
echo "Exécution de ./4_c_analysis < data_4_b.txt"
./4_c_analysis < data_4_b.txt
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./4_c_analysis"
    exit 1
fi

# Exécuter la troisième commande
echo "Exécution de ./4_d_histo 5000 0 1141 < data_4_b.txt > histo4_b.txt"
./4_d_histo 5000 0 1141 < data_4_b.txt > histo4_b.txt
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./4_d_histo"
    exit 1
fi

# Exécuter la quatrième commande
echo "Exécution de ./4_d_histosh.sh histo4_b.txt \"Histogramme avec perturbateur temps réel\""
./4_d_histosh.sh histo4_b.txt "Histogramme avec perturbateur temps réel"
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./4_d_histosh.sh"
    exit 1
fi

echo "Toutes les commandes ont été exécutées avec succès."