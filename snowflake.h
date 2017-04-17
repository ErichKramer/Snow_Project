//Snowflake.h (change to header)

#include <stdlib.h>
#include <assert.h>


typedef struct snowflake snowflake;

struct snowflake{
    int idx;

    double* voxelSpace; //3d array of voxels describing shell
    int voxCubeLen;     //dimensions of voxelSpace x*y*z    

    int originX, originY;
       
    //integers to bound the extremes, used to calc eclippse
    int xMax, yMax, zMax;//qed
    int xMin, yMin, zMin;

    //variables for ellipsoid description
    //x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
    float eX, eY, eZ;
    

    snowflake** neighborCollisions;//set of nearby structs
    int nColSize;//num of elements in neighbor

};

//initialize values based on params
snowflake* initSnowflake(int x, int y, int idx){
    snowflake* s = malloc(sizeof(snowflake));
    s->originX = x;
    s->originY = y;
    s->idx = idx;

    s->voxCubeLen = 0;
    s->nColSize = 0;
    s->voxelSpace = NULL;   
    s->neighborCollisions = NULL;
}

void setOrigin(snowflake* s, int x, int y){
    s->originX = x;
    s->originY = y;
}

void setEllipses(){


}



snowflake* convertPlaneHeights(snowflake* s, double* arr, int size ){
//construct voxel shell from x,y cooords
    int centerPlane = size/2;//center plane of sflake
    s->voxelSpace = malloc(sizeof(double*size*size*size));
    for( int i = 0; i<size*size; i++){//access x = i%size, y = i/size
        int z = ceil(arr[i]/ 2.) ;//formula for z value
        for(int j = 0; j < size; j+){
            
            //UNFINISHED IMPLEMENTATION
        }

    }
}

void updateMaxMin(snowflake* s){
    assert(voxelSpace != NULL);
    int i;
    for( i = 0; i < s->voxCubeLen; i++){
    //for each plane scan for a valid z value, stop if found

    }

//flip a bool when going from hit to not hit

}

/*
Allow for input of a zed max so that we can scale along contours 
for this max value.

change extension to .out instead
*/







