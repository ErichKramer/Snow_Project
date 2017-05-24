#ifndef VERTEX_C
#define VERTEX_C

#include "vertex.h"


vertex* loadVal( int x, int y, int z){
    
    vertex* v = malloc(sizeof(vertex));
    v->x = x;
    v->y = y;
    v->z = z;   
}

void scaleV(vertex*, int scale){
    return;
}

void rotateV(vertex*, int rot){
    return;
}

void transV(vertex*, int tX, int tY, int tZ){
    return;
}



#endif
