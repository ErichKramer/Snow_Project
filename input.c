#ifndef INPUT_C
#define INPUT_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "point.h"

void trimHeader(FILE* fp){
//if file has Header, trim it or move file pointer. Otherwise do noting

}



//Construct an array of point structs of all the described
//in the point cloud file

int getPointArr(point*** parentS, char* file){
    

/*current simulation has 61814 points.                              
* Do not attempt to dynamically resize. This is incredibly expensive
* Instead use a script to determine wc and pass as a param          
* (requires implementation of accepting that param)                 
*/
    int max = 512;
    point** pS = malloc(sizeof(point*) * max);

    FILE* fp = fopen(file, "r");
    int idx = 0;
    const char delimit[2] = "\t";
    int scale = 100;//multiply by 100, cast to int.

    //trimHeader(fp);

    char buffer[256];
    

    while(fgets(buffer, 256, (FILE*)fp)){  
        if(idx >= max){
            max *=2;
            point** tmp = malloc(sizeof(point*) *max);
            for(int i = 0; i < idx; i++){
                tmp[i] = pS[i];
            }
            free(pS);
            pS = tmp;
//            printf("%d \n", max);
        }

        pS[idx] = malloc(sizeof(point));

        char* token = strtok(buffer, delimit);
        pS[idx]->x   = atof(token) *scale;
        pS[idx]->y   = atof(strtok(NULL, delimit)) *scale;
        pS[idx]->z   = atof(strtok(NULL, delimit)) *scale;
        pS[idx]->sxx = atof(strtok(NULL, delimit)) ;
        pS[idx]->syy = atof(strtok(NULL, delimit)) ;
        pS[idx]->szz = atof(strtok(NULL, delimit)) ;
        pS[idx]->sxy = atof(strtok(NULL, delimit)) ;
        pS[idx]->sxz = atof(strtok(NULL, delimit)) ;
        pS[idx]->syz = atof(strtok(NULL, delimit)) ;
        pS[idx]->mat = atof(strtok(NULL, delimit)) ;
        pS[idx]->mass= atof(strtok(NULL, delimit)) ;
        idx++;
    }
    *parentS = pS;
    return idx;
}



int *randomRotArr(int count){

    int* tmp = malloc(sizeof(int*));
    tmp = malloc(sizeof(int) * count*4);

    for(int i = 0; i< count*4;){
        tmp[i++] = rand()%180;
        tmp[i++] = rand()%10;
        tmp[i++] = rand()%10;
        tmp[i++] = rand()%10;
    }
    return tmp;

}









#endif
