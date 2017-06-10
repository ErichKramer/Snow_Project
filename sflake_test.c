
#include <fcntl.h>
#include <unistd.h>
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

void trimHeader(FILE* fp){
//if file has Header, trim it or move file pointer. Otherwise do noting

}



//Construct an array of point structs of all the described
//in the point cloud file

int getPointArr(point** pS, char* file){
    

/*current simulation has 61814 points.                              
* Do not attempt to dynamically resize. This is incredibly expensive
* Instead use a script to determine wc and pass as a param          
* (requires implementation of accepting that param)                 
*/
    FILE* fp = fopen(file, "r");
    int idx = 0;
    const char delimit[2] = "\t";
    int scale = 100;//multiply by 100, cast to int.

    //trimHeader(fp);

    char buffer[256];
    

    while(fgets(buffer, 256, (FILE*)fp)){  

        printf("idx is %d", idx); 
        pS[idx] = malloc(sizeof(point));

        char* token = strtok(buffer, delimit);
        pS[idx]->x   = atof(token) *scale;
        pS[idx]->y   = atof(strtok(NULL, delimit)) *scale;
        pS[idx]->z   = atof(strtok(NULL, delimit)) *scale;
        pS[idx]->sxx = atof(strtok(NULL, delimit)) ;
        pS[idx]->syy = atof(strtok(NULL, delimit)) ;
        pS[idx]->szz = atof(strtok(NULL, delimit)) ;
        pS[idx]->sxy = atof(strtok(NULL, delimit)) ;
        pS[idx]->sxz = atof(strtok(NULL, delimit)) ;
        pS[idx]->syz = atof(strtok(NULL, delimit)) ;
        pS[idx]->mat = atof(strtok(NULL, delimit)) ;
        pS[idx]->mass= atof(strtok(NULL, delimit)) ;
        idx++;
    }
    return idx;
}


int main(){


    snowflake* a = initSnowflake(0, 0, 0, -1);
    srand(time(NULL));


    int fd;
    if((fd = open("zeroFileSoup.txt", O_WRONLY | O_CREAT |O_APPEND, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IROTH)) ==-1){
        perror("Open Fail");
        exit(EXIT_FAILURE);
    }



    double* tmp = gen_crystal(0);
    double* tmpContour = malloc(sizeof(double) * size*size);
    contour2D(tmp, tmpContour, size/2, size/2);
    import2DArr(a, tmpContour, size);
    
   

    const char delimit[2] = "\t";
    char buffer[256];
    double x,y,z;


    //how much to spread snowflakes for good looking bank


    int pointCount = 65536;
    point** pointSoup;//array of pointers to structs
    pointSoup = malloc(sizeof(point*) * pointCount);
    int cnt = getPointArr(pointSoup, "out.0.txt");
    printf("Finished reading file\n");

    for(int i = 0; i < cnt; i++){
        free(a->voxelSpace);
        import2DArr(a, tmpContour, size);
        if(i%50 == 0){
            printf("Now at idx:%d/%d \n", i, cnt);
        }
        setOrigin(a, pointSoup[i]->x, pointSoup[i]->y, pointSoup[i]->z);
        //rotate(a, rand()%180, rand()%10, rand()%10, rand()%10 );
        printLocal(a, fd);
    }



    

    free(tmp);
    free(tmpContour);
}



