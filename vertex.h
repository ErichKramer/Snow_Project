#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex vertex;
struct vertex{

    double x,y,z,w;

};

vertex* loadVal( int, int, int);

void scaleV(vertex*,  int );
void rotateV(vertex*, int );
void transV(vertex*,  int , int , int );


#endif
