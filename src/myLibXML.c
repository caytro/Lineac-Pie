#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int openTagXML(FILE *fp, char *nomChamp, int level,char *chaineRetour)
{
    char chaineLue[255]="";
    char chaineCmp[255]="";
    char tabs[10]="";
    for (int i=0;i<level;i++){
        strcat(tabs,"\t");
    }

    fgets(chaineLue,255,fp);
    sprintf(chaineCmp,"%s<%s>\n",tabs,nomChamp);
    strcpy(chaineRetour,chaineLue);
    if(strcmp(chaineLue,chaineCmp)!=0) return 1;
    return 0;
}

int closeTagXML(FILE *fp, char *nomChamp, int level, char *chaineLue)
{
    char chaineCmp[255]="";
    char tabs[10]="";
    for (int i=0;i<level;i++){
        strcat(tabs,"\t");
    }

    fgets(chaineLue,255,fp);
    sprintf(chaineCmp,"%s</%s>\n",tabs,nomChamp);
    if(strcmp(chaineLue,chaineCmp)!=0) return 1;
    return 0;
}

int parseXML(FILE *fp,char *nomChamp, int level, char *result, char *chaineLue){
    char chaineCmp[255]="";
    char tabs[10]="";
    for (int i=0;i<level;i++){
        strcat(tabs,"\t");
    }

    fgets(chaineLue,255,fp);
    sprintf(chaineCmp,"%s<%s>\n",tabs,nomChamp);
    if(strcmp(chaineLue,chaineCmp)!=0) return 1;
    fgets(chaineLue,255,fp);
    strncpy(result,chaineLue+level+1, strlen(chaineLue)-level-1 );
    result[strlen(chaineLue)-level-2]='\0';
    fgets(chaineLue,255,fp);
    sprintf(chaineCmp,"%s</%s>\n",tabs,nomChamp);
    if(strcmp(chaineLue,chaineCmp)!=0) return 1;


    return 0;
}
