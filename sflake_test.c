
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "snowflake.h"
#include "crystal_phase.h"
#include "contour.h"
#include "point.h"

//extern for all files
int size = 250;



/*  Arrays must be 2D contoured before being passed to a 3D contour
 *  2D contour is used to create 3D structure
 *  3D contour is used to find collisions and remove overlap
 * */

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
    point** pS = *parentS;
    int max = 512;
    pS = malloc(sizeof(point*) * max);

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
            printf("%d \n", max);
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


int main(){


    snowflake* a = initSnowflake(0, 0, 0, -1);
    srand(time(NULL));

    double* tmp = gen_crystal(0);
    double* tmpContour = malloc(sizeof(double) * size*size);
    contour2D(tmp, tmpContour, size/2, size/2);
    import2DArr(a, tmpContour, size);
 
    int fd;
    if((fd = open("zeroFileSoup.txt", O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IROTH)) ==-1){
        perror("Open Fail");
        exit(EXIT_FAILURE);
    }

   

    //how much to spread snowflakes for good looking bank


    point** pointSoup;//array of pointers to structs

    int cnt = getPointArr(&pointSoup, "out.0.txt");
    
    printf("Finished reading file\n");

    int rSize = 25;
    int* rotArr = randomRotArr(rSize); 
    int rIdx = 0;

    for(int i = 0; i < cnt; i++){
        free(a->voxelSpace);
        import2DArr(a, tmpContour, size);
        if(i%50 == 0){
            printf("Now at idx:%d/%d \n", i, cnt);
        }
        rIdx = (rand()%rSize) *4;
        setOrigin(a, pointSoup[i]->x, pointSoup[i]->y, pointSoup[i]->z);
        rotate(a, rotArr[rIdx], rotArr[rIdx+1], rotArr[rIdx+2], rotArr[rIdx+3] );
        printLocal(a, fd);
    }
    free(tmp);
    free(tmpContour);
}



