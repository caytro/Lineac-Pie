# Lineac Pie 
## Description

This C project allows you to generate pie charts and histograms from the command line. The data is read directly in the command line, or in a file in XML format (see in the demo folder)



The graph is saved in png format and can be viewed directly on the screen.
## Use

` pie [-o outputFileName] [-f inputFileName] [-t title] [-d] [-i] [-s size] [-b backgroundColor] [-3] [-h] label1 value1 label2 value2 ... `

If the -f option is present, the arguments label1 value1 ... will be ignored

## Options

	 -o		Save to file. Default output filename is pieChart.png
	 -f		Read data from file in XML format
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
	 -t		Chart Title. Default: My Chart.
	 -d		Display. Displays the graph on the screen.
	 -i		Histogram. By default the graph is a pie chart.
	 -s		Chart size in pixel. The -h option lets you know the minimum authorized size and the default size.
	 -b		Background color. Valid values ​​are white and gray. The default background color is black.
	 -3		3D pie chart.
	 -h		Help : Display this help page.

## Examples

	`pie -f demos/datas.xml -d  -o "demos/graph-Pie-bggray.png"  -s1024 -t "Mon Pie Chart Sur fond gris" -bgray`
	`pie  -d  -o "demos/graph-Pie-bggray.png"  -i -t "Mon Histogramme sur fond noir" Toulouse 25 Marseille 20 Lille 12 Rodez 14 "St Girons" 17`
	`pie -f demos/datas.xml -d  -3 -o "demos/graph-Pie-3D.png"  -s512 -t "Mon Pie Chart 3D sur fond noir" `


