#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include "vertex.h"

extern int size;

typedef struct snowflake snowflake;
struct snowflake{
    int idx;

    vertex** vertexSoup; //3d array of voxels describing shell
    int vertCount;     //dimensions of voxelSpace x*y*z

    double originX, originY, originZ;

    //integers to bound the extremes, used to calc eclippse
    float xMax, yMax, zMax;//qed
    float xMin, yMin, zMin;

    //variables for ellipsoid description
    //x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
    float eX, eY, eZ;

    //ellipsoid coefficents = 1 / {[(Max - Min)/2]^2} 

    snowflake** neighborCollisions;//set of nearby structs
    int neighSize;//num of elements in neighbor

};

//Snowflake Creation
snowflake* initSnowflake(int, int, int, int);


/*  Snowflake Operations  */
void setOrigin(snowflake*, int, int, int);
void setEllipses(snowflake*, int, int, int);
void combineGeom(snowflake*, snowflake*);
int boxCollide(snowflake*, snowflake*);
void import2DArr(snowflake*, double*, int);
void updateMaxMin(snowflake*);

/*  Logging functions(depend on snowflake)   */
void displayExtreme(snowflake*);
void printLocal(snowflake*, char*);
void write_file3D(int, snowflake*, int);
void write_file2D(int, double*, int);



#endif
