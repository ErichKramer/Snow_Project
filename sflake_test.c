

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "snowflake.h"
#include "crystal_phase.h"
#include "contour.h"
#include "point.h"

//extern for all files
int size = 250;



/*  Arrays must be 2D contoured before being passed to a 3D contour
 *  2D contour is used to create 3D structure
 *  3D contour is used to find collisions and remove overlap
 * */

//Construct an array of point structs of all the described
//in the point cloud file

void getPointArr(point* pS, char* file){
    
/*current simulation has 61814 points.                              
* Do not attempt to dynamically resize. This is incredibly expensive
* Instead use a script to determine wc and pass as a param          
* (requires implementation of accepting that param)                 
*/
    FILE* fp = fopen(file, "r");
    int idx = 0;
    const char delmit[2] = "\t";
    int scale = 200;//temporary, change in future

    int pointCount = 65536;
    pS = malloc(sizeof(point) * pointCount);

    while(fgets(buffer, 256, (FILE*)fp)){

        
        char* token = strtok(buffer, delimit);
        pS[idx].x = atof(token) *scale;
        pS[idx].y = atof(strtok(NULL, delimit)) *scale;
        ps[idx].z = atof(strtok(NULL, delimit)) *scale;


        idx++;
    }

}


int main(){


    snowflake* a = initSnowflake(0, 0, 0, -1);

//    FILE* fp = fopen("out.0.txt", "r");
    srand(time(NULL));


    double* tmp = gen_crystal(0);
    double* tmpContour = malloc(sizeof(double) * size*size);
    contour2D(tmp, tmpContour, size/2, size/2);
    import2DArr(a, tmpContour, size);
    
   

    const char delimit[2] = "\t";
    char buffer[256];
    double x,y,z;


    int scale = 200;
    //how much to spread snowflakes for good looking bank



    /*
    for( int i = 0; i < 10; i++){

        printf("in loop\n");
        fgets(buffer, 256, (FILE*)fp);

        char* token = strtok(buffer, delimit);
        x = atof(token) *scale;
        y = atof(strtok(NULL, delimit)) *scale;
        z = atof(strtok(NULL, delimit)) *scale;

       }


     printf("before set origin\n");
        setOrigin(a, x, y, z);
    
        rotate(a, rand()%180, rand()%10, rand()%10, rand()%10 );
        printf("After rotation\n");

        printLocal(a, "collision.txt");

        printf("after printLocal \n");

        free(a->voxelSpace);
        import2DArr(a, tmpContour, size);





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



