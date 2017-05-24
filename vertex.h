#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex vertex;
struct vertex{

    double x,y,z,w;

};


void scaleV(vertex*,  int scale);
void rotateV(vertex*, int rot);
void transV(vertex*,  int tX, int tY, int tZ);


#endif
