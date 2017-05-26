#ifndef VERTEX_C
#define VERTEX_C

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vertex.h"

extern int size;

vertex* loadVal( int x, int y, int z){
    
    vertex* v = malloc(sizeof(vertex));

    v->x = x-size/2.0;
    v->y = y-size/2.0;
    v->z = z;   
    v->w = 1;
    return v;
}

void scaleV(vertex* v, int scale){
    
    v->x *= scale;
    v->y *= scale;
    v->z *= scale;

    return;
}

void rotateV(vertex* v, double angle, int rX, int rY, int rZ){

    angle = angle * M_PI/180;

    double s = sin(angle);
    double c = cos(angle);
    double t = 1-c;

    double x = v->x;
    double y = v->y;
    double z = v->z;


    int Mat[] = {
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

void transV(vertex* v, int tX, int tY, int tZ){
    return;
}



#endif
