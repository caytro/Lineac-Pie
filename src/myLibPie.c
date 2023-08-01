#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "myLibXML.h"
#include "gd.h"
#include "gdfontt.h"
#include "gdfonts.h"
#include "gdfontmb.h"
#include "gdfontl.h"
#include "gdfontg.h"

#define IMAGE_SIZE 712
#define H_TITRE 128
#define TYPE_PIE 1
#define TYPE_HISTO 2

typedef struct PieDataType{
    double valeur;
    char label[255];
    struct PieDataType *next;
} PieData;

typedef struct {
    char titre[255];
    int type;
    PieData *first;
} PieChart;



PieData *createPieData(char *label, double val){
    PieData *new = malloc(sizeof(PieData));
    strcpy(new->label,label);
    new->valeur = val;
    new->next = NULL;
    return new;
}

PieChart *newPieChart(char *titre)
{
    PieChart *new = malloc(sizeof(PieChart));
    strcpy(new->titre,titre);
    new->type = TYPE_PIE; // valeur par défaut
    new->first=NULL;
    return new;
}


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


PieChart *parseError(PieChart *pieChart, char *chaineLue,int ligne){
    printf("\n** Parse Error ligne %d\n",ligne);
    printf("Chaine Lue '%s'\n",chaineLue);
    return NULL;
}

PieChart *readDataFile(PieChart *pieChart,char *ficIn)
{
    printf("\nOuverture de %s...\n",ficIn);
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
    printf("\n...Ok\n");
    return pieChart;
}

void displayHelp(char *execName)
{
    printf("\n** Usage : %s [-o outputFileName] [-f inputFileName] [-t title] [-d] [-i] [-h] label1 valeur1 label2 valeur2 ...\n",execName);
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
    printf("\n");
}
