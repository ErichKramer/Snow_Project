
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "snowflake.h"
#include "crystal_phase.h"
#include "contour.h"
#include "point.h"
#include "input.h"



//extern for all files, should not be a cmd argument
int size = 250;



/*  Arrays must be 2D contoured before being passed to a 3D contour
 *  2D contour is used to create 3D structure
 *  3D contour is used to find collisions and remove overlap
 * */


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

    point** pointSoup;//array of pointers to structs

    printf("Begin reading file\n");
    //cnt is the number of points we must work with, number of snowflakes
    int cnt = getPointArr(&pointSoup, "out.0.txt");
    
    printf("Finished reading file\n");

    int rSize = 25;
    int* rotArr = randomRotArr(rSize); 
    int rIdx = 0;
    printf("Beginning to build snowflakes\n");

    snowflake** killMyMemory = malloc(sizeof(snowflake*) * cnt);
    
    int blockSize = size*size*size * sizeof(double);
    int cubeSize  = size*size*size;
    //import2DArr(a, tmpContour, size);
    double* copy =  a->voxelSpace;
    double* sBlock;

   


    cnt = 10;
    for(int i = 0; i < cnt; i++){
        killMyMemory[i] = initSnowflake(pointSoup[i]->x, 
                           pointSoup[i]->y, pointSoup[i]->z, -1  ); 

        killMyMemory[i]->voxelSpace = malloc(blockSize);
        memcpy(killMyMemory[i]->voxelSpace, copy, blockSize);
        killMyMemory[i]->voxCubeLen = cubeSize;

        if(i%50 == 0){
            printf("Init: Now at idx:%d/%d \n", i, cnt);
        }
        rIdx = (rand()%rSize) *4;
        rotate(killMyMemory[i], rotArr[rIdx], rotArr[rIdx+1], rotArr[rIdx+2], rotArr[rIdx+3] );
    }

/*
        for(int n = 0; n< size*size; n++){
            for(int j = 0; j<size; j++){
                printf("%f,", killMyMemory[0]->voxelSpace[j + n*size ]);
            }
            printf("\n");

        }
*/


    printf("Finished constructing Snowflakes\n");

    for(int i = 0; i < cnt; i++){
        if(i%50 == 0){
            printf("Collision: Now at idx:%d/%d\n", i, cnt);
        }
        for(int j = i+1; j < cnt; j++){
            boxCollide(killMyMemory[i], killMyMemory[j]);

        }
    }
    for(int i = 0; i < cnt; i++){
        printLocal(killMyMemory[i], fd);

    }


    free(tmp);
    free(tmpContour);
}



