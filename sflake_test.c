
#include "snowflake.h"
#include "crystal_phase.h"


#include <stdio.h>


int main(){

    snowflake* a = initSnowflake(0, 0, 0, -1);
    snowflake* b = initSnowflake(1, 1, 1, -1);
    printf("Init success\n");
    double* tmp = data_log(0);
    double* tmpContour = malloc(sizeof(double) * size*size);
    contour2D(tmp, tmpContour, size/2, size/2);

//    printArray(tmpContour);

    printf("Array  success\n");
    import2DArr(a, tmpContour, size);

    printf("Import Success\n");
    
    //    double* tmp2 = data_log(0);

    
//    b = import2DArr(b, tmp, size);
    printf("Check Beta\n");

    int fd;
    if(fd = open("file.txt", O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IROTH) ==-1 ){
        perror("Open Fail");
        exit(EXIT_FAILURE);
    }


//    log_python(a->voxelSpace, fd);
    
/*
    if(boxCollide(a, b)){
        printf("Collision");
    }
*/
    close(fd);
    free(tmp);
    free(tmpContour);
    //   free(tmp2);
}



