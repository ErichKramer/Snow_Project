//Snowflake.h (change to header)
#ifndef SNOWFLAKE_C
#define SNOWFLAKE_C

#include "snowflake.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#include "contour.h"
#include "crystal_phase.h"
#include "vertex.h"

extern int size;




//initialize values based on params
snowflake* initSnowflake(int x, int y, int z, int idx){
    snowflake* s = malloc(sizeof(snowflake));
    s->originX = x;
    s->originY = y;
    s->originZ = z;
    s->idx = idx;

    s->vertCount = 0;
    s->neighSize = 0;
    s->vertexSoup = NULL;   
    s->neighborCollisions = malloc(sizeof(void*)* 32);

    s->xMax = 0;
    s->yMax = 0;
    s->zMax = 0;
    s->xMin = size;
    s->yMin = size;
    s->zMin = size;


}



void setOrigin(snowflake* s, int x, int y, int z){
    s->originX = x;
    s->originY = y;
    s->originZ = z;
}

//set the bounds
void setEllipses(snowflake* s, int x, int y, int z){

    s->eX = x; s->eY = y; s->eZ = z;

    s->xMin = s->originX - x;
    s->xMax = s->originX + x;

    s->yMin = s->originY - y;
    s->yMax = s->originY + y;   

    s->zMin = s->originZ - z;
    s->zMax = s->originZ + z;
}



/*Requires Re-write for new geometry representation*/
void combineGeom(snowflake* a, snowflake* b){

    int sizeCube    = size*size*size;

    //9 times for a 3x3x3 cube

    int xDiff = b->originX - a->originX;
    int yDiff = b->originY - a->originY;
    int zDiff = b->originZ - a->originZ;
    
    int vecDiff = xDiff + yDiff*size + zDiff*size*size;

    a->neighborCollisions[a->neighSize++] = b;
    b->neighborCollisions[b->neighSize++] = a;
}


//Finished implementation
int boxCollide(snowflake* a, snowflake* b){
    
    if( (a->xMax- b->xMin)*(a->xMin - b->xMax) < 0   ){
        if((a->yMax- b->yMin)*(a->yMin - b->yMax) < 0 ){
            if( (a->zMax- b->zMin)*(a->zMin - b->zMax) < 0 ){
                combineGeom(a, b);
                return 1;
            }
        }
    }
    return 0;

    //include methods for 
}





//construct voxel shell from x,y cooords
void import2DArr(snowflake* s, double* arr, int size ){


    //max of 2 at each x,y point
    s->vertexSoup = malloc(sizeof(vertex*) * 2*size*size);
    int z=0;
    int count = 0;

    for( int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){

            if(z = arr[j+ i*size] ){//there is a point in the contour bit
            
                s->vertexSoup[count++] = loadVal( j, i, z);
                s->vertexSoup[count++] = loadVal( j, i,-z);




                if( i < s->xMin) s->xMin = i;
                if( i > s->xMax) s->xMax = i;
                if( j < s->yMin) s->yMin = j;
                if( j > s->yMax) s->yMax = j;
                if( z > s->zMax) {
                    s->zMin = -z;
                    s->zMax =  z;
                }
            }
        }
    }

    s->xMin += s->originX;
    s->yMin += s->originY;
    s->zMin += s->originZ;

    s->xMax += s->originX;
    s->yMax += s->originY;
    s->zMax += s->originZ;
    
    s->vertCount = count;
}



void updateMaxMin(snowflake* s){
    assert(s->vertexSoup != NULL);



//this needs to be updated
}


/*************************************************
 *
 *LOGGING FUNCTIONS
 *
 ************************************************/


void displayExtreme(snowflake* s){

    printf("Min X: %f Max X: %f \n", s->xMin, s->xMax);
    printf("Min Y: %f Max Y: %f \n", s->yMin, s->yMax);
    printf("Min Z: %f Max Z: %f \n", s->zMin, s->zMax);


}


void printLocal(snowflake* s, char* file){

    int fd;
    if((fd = open(file, O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IROTH)) ==-1){
        perror("Open Fail");
        exit(EXIT_FAILURE);
    }

    printf("fd inside printLocal: %d\n", fd);

    int i;
    write_file3D(fd, s, size);//this is breaking

    for( i = 0; i < s->neighSize; i++){
        write_file3D(fd, s->neighborCollisions[i], size);
    }

    close(fd);
}


//write from a 3 space array into 3 space coordinates
void write_file3D(int fd,  snowflake* s, int lsize ){

    //this needs to be updated
    
//ToDo: divide by w?

    vertex** verts = s->vertexSoup;

    char buffer[100];
    char* bPoint = buffer;


    printf("Origins: %f, %f, %f\n", s->originX, s->originY, s->originZ );


    for(int i = 0; i < s->vertCount; i++){

        sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n",
            (float)verts[i]->x/10, (float)verts[i]->y/10,
            (float)verts[i]->z/10);

        if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
            perror("Write to file: ");
            exit(EXIT_FAILURE);
        }

    }

}


void write_file2D(int fd, double* geom, int lsize){
//this needs to be updated
    char buffer[100];
    char* bPoint = buffer;


    for( int y = 0; y < lsize; y++){
        for( int x = 0; x< lsize; x++){
            if(geom[x + y*lsize]){

                sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n",
                    (float)x/10, (float)y/10, geom[x + y*lsize]);

                if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                    perror("Write to file: ");
                    exit(EXIT_FAILURE);
                }

                sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n",
                    (float)x/10, (float)y/10, -geom[x + y*lsize]);

                if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                    perror("Write to file: ");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}







/*
Allow for input of a zed max so that we can scale along contours 
for this max value.

change extension to .out instead
*/



#endif



