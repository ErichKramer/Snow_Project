//Snowflake.h (change to header)

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "contour.h"
#include "crystal_phase.h"


extern int size;

typedef struct snowflake snowflake;

struct snowflake{
    int idx;

    double* voxelSpace; //3d array of voxels describing shell
    int voxCubeLen;     //dimensions of voxelSpace x*y*z    

    int originX, originY, originZ;
       
    //integers to bound the extremes, used to calc eclippse
    float xMax, yMax, zMax;//qed
    float xMin, yMin, zMin;

    //variables for ellipsoid description
    //x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
    float eX, eY, eZ;
    

    snowflake** neighborCollisions;//set of nearby structs
    int neighSize;//num of elements in neighbor

};

//initialize values based on params
snowflake* initSnowflake(int x, int y, int z, int idx){
    snowflake* s = malloc(sizeof(snowflake));
    s->originX = x;
    s->originY = y;
    s->originZ = z;
    s->idx = idx;

    s->voxCubeLen = 0;
    s->neighSize = 0;
    s->voxelSpace = NULL;   
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
    write_file3D(fd, s->voxelSpace, size);//this is breaking

    for( i = 0; i < s->neighSize; i++){
        write_file3D(fd, s->neighborCollisions[i]->voxelSpace, size);
    }

    close(fd);
}



void combineGeom(snowflake* a, snowflake* b){

/*  Put it into a Giant 3d array
 *  use contour3d on that array
 *  remove points > 1
 *
 * */
    int sizeThree   = 3*size; //prevent extraneous comparison calcs.
    int sizeTwo     = 2*size;
    int sizeCube    = size*size*size;

    //9 times for a 3x3x3 cube

    int xDiff = b->originX - a->originX;
    int yDiff = b->originY - a->originY;
    int zDiff = b->originZ - a->originZ;
    
    int vecDiff = xDiff + yDiff*size + zDiff*size*size;


    for(int i = 0; i < sizeCube; i++){
        if(vecDiff+i > 0 && vecDiff+i < sizeCube){

            if(b->voxelSpace[i]){
                if(b->voxelSpace[i] == -1){
                    a->voxelSpace[i+vecDiff] = -1;
                }
//                else if (a->voxelSpace[i+vecDiff] != -1){
//                    a->voxelSpace[i+vecDiff] = 1;
//                }
            }
            //if b has -1 it was already written to a and this is redundant
            if(a->voxelSpace[i+vecDiff]==-1){
                b->voxelSpace[i] = a->voxelSpace[i+vecDiff];
            }
            
        }
    }

   
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


    s->voxelSpace = malloc(sizeof(double) * size*size*size);
    int z=0;
    int idx;
    int cubeSize = size*size; //prevent excessive mul, used for z
    int centerPlane = size/2;//center plane of sflake

    for( int i = 0; i < size; i++){

        //formula for z value
        for(int j = 0; j < size; j++){

            if(z = arr[j+ i*size] ){
                for(int n = 0; n < z-1; n++){
                    s->voxelSpace[j + i*size + (centerPlane +(int)n/2)*cubeSize ]=-1;
                    s->voxelSpace[j + i*size + (centerPlane - (int)n/2)*cubeSize ]=-1;
                }
                s->voxelSpace[j + i*size + (centerPlane +z/2)*cubeSize ]=1;
                s->voxelSpace[j + i*size + (centerPlane -z/2)*cubeSize ]=1;

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
    
    s->voxCubeLen = size;
}

void updateMaxMin(snowflake* s){
    assert(s->voxelSpace != NULL);
    int x,y,z;
    
    int yMax, xMax, zMax = 0;
    for( z = 0; z  < s->voxCubeLen; z++){
        for( y = 0; y < s->voxCubeLen; y++){
            for(x = 0; x < s->voxCubeLen; x++){
                if(s->voxelSpace[z*size*size + y*size + x]){
                    if(x > xMax){
                        xMax = x;
                    }
                    if(y > yMax){
                        yMax = y;
                    }
                    if(z > zMax){
                        zMax = z;
                    }

                }
            }
        }


    }
    //for each plane scan for a valid z value, stop if found
        
//flip a bool when going from hit to not hit
}

/*
Allow for input of a zed max so that we can scale along contours 
for this max value.

change extension to .out instead
*/







