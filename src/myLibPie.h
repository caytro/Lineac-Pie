#ifndef MYLIBPIE_H
#define MYLIBPIE_H

#endif // MYLIBPIE_H


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

#define IMAGE_SIZE 712  // dimension par défaut de la zone de graphique
#define MIN_IMAGE_SIZE 128  // dimension minimum en pixel de la zone de graphique autorisée avec l'option -s size
#define H_TITRE 80    // hauteur de la zone de titre en pixel
#define NB_COLOR_PALETTE 12 // nombre de couleurs dans la palette
#define TYPE_PIE 1
#define TYPE_HISTO 2
#define TYPE_3D 3

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

typedef struct {
    char nom[255];
    int indice;
    int red;
    int green;
    int blue;
} Color;


void clearPieData(PieData *pieData);

void clearPieChart(PieChart *pieChart);

void clearPalette(Color **palette);

/**
 * @brief createColor
 * @param nom
 * @param indice
 * @param red
 * @param green
 * @param blue
 * @return
 */
Color *createColor(char *nom, int indice, int red, int green, int blue);

/**
 * @brief createPieData
 * @param label
 * @param val
 * @return
 */
PieData *createPieData(char *label, double val);

/**
 * @brief newPieChart
 * @param titre
 * @return
 */
PieChart *newPieChart(char *titre);

/**
 * @brief appendPieData
 * @param pieChart
 * @param label
 * @param val
 * @return
 */
PieChart *appendPieData(PieChart *pieChart,char *label, double val);

int getPieChartDataCount(PieChart *pieChart);

int checkPieChartIntegrity(PieChart *pieChart);

double calcRatioPourcent(PieChart *pieChart); //calcule le ratio à appliquer pour avoir un total de 100(pour cents)

double getMaxPieChartValue(PieChart *pieChart);

PieChart *parseError(PieChart *pieChart, char *chaineLue,int ligne);

PieChart *readDataFile(PieChart *pieChart,char *ficIn);

void displayHelp(char *execName);
