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

#define IMAGE_SIZE 712
#define MIN_IMAGE_SIZE 128
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



PieData *createPieData(char *label, double val);

PieChart *newPieChart(char *titre);

PieChart *appendPieData(PieChart *pieChart,char *label, double val);

int getPieChartDataCount(PieChart *pieChart);

int checkPieChartIntegrity(PieChart *pieChart);

double calcRatioPourcent(PieChart *pieChart); //calcule le ratio à appliquer pour avoir un total de 100(pour cents)

double getMaxPieChartValue(PieChart *pieChart);

PieChart *parseError(PieChart *pieChart, char *chaineLue,int ligne);

PieChart *readDataFile(PieChart *pieChart,char *ficIn);

void displayHelp(char *execName);
