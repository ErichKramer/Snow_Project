//Snowflake.h (change to header)

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "stdio.h"

int size;

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
    s->neighborCollisions = NULL;
}

void setOrigin(snowflake* s, int x, int y, int z){
    s->originX = x;
    s->originY = y;
    s->originZ = z;
}

void setEllipses(snowflake* s, int x, int y, int z){

    s->eX = x; s->eY = y; s->eZ = z;

    s->xMin = s->originX - x;
    s->xMax = s->originX + x;

    s->yMin = s->originY - y;
    s->yMax = s->originY + y;   

    s->zMin = s->originZ - z;
    s->zMax = s->originZ + z;
}


//Finished implementation
int boxCollide(snowflake* a, snowflake* b){
    
    if( (a->xMax- b->xMin)*(a->xMin - b->xMax) < 0   ){
        if((a->yMax- b->yMin)*(a->yMin - b->yMax) < 0 ){
            if( (a->zMax- b->zMin)*(a->zMin - b->zMax) < 0 ){
                return 1;
            }
        }
    }
    return 0;

    //include methods for 
}



void printNeighbors(snowflake* s){

    int i;
    for( i = 0; i < s->neighSize; i++){
        

    }


}


void import2DArr(snowflake* s, double* arr, int size ){
//construct voxel shell from x,y cooords
    int centerPlane = size/2;//center plane of sflake
    printf("Entered the import func\n");
    s->voxelSpace = malloc(sizeof(double) * size*size*size);
    int z=0;
    printf("dSize: %f\n", dSize);
    for( int i = 0; i < size; i++){

        //formula for z value
        for(int j = 0; j < size; j++){

 //           printf(" arr[j+isize] = %f\n", arr[j+i*size]);
            if(z = arr[j+ i*size] ){
            //everything inbetween top and bottom = -1
                for(int n = 0; n < z-1; n++){

//                    printf("Internal For\n");
//                    s->voxelSpace[j+i*size + (n*size*size)/2] = -1; 
//                    s->voxelSpace[j+i*size - (n*size*size)/2] = -1; 
                }
                //add to the top and bottom of the shell
//change how this indexes

                s->voxelSpace[j+i*size-((z/2)*size*size)] = 1;//bottom
                s->voxelSpace[j+i*size+((z/2)*size*size)] = 1;//top
                printf("Unit: %f\n", s->voxelSpace[j+i*size+((z/2)*size*size)]);
            }
        }

    }
    printf("Outside of Import2D loops\n");

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







