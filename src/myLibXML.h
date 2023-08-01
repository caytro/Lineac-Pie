#ifndef MYLIBXML_H
#define MYLIBXML_H

#endif // MYLIBXML_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int openTagXML(FILE *fp, char *nomChamp, int level,char *chaineRetour);

int closeTagXML(FILE *fp, char *nomChamp, int level, char *chaineLue);

int parseXML(FILE *fp,char *nomChamp, int level, char *result, char *chaineLue);

