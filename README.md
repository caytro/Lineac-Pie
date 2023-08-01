# Lineac Pie 
## Description
Ce projet en C permet de générer des graphiques circulaires et des histogrammes en ligne de commandes. Les données sont lues directement dans la ligne de commande, ou bien dans un fichier au format XML (voir dans le dossier demo)

Le graphique est enregistré au format png et peut être visualisé directement à l'écran.
## Utilisation
` pie [-o outputFileName] [-f inputFileName] [-t title] [-d] [-i] [-h] label1 valeur1 label2 valeur2 ... `


Si l'option -f est présente, les arguments label1 valeur1 ... seront ignorés

	 -o		Enregistre dans le fichier. Défault : pieChart.png
	 -f		Lit les données dans le fichier au format XML
			 <datas>
				<data>
					<label>
						label1
					</label>
					<valeur>
						valeur1
					</valeur>
				</data>
				...
			</datas>
	 -t		Titre du graphique
	 -d		Display : affiche le graphique
	 -i		Histogramme - Par défaut graphique de type pie (camembert)


