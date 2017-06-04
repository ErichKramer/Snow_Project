

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "snowflake.h"
#include "crystal_phase.h"
#include "contour.h"

//extern for all files
int size = 250;





/*  Arrays must be 2D contoured before being passed to a 3D contour
 *  2D contour is used to create 3D structure
 *  3D contour is used to find collisions and remove overlap
 * */


int main(){


    snowflake* a = initSnowflake(0, 0, 0, -1);

    FILE* fp = fopen("out.0.txt", "r");
    srand(time(NULL));


    double* tmp = gen_crystal(0);
    double* tmpContour = malloc(sizeof(double) * size*size);
    contour2D(tmp, tmpContour, size/2, size/2);
    import2DArr(a, tmpContour, size);
    
   

    const char delimit[2] = "\t";
    char buffer[256];
    double x,y,z;

    int scale = 200;

    for( int i = 0; i < 10; i++){

        printf("in loop\n");
        fgets(buffer, 256, (FILE*)fp);

        char* token = strtok(buffer, delimit);
        x = atof(token) *scale;
        y = atof(strtok(NULL, delimit)) *scale;
        z = atof(strtok(NULL, delimit)) *scale;

        printf("before set origin\n");
        setOrigin(a, x, y, z);
    
        rotate(a, rand()%180, rand()%10, rand()%10, rand()%10 );
        printf("After rotation\n");

        printLocal(a, "collision.txt");

        printf("after printLocal \n");

        free(a->voxelSpace);
        import2DArr(a, tmpContour, size);

    }



/*
    if(boxCollide(a, b)){
        printf("Collision\nPrinting to File...\n");
        printLocal(a, "collision.txt");

    }
*/

    
//    rotate(a, 90, 1, 0, 0);
//    rotate(a, 90, -1, 0, 0);
//    printLocal(a, "collision.txt");



    free(tmp);
    free(tmpContour);
}



