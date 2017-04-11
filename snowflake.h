//Snowflake.h (change to header)

#include <stdlib.h>
#include <assert.h>


struct snowflake{
    int idx;

    double* voxelSpace; //3d array of voxels describing shell
    int voxCubeLen;     //dimensions of voxelSpace x*y*z    

    int originX, originY;
       
    //integers to bound the extremes, used to calc eclippse
    int xMax, yMax, zMax;//qed
    int xMin, yMin, zMin;

    snowflake** neighborCollisions;//set of nearby structs
    int nColSize;//num of elements in neighbor
};

//initialize values based on params
snowflake* initSnowflake(int x, int y, int idx){
    snowflake* s = malloc(sizeof(snowflake));
    s->originX = x;
    s->originY = y;
    s->idx = idx;

    voxCubeLen = 0;
    nColSize = 0;
    voxelSpace = NULL;   
    neighborCollisions = NULL;
}

void setOrigin(snowflake* s, int x, int y){
    s->originX = x;
    s->originY = y;
}


snowflake* convertPlaneHeights(snowflake* s, double* arr, int size ){
//construct voxel shell from x,y cooords
    int centerPlane = size/2;//center plane of sflake
    s->voxelSpace = malloc(sizeof(double*size*size*size));
    for( int i = 0; i<size*size; i++){//access x = i%size, y = i/size
        int z = ceil(arr[i]/ 2.) ;//formula for z value
        for(int j = 0; j < size; j+){
            
            
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




/*
	Need a struct representation of snowflake:
		Index Identifier?   (memory)
		3d array of coordinates, 
		localized origin(center)    (scene)
[array]	Mximum and minimum values of each x, y, and z
        
	void (*function) int??
[array]	a,b,c coefficients for the ellipse

	array of pointers to neighbors? or integers of indecies in the array

// What about a three-way collision?


For collisions it is okay to lose information because neighbors are stored

if we are updating ellipses we are doing something wrong

*/
/*
Other:
{
Simple Z values - Pyramid

Crystal growth papers- Read 2
Hexagonal shapes paper (email available)

Find out details about the coefficients. Is it possible to create other shapes


Watch rendering series

Install Mitsuba

}

comment everything, touch up code
read papers, get snowflake structures working fully.
Fix Zed Value, email to paris?

So that Paris can understand
*/




