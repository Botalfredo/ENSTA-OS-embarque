#! /bin/sh

if [ $# -lt 2 ]
then
	echo "usage: $0 <fichier-histogramme> <titre> [<sortie>]"
	exit 1
fi

if [ $# -eq 3 ]
then
	output="$3"
else
	output="|display png:-"
fi

gnuplot <<- EOF
	set terminal png size 800,600 
	set title "$2"
	set output '$output'
	# Echelle logarithmique en ordonnee
	set logscale y 10
	# Remplissage par rayures
	set style fill pattern 2
	# Afficher le contenu du fichier
	plot [] [0.1:] "$1" notitle with boxes
	# Attendre click de souris
	pause mouse
EOF
