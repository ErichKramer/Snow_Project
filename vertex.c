#ifndef VERTEX_C
#define VERTEX_C

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vertex.h"

extern int size;

vertex* loadVal( double x, double y, double z){
    
    vertex* v = malloc(sizeof(vertex));

    v->x = x;
    v->y = y;
    v->z = z;   
    v->w = 1;
    return v;
}

void normalize(vertex* v){

    double magnitude = sqrt( pow(v->x, 2)  + pow(v->y, 2) + pow(v->z, 2));
    printf("Magnitude:%f\n", magnitude);
    v->x = v->x / magnitude;
    v->y = v->y / magnitude;
    v->z = v->z / magnitude;

}

void scaleV(vertex* v, double scale){
    
    v->x *= scale;
    v->y *= scale;
    v->z *= scale;

    return;
}

void rotateV(vertex* v, double angle, double rX, double rY, double rZ){

    angle = angle * M_PI/180;

    double s = sin(angle);
    double c = cos(angle);
    double t = 1-c;

    double x = v->x;
    double y = v->y;
    double z = v->z;


    double Mat[] = {
        rX*rX*t + c, rY*rX*t + rZ*s, rZ*rX*t - rY*s, 0,
        rX*rY*t - rY*s, rY*rY*t + c, rZ*rY*t + rX*s, 0,
        rX*rZ*t + rY*s, rY*rZ*t - rX*s, rZ*rZ*t + c, 0, 
        0           , 0             , 0             ,1
    };

    v->x = x * Mat[0] + y * Mat[1] + z * Mat[2];
    v->y = x * Mat[4] + y * Mat[5] + z * Mat[6];
    v->z = x * Mat[8] + y * Mat[9] + z * Mat[10];
   


    return;
}

void transV(vertex* v, double tX, double tY, double tZ){
 

    return;
}



#endif
