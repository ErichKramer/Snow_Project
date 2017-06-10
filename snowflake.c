#ifndef SNOWFLAKE_C
#define SNOWFLAKE_C

#include "snowflake.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "contour.h"
#include "crystal_phase.h"


extern int size;




//initialize values based on params
snowflake* initSnowflake(int x, int y, int z, int idx){
    snowflake* s = malloc(sizeof(snowflake));
    s->originX = x;
    s->originY = y;
    s->originZ = z;
    s->idx = idx;

    s->voxCubeLen = 0;
    s->neighSize = 0;
    s->voxelSpace = NULL;   
    s->neighborCollisions = malloc(sizeof(void*)* 32);

    s->xMax = 0;
    s->yMax = 0;
    s->zMax = 0;
    s->xMin = size;
    s->yMin = size;
    s->zMin = size;
    s->sX = s->sY = s->sZ = 10;

}


/*SNOWFLAKE OPERATIONS*/


void setOrigin(snowflake* s, double x, double y, double z){
    s->originX = x;
    s->originY = y;
    s->originZ = z;
}



/* DEPRECATED CURRENTLY. SEE COLLIDE FUNCTION
//set the bounds
void setEllipses(snowflake* s, int x, int y, int z){

    s->eX = x; s->eY = y; s->eZ = z;

    s->xMin = s->originX - x;
    s->xMax = s->originX + x;

    s->yMin = s->originY - y;
    s->yMax = s->originY + y;   

    s->zMin = s->originZ - z;
    s->zMax = s->originZ + z;
}
*/

void combineGeom(snowflake* a, snowflake* b){

    assert(a->voxCubeLen == b->voxCubeLen);

    int fd;
    if ((fd = open("removed.txt", O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IROTH)) ==-1){
        perror("Open fail");
        exit(EXIT_FAILURE);
    }
    char buffer[100];
    char* bPoint = buffer;

    
    int sizeCube    = a->voxCubeLen;

    //9 times for a 3x3x3 cube

    int xDiff = b->originX - a->originX;
    int yDiff = b->originY - a->originY;
    int zDiff = b->originZ - a->originZ;
    
    int vecDiff = xDiff + yDiff*size + zDiff*size*size;
    int currLoc=0;

    int counter = 0;
    for(int i = 0; i < sizeCube; i++){
        if(vecDiff+i > 0 && vecDiff+i < sizeCube){

            if(b->voxelSpace[i]==-1){
                a->voxelSpace[i+vecDiff] = -1;
                counter++;
            }
            //if b has -1 it was already written to a and this is redundant
            if(a->voxelSpace[i+vecDiff] == -1){
                b->voxelSpace[i] = a->voxelSpace[i+vecDiff];
                counter++;
            }
            
        }
    }
    


    /* WRITE REMOVED GEOMETRIES*/
    for(int z = 0; z<size; z++){
        for(int y = 0; y<size; y++){
            for(int x = 0; x<size; x++){
                //this finds the correct number of conflictes to match above
                currLoc = x + y*size + z*size*size;
                if(vecDiff+currLoc > 0 && vecDiff+currLoc < sizeCube){                   
                    if((b->voxelSpace[currLoc] == -1 && a->voxelSpace[currLoc+vecDiff] == 1)
                            || (a->voxelSpace[currLoc+vecDiff] == -1 && b->voxelSpace[currLoc] == 1)){
                        a->voxelSpace[currLoc+vecDiff] = -1;
                        b->voxelSpace[currLoc] = -1;
                        sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n",
                                (float)x/10, (float)y/10, (float)z/10 );

                        if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                            perror("Write to file: ");
                            exit(EXIT_FAILURE);
                        }
                    }
                }   
            }
        }
    }
    printf("Removed %d \n", counter);
}


//Finished implementation
int boxCollide(snowflake* a, snowflake* b){
    
    if( (a->xMax- b->xMin)*(a->xMin - b->xMax) < 0   ){
        if((a->yMax- b->yMin)*(a->yMin - b->yMax) < 0 ){
            if( (a->zMax- b->zMin)*(a->zMin - b->zMax) < 0 ){
                combineGeom(a, b);
             
                a->neighborCollisions[a->neighSize++] = b;
                b->neighborCollisions[b->neighSize++] = a;

                return 1;
            }
        }
    }
    return 0;

    //include methods for 
}





//construct voxel shell from x,y cooords
void import2DArr(snowflake* s, double* arr, int size ){


    s->voxCubeLen = size*size*size;
    s->voxelSpace = malloc(sizeof(double) * s->voxCubeLen);
    int z=0;
    int idx;
    int squareSize = size*size; //prevent excessive mul, used for z
    int centerPlane = size/2;//center plane of sflake

    for( int i = 0; i < size; i++){

        //formula for z value
        for(int j = 0; j < size; j++){

            if(z = arr[j+ i*size] ){
                for(int n = 0; n < z-1; n++){
                    s->voxelSpace[j + i*size + (centerPlane +(int)n/2)*squareSize ]=-1;
                    s->voxelSpace[j + i*size + (centerPlane - (int)n/2)*squareSize ]=-1;
                }
                s->voxelSpace[j + i*size + (centerPlane +z/2)*squareSize ]=1;
                s->voxelSpace[j + i*size + (centerPlane -z/2)*squareSize ]=1;

                if( i < s->xMin) s->xMin = i;
                if( i > s->xMax) s->xMax = i;
                if( j < s->yMin) s->yMin = j;
                if( j > s->yMax) s->yMax = j;
                if( z > s->zMax) {
                    s->zMin = -z;
                    s->zMax =  z;
                }
            }
        }
    }

    s->xMin += s->originX;
    s->yMin += s->originY;
    s->zMin += s->originZ;

    s->xMax += s->originX;
    s->yMax += s->originY;
    s->zMax += s->originZ;
    
}

void updateMaxMin(snowflake* s){
    assert(s->voxelSpace != NULL);
    int x,y,z;
    
    int yMax, xMax, zMax, xMin, yMin, zMin = 0;
    for( z = 0; z  < size; z++){
        for( y = 0; y < size; y++){
            for(x = 0; x < size; x++){
                if(s->voxelSpace[z*size*size + y*size + x]){
                    if(x > xMax){
                        xMax = x;
                    }
                    if(x < xMin){
                        xMin = x;
                    }
                    if(y > yMax){
                        yMax = y;
                    }
                    if(y < yMin){
                        yMin = y;
                    }
                    if(z > zMax){
                        zMax = z;
                    }
                    if( z < zMin){
                        zMin = z;
                    }//make this less bad at some point

                }
            }
        }
    }
    s->xMax = xMax; s->xMin = xMin;   
    s->yMax = yMax; s->yMin = yMin;
    s->zMax = zMax; s->zMin = zMin;
}

/*************************************************
 *
 *Snowflake Translation operations
 *
 ************************************************/
void rotate(snowflake* sflake, double angle, double rX, double rY, double rZ){
    double* voxSpace = sflake->voxelSpace;


//    printf("Before normaling: rX %f, rY %f, rZ %f\n", rX, rY, rZ);

    double magnitude = 0.;

    magnitude = sqrt(rX * rX + rY * rY + rZ * rZ);
    rX /= magnitude;
    rY /= magnitude;
    rZ /= magnitude;
   
//    printf("After Normasl: rX %f, rY %f, rZ %f, mag %f\n", rX, rY, rZ, magnitude);
    

    //double xTmp,yTmp,zTmp = 0; // temp values for conservation of values
    angle = angle * M_PI/180;
    double s = sin(angle);
    double c = cos(angle);
    double t = 1-c;

//    printf("sin %f,cos  %f,tan %f\n", s, c, t);

    double Mat[] = {
        rX*rX*t + c, rY*rX*t + rZ*s, rZ*rX*t - rY*s, 0,
        rX*rY*t - rY*s, rY*rY*t + c, rZ*rY*t + rX*s, 0,
        rX*rZ*t + rY*s, rY*rZ*t - rX*s, rZ*rZ*t + c, 0,
        0           , 0             , 0             ,1
    };
//    for(int i = 0; i < sizeof(Mat) / sizeof(Mat[0]); i++){
//        printf("Value i = %d , %f\n", i, Mat[i]);
//    }   


    double* newCube = malloc(sizeof(double) * sflake->voxCubeLen);
    int xTmp, yTmp, zTmp;

    int sizeSqr = size*size;//prevent extraneous mul

    int offset = size*size*size /2 + size*size/2 + size/2;

    for(int k = -size/2; k<size/2; k++){
        for(int j = -size/2; j<size/2; j++){
            for(int i = -size/2; i<size/2; i++){


               // printf("Size3 = %d, current = %d\n", size*size*size,
               //         i + j*size + k*sizeSqr + offset);

                if(voxSpace[i + j*size + k*sizeSqr + offset]){
                    //cast to int and add .5 to round up or down 
                    xTmp = (int)(i*Mat[0] + j*Mat[1] + k*Mat[2] + .5);
                    yTmp = (int)(i*Mat[4] + j*Mat[5] + k*Mat[6] + .5);
                    zTmp = (int)(i*Mat[8] + j*Mat[9] + k*Mat[10]+ .5);
                    
//                    printf("New vals: %d, %d, %d\n", xTmp, yTmp, zTmp);
                    
     
                    newCube[xTmp + yTmp*size + zTmp*sizeSqr + offset] = 
                        voxSpace[i + j*size + k*sizeSqr + offset];
    
                }
            }
        }
    }
//    printf("before free\n");

    free(sflake->voxelSpace);
    sflake->voxelSpace = newCube;

    updateMaxMin(sflake);
}
   

void scale(snowflake* s, double sX, double sY, double sZ){

    if(sX)
        s->sX *= sX;
    if(sY)
        s->sY *= sY;
    if(sZ)
        s->sZ *= sZ;

}

void translate(snowflake* s, double tX, double tY, double tZ){
    s->originX +=tX; s->originY += tY; s->originZ += tZ;
}







/*************************************************
 *
 *LOGGING FUNCTIONS
 *
 ************************************************/


void displayExtreme(snowflake* s){

    printf("Min X: %f Max X: %f \n", s->xMin, s->xMax);
    printf("Min Y: %f Max Y: %f \n", s->yMin, s->yMax);
    printf("Min Z: %f Max Z: %f \n", s->zMin, s->zMax);


}


void printLocal(snowflake* s, int fd){



    int i;
    write_file3D(fd, s, size);//this is breaking

    for( i = 0; i < s->neighSize; i++){
    //    write_file3D(fd, s->neighborCollisions[i], size);
    }

}


//write from a 3 space array into 3 space coordinates
void write_file3D(int fd,  snowflake* s, int lsize ){

    double* geom = s->voxelSpace;

    char buffer[100];
    char* bPoint = buffer;

    int x = lsize;
    int y = lsize;
    int z = lsize;

//    printf("Origins: %f, %f, %f\n", s->originX, s->originY, s->originZ );


    for(int i = 0; i < z; i++){
        for( int j = 0; j< y; j++){
            for( int k = 0; k < x; k++){

                if( geom[lsize*lsize*i + lsize*j + k] == 1){
                    sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n",
                            ((float)k + s->originX)/s->sX, ((float)j + s->originY)/s->sY,
                            ((float)i + s->originZ)/s->sZ  );

                    if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                        perror("Write to file: ");
                        exit(EXIT_FAILURE);
                    }

                }
            }
        }
    }
}


void write_file2D(int fd, double* geom, int lsize){

    char buffer[100];
    char* bPoint = buffer;


    for( int y = 0; y < lsize; y++){
        for( int x = 0; x< lsize; x++){
            if(geom[x + y*lsize]){

                sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n",
                            (float)x/10, (float)y/10, geom[x + y*lsize]);

                    if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                        perror("Write to file: ");
                        exit(EXIT_FAILURE);
                    }

                sprintf(bPoint, "%f\t%f\t%f\t0\t0\t0\t0\t0\t0\t0\t0\n",
                            (float)x/10, (float)y/10, -geom[x + y*lsize]);

                    if(write(fd, bPoint, sizeof(char) * strlen(bPoint)) ==-1){
                        perror("Write to file: ");
                        exit(EXIT_FAILURE);
                    }
            }

        }

    }

}







/*
Allow for input of a zed max so that we can scale along contours 
for this max value.

change extension to .out instead
*/



#endif



