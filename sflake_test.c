

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "snowflake.h"
#include "crystal_phase.h"
#include "contour.h"

//extern for all files

//this is a dependency for everything
int size = 250;





/*  Arrays must be 2D contoured before being passed to a 3D contour
 *  2D contour is used to create 3D structure
 *  3D contour is used to find collisions and remove overlap
 * */




int main(){


    snowflake* a = initSnowflake(0, 0, 0, -1);
//    snowflake* b = initSnowflake(50, 0, 0, -1);

    double* tmp = gen_crystal(0);
    double* tmpContour = malloc(sizeof(double) * size*size);
    contour2D(tmp, tmpContour, size/2, size/2);
    import2DArr(a, tmpContour, size);


//    free(tmp);
//    rotate(a, 90, 1, 1, 1);
 //   scale(a, 5);

    const char delimit[2] = "\t";
    double x,y,z;
    
    char buffer[256];
    FILE* fp = fopen("out.0.txt", "r");
    srand(time(NULL));

    for( int i = 0; i < 50; i++){
        fgets(buffer, 256, (FILE*)fp);

        char* token = strtok(buffer, delimit);
        x = atof(token);
        y = atof(strtok(NULL, delimit));
        z = atof(strtok(NULL, delimit));

        setOrigin(a, x, y, z);
        rotate(a, rand()%180, rand()%10, rand()%10, rand()%10);
        printLocal(a, "collision.txt");
    }
/*
    if(boxCollide(a, b)){
        printf("Collision\nPrinting to File...\n");
        printLocal(a, "collision.txt");
    }
*/
    free(tmp);
    free(tmpContour);
}



