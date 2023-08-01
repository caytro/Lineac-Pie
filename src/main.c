#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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


int main (int argc, char **argv)
{

    char *titreOpt = NULL;
    char *ficOutOpt = NULL;
    char *ficInOpt = NULL;
    int sOpt, sFlag = 0;
    char titre[255];
    char ficOut[255];
    int display = 0;
    int histo = 0;
    int help = 0;
    int imageSize = IMAGE_SIZE;
    int c, index;

    opterr = 0;
    // lecture des options -t titre -o outputfile -d (display) -f inputFile -h (help) -i (histogramme)

    while( (c = getopt (argc, argv,"dt:o:f:his:")) != -1)
    {
        switch ((char)c)
        {
            case 'd' : // display graph
                display = 1;
            break;
            case 't' : // titre
                titreOpt = optarg;
            break;
            case 'o' : // outputFileName
                ficOutOpt = optarg;
            break;
            case 'f': // inputFileName (XML)
                ficInOpt = optarg;
            break;
            case 'i': // histogramme
                histo = 1;
            break;
            case 'h': // help
                help = 1;
            break;
            case 's': // size
                sOpt = atoi(optarg);
                sFlag = 1;
            break;
            case '?':
                if ((optopt == 't')||(optopt == 'o')||(optopt == 's'))
                      fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                  fprintf (stderr,
                           "Unknown option character `\\x%x'.\n",
                           optopt);
                return 1;
          default:
            abort ();
        }

    }
    if (help)
    {
        displayHelp(argv[0]);
        return 0;
    }
    if (sFlag == 1 )
    {
        if(sOpt >= MIN_IMAGE_SIZE)
        {
            imageSize = sOpt;
        }
        else
        {
            printf("\n** Parametre incorrect -s : la taille de l'image doit être supérieure ou égale à %d\n",MIN_IMAGE_SIZE);
            return 1;
        }
    }
    if (titreOpt) strncpy(titre,titreOpt,254); else strcpy(titre,"Mon graphique");
    if (ficOutOpt) strncpy(ficOut,ficOutOpt,254); else strcpy(ficOut,"pieChart.png");
    PieChart *pieChart = newPieChart(titre);
    if (histo) pieChart->type = TYPE_HISTO;

    if (ficInOpt) // lecture des datas dans fichier formaté xml
    {
        pieChart = readDataFile(pieChart,ficInOpt);
        if(pieChart == NULL) return 1;
    }
    else // lecture des datas dans la ligne de commande
    {
        if(argc - optind<3){
            printf("\nUsage : %s -t titre -o output filename label valeur [label valeur ...]\n", argv[0]);
            return 1;
        }

        for (int i=optind;i<argc ;i+=2)
        {
             if (strtod(argv[i+1],NULL) ==  0)
            {
                printf("\nParse error : param %d\n",i);
                printf("\nUsage : %s titre label valeur [...]\n",argv[0]);
                return 1;
            }
            pieChart= appendPieData(pieChart,argv[i],strtod(argv[i+1],NULL));

        }
    }

    if (checkPieChartIntegrity(pieChart) != 0){
        printf("\n** Erreur : Les datas contiennent des valeurs non conformes\n");
        return 1;
    }

    // Création de l'image
    gdImagePtr im = gdImageCreate(imageSize,imageSize+H_TITRE);


    // Colors

    int colors[16];
    int colorBlack=gdImageColorAllocate(im,0,0,0);
    int colorRed=gdImageColorAllocate(im,255,0,0);
    int colorGreen=gdImageColorAllocate(im,0,255,0);
    int colorBlue=gdImageColorAllocate(im,80,80,255);
    int colorWhite=gdImageColorAllocate(im,255,255,255);
    int colorYellow=gdImageColorAllocate(im,240,240,0);
    int colorOrange=gdImageColorAllocate(im,255,128,0);
    int colorViolet=gdImageColorAllocate(im,255,0,255);
    int colorGray=gdImageColorAllocate(im,200,200,200);
    int colorDarkGray=gdImageColorAllocate(im,30,30,30);
    int indiceColor = 0;
    colors[indiceColor++]=colorBlack;
    colors[indiceColor++]=colorRed;
    colors[indiceColor++]=colorGreen;
    colors[indiceColor++]=colorBlue;
    colors[indiceColor++]=colorWhite;
    colors[indiceColor++]=colorYellow;
    colors[indiceColor++]=colorOrange;
    colors[indiceColor++]=colorViolet;
    colors[indiceColor++]=colorGray;
    colors[indiceColor++]=colorDarkGray;


    // Fonts
    gdFontPtr fonts[5];
    fonts[0] = gdFontGetTiny ();
    fonts[1] = gdFontGetSmall ();
    fonts[2] = gdFontGetMediumBold ();
    fonts[3] = gdFontGetLarge ();
    fonts[4] = gdFontGetGiant ();

    // Titre
    gdImageString(im, fonts[4],(imageSize - strlen(titre) * 10) /2,H_TITRE/2 ,(unsigned char *)titre ,colorWhite);

    // focus sur la zone de dessin
    gdImageFilledRectangle(im,1,H_TITRE,imageSize,imageSize+H_TITRE,colorDarkGray);


     // Pie

    if(pieChart->type == TYPE_PIE)
    {
        int xc = imageSize /2;
        int yc = (imageSize + H_TITRE) /2;
        double ratio = 0.5;
        int w = imageSize * ratio;
        int h = imageSize * ratio;
        int textRadius = w /2 * 1.2;
        int percentRadius = w * 0.4;
        double ratioAngle = calcRatioPourcent(pieChart);
        double curAngle = 0;
        char label[256];
        PieData *curPieData = pieChart->first;
        int colorIndex=1;
        while(curPieData != NULL)
        {
            int s = (int) round(curAngle);
            int e = (int) round(curAngle  + curPieData->valeur * ratioAngle * 360.0 /100.0);
            gdImageFilledArc(im,xc,yc,w,h,(int)s,(int)e,colors[colorIndex],0);
            // label
            int xText = (int) (xc + textRadius * cos ( (e + s)/2 * M_PI/180 ));
            int yText = (int) (yc + textRadius * sin ( (e + s)/2 * M_PI/180 ));
            if((((e+s)/2)>90) && (((e+s)/2)<270)){  // partie gauche du camembert
                xText -= strlen((char *)curPieData->label)*8; // Décalage du texte vers la gauche pour avoir un affichage plus homogène
            }
            gdImageString(im, fonts[2],(int)xText,(int)yText,(unsigned char *)curPieData->label,colors[colorIndex]);

            //Pourcentages
            xText = (int) (xc + percentRadius * cos ( (e + s)/2 * M_PI/180 ));
            if((((e+s)/2)>270) || (((e+s)/2)<90)){  // partie droite du camembert
                xText -= 30; // Décalage du texte vers la gauche pour avoir un affichage plus homogène
            }
            yText = (int) (yc + percentRadius * sin ( (e + s)/2 * M_PI/180 ));
            sprintf(label,"%.2f%%",curPieData->valeur * ratioAngle);
            gdImageString(im, fonts[2],(int)xText,(int)yText,(unsigned char *)label ,colorBlack);
            curAngle = (double) e;
            colorIndex++;
            curPieData = curPieData->next;

        }
    }
    // Histo
    else if (pieChart->type == TYPE_HISTO)
    {
        double maxVal= getMaxPieChartValue(pieChart);
        int nbDatas = getPieChartDataCount(pieChart);
        if(nbDatas ==0) return 1;
        int marginH = imageSize * 0.1; // margin left et margin right
        int marginTop = imageSize * 0.1;
        int textZoneHeight = 200; // hauteur de l'espace destiné à écrire les labels
        int graphicWidth = imageSize - 2 * marginH;
        int graphicHeight = imageSize - marginTop - textZoneHeight;
        int originY = H_TITRE + graphicHeight + marginTop;
        int originX = marginH;
        int largeurRectangle = graphicWidth / nbDatas;
        gdImageLine(im,originX, originY,originX+graphicWidth,originY,colorWhite);
        gdImageLine(im,originX,originY,originX,originY-graphicHeight,colorWhite);
        PieData *curPieData = pieChart->first;
        int colorIndex=1;
        int rectStartX = originX+1;
        double yFactor = graphicHeight / maxVal;
        char label[20];
        while(curPieData != NULL)
        {
            int hauteur = (int) (yFactor * curPieData->valeur);
            gdImageFilledRectangle(im,rectStartX, originY-1,rectStartX + largeurRectangle, originY - hauteur ,colors[colorIndex]);
            gdImageStringUp(im, fonts[2],rectStartX + largeurRectangle /2, originY + strlen(curPieData->label) * 8,(unsigned char *)curPieData->label, colors[colorIndex]);
            sprintf(label,"%.2f",curPieData->valeur);
            gdImageString(im,fonts[2], rectStartX + largeurRectangle /2, originY - hauteur - marginTop /2,(unsigned char *)label, colorWhite);
            curPieData =curPieData->next;
            rectStartX+=largeurRectangle;
            colorIndex++;
        }

    }


    // enregistrement
    FILE *out = fopen(ficOut,"wb");
    gdImagePng(im,out);
    fclose(out);
    if (display)
    {
        char commande[300];
        sprintf(commande, "display %s &",ficOut);
        int status = system(commande);
        return status;
     }
    return 0;
}
