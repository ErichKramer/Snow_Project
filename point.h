#ifndef POINT_H
#define POINT_H

struct point{
    double x,y,z;
    double sxx,syy,szz;
    double sxy,sxz,syz;
    double mat, mass;
};


typedef struct point point;

#endif
