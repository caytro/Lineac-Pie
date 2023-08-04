#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "myLibXML.h"
#include "myLibPie.h"
#include "gd.h"
#include "gdfontt.h"
#include "gdfonts.h"
#include "gdfontmb.h"
#include "gdfontl.h"
#include "gdfontg.h"


/**
 * @brief createColor
 * @param nom
 * @param indice
 * @param red
 * @param green
 * @param blue
 * @return
 */


/***
 * Libération de la mémoire
 ***/

/**
 * @brief clearPieData
 * @param pieData
 */
void clearPieData(PieData *pieData){
    if(pieData->next != NULL)
    {
        clearPieData(pieData->next);

    }
    free(pieData);
}

/**
 * @brief clearPieChart
 * @param pieChart
 */
void clearPieChart(PieChart *pieChart)
{
    if (pieChart->first != NULL)
    {
        clearPieData(pieChart->first);
    }
    free(pieChart);
}

/**
 * @brief clearPalette
 * @param palette
 */
void clearPalette(Color **palette)
{
    for (int i=0;i < NB_COLOR_PALETTE; i++)
    {
        free(palette[i]);

    }
}

/**
 * @brief createColor
 * @param nom
 * @param indice
 * @param red
 * @param green
 * @param blue
 * @return Color *newColor
 */

Color *createColor(char *nom, int indice, int red, int green, int blue){
    Color *newColor = (Color *)malloc(sizeof(Color));
    strncpy(newColor->nom,nom,254);
    newColor->indice = indice;
    newColor->red = red;
    newColor->green = green;
    newColor->blue = blue;
    return newColor;
}

/**
 * @brief createPieData
 * @param label
 * @param val
 * @return PieData *newPieData
 */
PieData *createPieData(char *label, double val){
    PieData *new = (PieData *)malloc(sizeof(PieData));
    strcpy(new->label,label);
    new->valeur = val;
    new->next = NULL;
    return new;
}

/**
 * @brief newPieChart
 * @param titre
 * @return PieChart *pieChart
 */
PieChart *newPieChart(char *titre)
{
    PieChart *new = (PieChart *)malloc(sizeof(PieChart));
    strcpy(new->titre,titre);
    new->type = TYPE_PIE; // valeur par défaut
    new->first=NULL;
    return new;
}

/**
 * @brief appendPieData
 * @param pieChart
 * @param label
 * @param val
 * @return PieChart *updatedPieChart
 */
PieChart *appendPieData(PieChart *pieChart,char *label, double val)
{
    PieData *newPieData = createPieData(label,val);
    if (pieChart->first == NULL) pieChart->first = newPieData;
    else
    {
        PieData *curPieData = pieChart->first;
        while ((curPieData->next) !=NULL) curPieData = curPieData->next;
        curPieData->next = newPieData;
     }
    return pieChart;
}

/**
 * @brief getPieChartDataCount
 * @param pieChart
 * @return int PieChartDataCount
 */
int getPieChartDataCount(PieChart *pieChart)
{
    PieData *curPieData = pieChart->first;
    if (curPieData == NULL) return 0;
    int cpt=1;
    while(curPieData->next != NULL)
    {
        cpt++;
        curPieData=curPieData->next;
    }
    return cpt;
}

/**
 * @brief checkPieChartIntegrity
 * @param pieChart
 * @return 0 if OK. 1 if one value is <=0. 2 if empty
 */
int checkPieChartIntegrity(PieChart *pieChart)
{
    PieData *curPieData = pieChart->first;
    if (curPieData==NULL) return 2;
    while (curPieData !=NULL)
    {
        if (curPieData->valeur <=0 ) return 1; // non conforme
        curPieData = curPieData->next;
    }
    return 0;
}

/**
 * @brief calcRatioPourcent
 * @param pieChart
 * @return double ratio : coef. à appliquer pour avoir un total de 100 (pourcents)
 */
double calcRatioPourcent(PieChart *pieChart) //calcule le ratio à appliquer pour avoir un total de 100(pour cents)
{
    double total = 0.0;
    PieData *curPieData = pieChart->first;
    while (curPieData !=NULL)
    {
        total += curPieData->valeur;
        curPieData = curPieData->next;
    }
    return 100.0 / total;
}

/**
 * @brief getMaxPieChartValue
 * @param pieChart
 * @return double maxPieChartValue. Used to scale Hist
 */
double getMaxPieChartValue(PieChart *pieChart)
{
    PieData *curPieData = pieChart->first;
    double max=0;
    while (curPieData !=NULL)
    {
        if(curPieData->valeur > max) max = curPieData->valeur;
        curPieData = curPieData->next;
    }
    return max;
}

/**
 * @brief parseError
 * @param pieChart
 * @param chaineLue
 * @param ligne
 * @return PieChart* = NULL displays error message while parsing XML file
 */
PieChart *parseError(PieChart *pieChart, char *chaineLue,int ligne){
    printf("\n** Parse Error ligne %d\n",ligne);
    printf("Chaine Lue '%s'\n",chaineLue);
    return NULL;
}

/**
 * @brief readDataFile
 * @param pieChart
 * @param ficIn
 * @return PieChart *pieChart generated from data read in ficin
 */
PieChart *readDataFile(PieChart *pieChart,char *ficIn)
{
    FILE *fp = fopen(ficIn,"r");
    if (fp == NULL)
    {
        printf("\n*** Erreur lors de l'ouverture de %s\n",ficIn);
        return NULL;
    }
    char chaineLue[255];
    char label[255];
    char valeurStr[20];
    double valeur;
    int cpt=1;
    fgets(chaineLue,255,fp);
    if(strcmp(chaineLue,"<datas>\n") !=0 ) return parseError(pieChart, chaineLue,cpt);
    do{
        cpt++;
        if (openTagXML(fp,"data",1, chaineLue) != 0){
            if( strcmp(chaineLue,"</datas>\n" ) == 0) break; // Fin de parsing
            else return parseError(pieChart,chaineLue,cpt);
        }
        cpt++;
        if (parseXML(fp,"label",2,label,chaineLue) !=0) return parseError(pieChart,chaineLue,cpt);
        cpt+=3;
        if (parseXML(fp,"valeur",2,valeurStr,chaineLue) !=0) return parseError(pieChart,chaineLue,cpt);
        if (strtod(valeurStr,NULL) == 0) return parseError(pieChart,chaineLue,cpt);
        cpt+=3;
        if (closeTagXML(fp,"data",1, chaineLue)!=0) return parseError(pieChart,chaineLue,cpt);
        pieChart = appendPieData(pieChart,label,strtod(valeurStr,NULL));

    } while (strcmp(chaineLue,"</Datas>\n" ) !=0 );
    fclose(fp);
    return pieChart;
}

/**
 * @brief displayHelp
 * @param execName
 */

void displayHelp(char *execName)
{
    printf("\n** Usage : %s [-o outputFileName] [-f inputFileName] [-t title] [-d] [-i] [-s size] [-b bgColor] [-h] label1 valeur1 label2 valeur2 ...\n",execName);
    printf("\tSi l'option -f est présente, les arguments label1 valeur1 ... seront ignorés\n\n");
    printf("\t -o\t\tEnregistre dans le fichier. Défault : pieChart.png\n");
    printf("\t -f\t\tLit les données dans le fichier au format XML\n");
    printf("\t\t\t <datas>\n");
    printf("\t\t\t\t<data>\n");
    printf("\t\t\t\t\t<label>\n");
    printf("\t\t\t\t\t\tlabel1\n");
    printf("\t\t\t\t\t</label>\n");
    printf("\t\t\t\t\t<valeur>\n");
    printf("\t\t\t\t\t\tvaleur1\n");
    printf("\t\t\t\t\t</valeur>\n");
    printf("\t\t\t\t</data>\n");
    printf("\t\t\t\t...\n");
    printf("\t\t\t</datas>\n");
    printf("\t -t\t\tTitre du graphique\n");
    printf("\t -d\t\tDisplay : affiche le graphique\n");
    printf("\t -i\t\tHistogramme - Par défaut graphique de type pie (camembert)\n");
    printf("\t -s\t\tTaille de l'image en pixels. La taille ne doit pas être inférieure à %d. Taille par défaut : %d\n",MIN_IMAGE_SIZE, IMAGE_SIZE);
    printf("\t -b\t\tCouleur de fond. Les valeurs autorisées sont white et gray. La valeur par défaut est black\n");
    printf("\t -3\t\tGraphique en 3D\n");
    printf("\t -h\t\tHelp - Affiche cette page d'aide\n");

    printf("\n");
}
