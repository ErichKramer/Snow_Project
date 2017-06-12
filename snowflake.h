#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

extern int size;

typedef struct snowflake snowflake;
struct snowflake{
    int idx;

    double* voxelSpace; //3d array of voxels describing shell
    int voxCubeLen;     //dimensions of voxelSpace x*y*z

    double originX, originY, originZ;

    //integers to bound the extremes, used to calc eclippse
    double xMax, yMax, zMax;//qed
    double xMin, yMin, zMin;

    //variables for ellipsoid description??? deprecated
    //x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
    //double eX, eY, eZ;

    double sX, sY, sZ;

    //ellipsoid coefficents = 1 / {[(Max - Min)/2]^2} 

    snowflake** neighborCollisions;//set of nearby structs
    int neighSize;//num of elements in neighbor

};

//Snowflake Creation
snowflake* initSnowflake(int, int, int, int);


/*  Snowflake Operations  */
void setOrigin(snowflake*, double, double, double);
void setEllipses(snowflake*, int, int, int);
void combineGeom(snowflake*, snowflake*);
int  boxCollide(snowflake*, snowflake*);
void import2DArr(snowflake*, double*, int);
void updateMaxMin(snowflake*);


/*  Snowflake Translations  */
void rotate(snowflake*, double, double, double, double);
void translate(snowflake*, double, double, double);
void scale(snowflake*, double, double ,double);


/*rotate:       angle, x, y, z
 *translate:    x...
 *scale:        x...
 * */

/*  Logging functions(depend on snowflake)   */
void displayExtreme(snowflake*);
void printLocal(snowflake*, int);
void write_file3D(int, snowflake*, int);

/* Extraneous Logging*/
void write_file2D(int, double*, int);



#endif
