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


void setOrigin(snowflake* s, int x, int y, int z){
    s->originX = x;
    s->originY = y;
    s->originZ = z;
}

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


void combineGeom(snowflake* a, snowflake* b){

    int fd;
    if ((fd = open("removed.txt", O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IROTH)) ==-1){
        perror("Open fail");
        exit(EXIT_FAILURE);
    }
    char buffer[100];
    char* bPoint = buffer;

    int sizeCube    = size*size*size;

    //9 times for a 3x3x3 cube

    int xDiff = b->originX - a->originX;
    int yDiff = b->originY - a->originY;
    int zDiff = b->originZ - a->originZ;
    
    int vecDiff = xDiff + yDiff*size + zDiff*size*size;
    int currLoc=0;

    
    for(int i = 0; i < sizeCube; i++){
        if(vecDiff+i > 0 && vecDiff+i < sizeCube){

            if(b->voxelSpace[i]==-1){
                a->voxelSpace[i+vecDiff] = -1;
            }
            //if b has -1 it was already written to a and this is redundant
            if(a->voxelSpace[i+vecDiff] == -1){
                b->voxelSpace[i] = a->voxelSpace[i+vecDiff];
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
}


//Finished implementation
int boxCollide(snowflake* a, snowflake* b){
    
    if( (a->xMax- b->xMin)*(a->xMin - b->xMax) < 0   ){
        if((a->yMax- b->yMin)*(a->yMin - b->yMax) < 0 ){
            if( (a->zMax- b->zMin)*(a->zMin - b->zMax) < 0 ){
 //               combineGeom(a, b);
             
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


    s->voxelSpace = malloc(sizeof(double) * size*size*size);
    int z=0;
    int idx;
    int cubeSize = size*size; //prevent excessive mul, used for z
    int centerPlane = size/2;//center plane of sflake

    for( int i = 0; i < size; i++){

        //formula for z value
        for(int j = 0; j < size; j++){

            if(z = arr[j+ i*size] ){
                for(int n = 0; n < z-1; n++){
                    s->voxelSpace[j + i*size + (centerPlane +(int)n/2)*cubeSize ]=-1;
                    s->voxelSpace[j + i*size + (centerPlane - (int)n/2)*cubeSize ]=-1;
                }
                s->voxelSpace[j + i*size + (centerPlane +z/2)*cubeSize ]=1;
                s->voxelSpace[j + i*size + (centerPlane -z/2)*cubeSize ]=1;

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
    
    s->voxCubeLen = size;
}

void updateMaxMin(snowflake* s){
    assert(s->voxelSpace != NULL);
    int x,y,z;
    
    int yMax, xMax, zMax = 0;
    for( z = 0; z  < s->voxCubeLen; z++){
        for( y = 0; y < s->voxCubeLen; y++){
            for(x = 0; x < s->voxCubeLen; x++){
                if(s->voxelSpace[z*size*size + y*size + x]){
                    if(x > xMax){
                        xMax = x;
                    }
                    if(y > yMax){
                        yMax = y;
                    }
                    if(z > zMax){
                        zMax = z;
                    }

                }
            }
        }


    }
    //for each plane scan for a valid z value, stop if found
        
//flip a bool when going from hit to not hit
}

/*************************************************
 *
 *Snowflake Translation operations
 *
 ************************************************/
void rotate(snowflake* sflake, double angle, double rX, double rY, double rZ){
    vertex** v = sflake->vertexSoup;

    vertex* rot = loadVal(rX, rY, rZ);   
    normalize(rot);//vertex operation from vertex.h, it's vectors all the way down

//atoi for floats    printf("rX %f rY %f rZ %f \n", rX, rY, rZ);


    rX = rot->x;
    rY = rot->y;
    rZ = rot->z;
//    printf("rotX = %f, rotY %f, rotZ %f\n",rot->x ,rot->y ,rot->z );

    double xTmp,yTmp,zTmp = 0; // temp values for conservation of values
    angle = angle * M_PI/180;
    double s = sin(angle);
    double c = cos(angle);
    double t = 1-c;

    double Mat[] = {
        rX*rX*t + c, rY*rX*t + rZ*s, rZ*rX*t - rY*s, 0,
        rX*rY*t - rY*s, rY*rY*t + c, rZ*rY*t + rX*s, 0,
        rX*rZ*t + rY*s, rY*rZ*t - rX*s, rZ*rZ*t + c, 0,
        0           , 0             , 0             ,1
    };
    

    for(int i = 0; i < sflake->vertCount; i++){
        xTmp = v[i]->x;
        yTmp = v[i]->y;
        zTmp = v[i]->z;

        v[i]->x = xTmp*Mat[0] + yTmp*Mat[1] + zTmp*Mat[2];
        v[i]->y = xTmp*Mat[4] + yTmp*Mat[5] + zTmp*Mat[6];
        v[i]->z = xTmp*Mat[8] + yTmp*Mat[9] + zTmp*Mat[10];

    }
    return;
}

void scale(){

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


void printLocal(snowflake* s, char* file){

    int fd;
    if((fd = open(file, O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH|S_IROTH)) ==-1){
        perror("Open Fail");
        exit(EXIT_FAILURE);
    }

    printf("fd inside printLocal: %d\n", fd);

    int i;
    write_file3D(fd, s, size);//this is breaking

    for( i = 0; i < s->neighSize; i++){
        write_file3D(fd, s->neighborCollisions[i], size);
    }

    close(fd);
}


//write from a 3 space array into 3 space coordinates
void write_file3D(int fd,  snowflake* s, int lsize ){

    double* geom = s->voxelSpace;

    char buffer[100];
    char* bPoint = buffer;

    int x = lsize;
    int y = lsize;
    int z = lsize;

    printf("Origins: %f, %f, %f\n", s->originX, s->originY, s->originZ );


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



