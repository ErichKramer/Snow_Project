#ifndef VERTEX_C
#define VERTEX_C

#include <stdlib.h>
#include <stdio.h>
#include "vertex.h"


vertex* loadVal( int x, int y, int z){
    
    vertex* v = malloc(sizeof(vertex));

    v->x = x;
    v->y = y;
    v->z = z;   
    v->w = 1;
    return v;
}

void scaleV(vertex* v, int scale){
    return;
}

void rotateV(vertex* v, int rot){
    return;
}

void transV(vertex* v, int tX, int tY, int tZ){
    return;
}



#endif
