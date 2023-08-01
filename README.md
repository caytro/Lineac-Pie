# Lineac Pie 
## Description
Ce projet en C permet de générer des graphiques circulaires et des histogrammes en ligne de commandes. Les données sont lues directement dans la ligne de commande, ou bien dans un fichier au format XML (voir dans le dossier demo)

Le graphique est enregistré au format png et peut être visualisé directement à l'écran.
## Utilisation
` pie [-o outputFileName] [-f inputFileName] [-t title] [-d] [-i] [-s size] [-b backgroundColor] [-h] label1 valeur1 label2 valeur2 ... `

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
	 -t		Titre du graphique - Valeur par défaut : Mon Graphique
	 -d		Display : affiche le graphique
	 -i		Histogramme - Par défaut le graphique est de type pie (camembert)
	 -s		Taille de l'image en pixels. l'option -h permet de connaitre la taille minimale autorisée et la taille par défaut.
	 -b		Couleur du fond. Les valeurs autorisées sont white et gray. La couleur de fond par défaut est black.
	 -h		Help : Affiche cette page d'aide


