#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex vertex;
struct vertex{

    double x,y,z,w;

};

vertex* loadVal( double, double, double);


void normalize(vertex*);
void scaleV(vertex*,  double );
void rotateV(vertex*, double, double , double , double );
void transV(vertex*,  double , double , double );


#endif
