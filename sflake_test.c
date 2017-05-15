
#include "snowflake.h"


#include <stdio.h>




/*  Arrays must be 2D contoured before being passed to a 3D contour
 *  2D contour is used to create 3D structure
 *  3D contour is used to find collisions and remove overlap
 * */

int main(){

    snowflake* a = initSnowflake(0, 0, 0, -1);
    snowflake* b = initSnowflake(50, 50, 0, -1);

    double* tmp = gen_crystal(0);
    double* tmpContour = malloc(sizeof(double) * size*size);
    contour2D(tmp, tmpContour, size/2, size/2);
    import2DArr(a, tmpContour, size);
    
    free(tmp);

    memset(tmpContour, 0, size*size*sizeof(int));
    tmp = gen_crystal(0);
    contour2D(tmp, tmpContour, size/2, size/2);

    import2DArr(b, tmpContour, size);


    printLocal(a, "first.txt");


    if(boxCollide(a, b)){
        printf("Collision\nPrinting to File...\n");
        printLocal(a, "collision.txt");
    }

    free(tmp);
    free(tmpContour);
}



