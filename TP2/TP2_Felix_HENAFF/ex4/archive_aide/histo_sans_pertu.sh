#!/bin/sh

# Exécuter la première commande
echo "Exécution de ./4_b_aide 1000 > data_4_b.txt"
./4_b_aide 1000 > data_4_b.txt
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./4_b_aide"
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
echo "Exécution de ./4_d_histo 5000 0 1500 < data_4_b.txt > histo4_b.txt"
./4_d_histo 5000 0 1500 < data_4_b.txt > histo4_b.txt
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./4_d_histo"
    exit 1
fi

# Exécuter la quatrième commande
echo "Exécution de ./4_d_histosh.sh histo4_b.txt \"Histogramme sans perturbateur\""
./4_d_histosh.sh histo4_b.txt "Histogramme sans perturbateur"
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de ./4_d_histosh.sh"
    exit 1
fi

echo "Toutes les commandes ont été exécutées avec succès."
