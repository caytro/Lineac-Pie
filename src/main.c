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

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */

int main (int argc, char **argv)
{

    char *titreOpt = NULL;
    char *ficOutOpt = NULL;
    char *ficInOpt = NULL;
    char *bgColorOpt = NULL;
    int sOpt, sFlag = 0;
    int d3Opt = 0;
    char titre[255];
    char ficOut[255];
    int display = 0;
    int histo = 0;
    int help = 0;
    int imageSize = IMAGE_SIZE;
    int c, index;

    Color *myPalette[16];

    opterr = 0;
    // lecture des options -t titre -o outputfile -d (display) -f inputFile -h (help) -i (histogramme) -s (size) -b (backgroundColor)

    while( (c = getopt (argc, argv,"dt:o:f:his:b:3")) != -1)
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
            case 'b': // backgrounColor
                bgColorOpt = optarg;
            break;
            case '3':
                d3Opt = 1;
            break;
            case '?':
                if ((optopt == 't')||(optopt == 'o')||(optopt == 's')||(optopt == 'b'))
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

    // -h

    if (help)
    {
        displayHelp(argv[0]);
        return 0;
    }

    // -s

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

    // -t

    if (titreOpt) strncpy(titre,titreOpt,254); else strcpy(titre,"Mon graphique");
    if (ficOutOpt) strncpy(ficOut,ficOutOpt,254); else strcpy(ficOut,"pieChart.png");
    PieChart *pieChart = newPieChart(titre);


    // -i

    if (histo) pieChart->type = TYPE_HISTO;

    // -b

    if (bgColorOpt)
    {
        if (strcmp(bgColorOpt,"white")==0)
        {
            myPalette[0] = createColor("bgColor",0,255,255,255);
            myPalette[1] = createColor("fgColor",1,0,0,0);
        }
        else if (strcmp(bgColorOpt,"gray")==0)
        {
            myPalette[0] = createColor("bgColor",0,200,200,200);
            myPalette[1] = createColor("fgColor",1,0,0,0);
        }
        else
        {
            printf("\n** Erreur argument incorrect -b. Les valeurs possibles sont white ou gray\n");
            return 1;
        }
    }
    else //par defaut bg = black
    {
        myPalette[0] = createColor("bgColor",0,0,0,0);
        myPalette[1] = createColor("fgColor",1,255,255,255);
    }

    // -f

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

    // -3

    if (d3Opt ==1)
    {
        pieChart->type = TYPE_3D;  // prévoir un systeme de flags pour pie, histo 2D et 3D
    }

    // check datas

    if (checkPieChartIntegrity(pieChart) != 0){
        printf("\n** Erreur : Les datas contiennent des valeurs non conformes\n");
        return 1;
    }

    // Création de l'image
    gdImagePtr im = gdImageCreate(imageSize,imageSize+H_TITRE);


    // Colors
    myPalette[2]=createColor("rouge",2,255,0,0);
    myPalette[3]=createColor("vert",2,0,255,0);
    myPalette[4]=createColor("bleu",2,0,0,255);
    myPalette[5]=createColor("bleuClair",2,180,180,255);
    myPalette[6]=createColor("jaune",2,200,200,0);
    myPalette[7]=createColor("orange",2,255,128,0);
    myPalette[8]=createColor("violet",2,255,0,255);
    myPalette[9]=createColor("lightGray",2,200,200,200);
    myPalette[10]=createColor("darkGray",2,30,30,30);
    myPalette[11]=createColor("black",2,0,0,0);
    int gdColors[12];
    for (int colorIndex = 0; colorIndex<12; colorIndex++)
    {
        gdColors[colorIndex] = gdImageColorAllocate(im,myPalette[colorIndex]->red,myPalette[colorIndex]->green,myPalette[colorIndex]->blue);
    }
    int gdBgColor = gdColors[0];
    int gdFgColor = gdColors[1];
    int gdBlackColor = gdColors[11];

    // Fonts
    gdFontPtr fonts[5];
    fonts[0] = gdFontGetTiny ();
    fonts[1] = gdFontGetSmall ();
    fonts[2] = gdFontGetMediumBold ();
    fonts[3] = gdFontGetLarge ();
    fonts[4] = gdFontGetGiant ();

    // Titre
    gdImageString(im, fonts[4],(imageSize - strlen(titre) * 10) /2,H_TITRE/2 ,(unsigned char *)titre ,gdColors[1]);

    // focus sur la zone de dessin
    gdImageFilledRectangle(im,1,H_TITRE,imageSize,imageSize+H_TITRE,gdBgColor);


     // Pie

    if(pieChart->type == TYPE_PIE)
    {
        int xc = imageSize /2;
        int yc = (imageSize /2) + H_TITRE;
        double ratio = 0.6;
        int w = imageSize * ratio;
        int h = imageSize * ratio;
        int textRadius = w /2 * 1.2;
        int percentRadius = w * 0.4;
        double ratioAngle = calcRatioPourcent(pieChart);
        double curAngle = 0;
        char label[256];
        gdImageFilledEllipse(im, xc, yc, w+2,h+2,gdFgColor);
        PieData *curPieData = pieChart->first;
        int colorIndex=2;
        while(curPieData != NULL)
        {
            int s = (int) round(curAngle);
            int e = (int) round(curAngle  + curPieData->valeur * ratioAngle * 360.0 /100.0);
            gdImageFilledArc(im,xc,yc,w,h,(int)s,(int)e,gdColors[colorIndex],0);
            // label
            int xText = (int) (xc + textRadius * cos ( (e + s)/2 * M_PI/180 ));
            int yText = (int) (yc + textRadius * sin ( (e + s)/2 * M_PI/180 ));
            if((((e+s)/2)>90) && (((e+s)/2)<270)){  // partie gauche du camembert
                xText -= strlen((char *)curPieData->label)*8; // Décalage du texte vers la gauche pour avoir un affichage plus homogène
            }
            gdImageString(im, fonts[2],(int)xText,(int)yText,(unsigned char *)curPieData->label,gdColors[colorIndex]);

            //Pourcentages
            xText = (int) (xc + percentRadius * cos ( (e + s)/2 * M_PI/180 ));
            if((((e+s)/2)>270) || (((e+s)/2)<90)){  // partie droite du camembert
                xText -= 30; // Décalage du texte vers la gauche pour avoir un affichage plus homogène
            }
            yText = (int) (yc + percentRadius * sin ( (e + s)/2 * M_PI/180 ));
            sprintf(label,"%.2f%%",curPieData->valeur * ratioAngle);
            gdImageString(im, fonts[2],(int)xText,(int)yText,(unsigned char *)label ,gdBlackColor);
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
        gdImageLine(im,originX, originY,originX+graphicWidth,originY,gdFgColor);
        gdImageLine(im,originX,originY,originX,originY-graphicHeight,gdFgColor);
        PieData *curPieData = pieChart->first;
        int colorIndex=2;
        int rectStartX = originX+1;
        double yFactor = graphicHeight / maxVal;
        char label[20];
        while(curPieData != NULL)
        {
            int hauteur = (int) (yFactor * curPieData->valeur);
            gdImageFilledRectangle(im,rectStartX, originY-1,rectStartX + largeurRectangle, originY - hauteur ,gdColors[colorIndex]);
            gdImageStringUp(im, fonts[2],rectStartX + largeurRectangle /2, originY + strlen(curPieData->label) * 8,(unsigned char *)curPieData->label, gdColors[colorIndex]);
            sprintf(label,"%.2f",curPieData->valeur);
            gdImageString(im,fonts[2], rectStartX + largeurRectangle /2 - 16, originY - hauteur - marginTop /2,(unsigned char *)label, gdFgColor);
            curPieData =curPieData->next;
            rectStartX+=largeurRectangle;
            colorIndex++;
        }

    }
    else if (pieChart->type == TYPE_3D )
    {
        int xc = imageSize /2;
        int yc = (imageSize + H_TITRE) /2;
        double ratio = 0.5;
        double zRatio = 0.6;
        int w = imageSize * ratio;
        int h = imageSize * ratio * zRatio;
        int textRadius = w /2 * 1.4;
        int percentRadius = w * 0.4;
        double ratioAngle = calcRatioPourcent(pieChart);
        double curAngle = 0.0;
        char label[256];
        gdImageFilledEllipse(im, xc  , yc + 40, w+2,h +2,gdFgColor);
        PieData *curPieData = pieChart->first;
        int colorIndex=2;
        for(int offset =0; offset <10; offset ++)
        {
            PieData *curPieData = pieChart->first;
            int colorIndex=2;
            double curAngle = 0.0;

            while(curPieData != NULL) // le bas
            {
                double curAngle2 = curAngle  + curPieData->valeur * ratioAngle * 360.0 /100.0;
                gdImageFilledArc(im,xc ,yc + 40 - 4*offset,w,h,(int)curAngle,(int)curAngle2,gdColors[colorIndex],0);

                curAngle = curAngle2;
                colorIndex++;
                curPieData = curPieData->next;

            }
        }
        curPieData = pieChart->first;
        colorIndex=2;
        curAngle = 0.0;
        gdImageSetThickness(im,3);
        curAngle = 0.0;
        while(curPieData != NULL) // les montants
        {

            int x1 = xc + w *cos(curAngle * M_PI /180) / 2;
            int y1 = yc + h * sin (curAngle * M_PI /180) / 2;
            gdImageLine(im, x1,y1,x1,y1+40, gdBgColor);

            curAngle = curAngle + curPieData->valeur * ratioAngle * 360.0 /100.0;
            curPieData = curPieData->next;

        }
        curPieData = pieChart->first;
        colorIndex=2;
        gdImageFilledEllipse(im, xc , yc, w+2,h +2,gdFgColor);
        curAngle = 0.0;
        while(curPieData != NULL) //le haut
        {
            double curAngle2 =curAngle  + curPieData->valeur * ratioAngle * 360.0 /100.0;
            gdImageFilledArc(im,xc,yc,w,h,(int)curAngle, (int)curAngle2 , gdColors[colorIndex],0);
            // label
            int xText = (int) (xc + textRadius * cos ( (curAngle + curAngle2)/2 * M_PI/180 ));
            int yText = (int) (yc + textRadius * zRatio * sin ( (curAngle + curAngle2)/2 * M_PI/180 ));
            if((((curAngle + curAngle2)/2)>90) && (((curAngle + curAngle2)/2)<270)){  // partie gauche du camembert
                xText -= (int)strlen(curPieData->label)*8;

            }
            gdImageString(im, fonts[2],(int)xText,(int)yText,(unsigned char *)curPieData->label,gdColors[colorIndex]);

            //Pourcentages
            xText = (int) (xc + percentRadius * cos ( (curAngle + curAngle2)/2 * M_PI/180 ));
            if((((curAngle + curAngle2)/2)>270) || (((curAngle + curAngle2)/2)<90)){  // partie droite du camembert
                xText -= 30; // Décalage du texte vers la gauche pour avoir un affichage plus homogène
            }
            yText = (int) (yc + percentRadius * zRatio * sin ( (curAngle + curAngle2)/2 * M_PI/180 ));
            sprintf(label,"%.2f%%",curPieData->valeur * ratioAngle);
            gdImageString(im, fonts[2],(int)xText,(int)yText,(unsigned char *)label ,gdBlackColor);
            curAngle = curAngle2;
            colorIndex++;
            curPieData = curPieData->next;
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
