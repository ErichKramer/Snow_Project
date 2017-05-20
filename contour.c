#ifndef CONTOUR_C
#define CONTOUR_C

#include "contour.h"

#include <stdio.h>
#include <stdlib.h>

//variables assigned in crystal_phase
extern int size;


//threshold parameter for acceptable geometry phase
double phase_tol = .99;

//init dest to 0 using helper
int contour2D(double *src, double *dest, int x, int y){


    //out of bounds case
	if(x < 0 || y < 0 || x >= size || y >= size)
		return -1;

    //dest [place] > 0? check for negative first in this case
    //if this spot is solved, don't solve again, return it.
    if(dest[y*size +x] > 0){
		return dest[y*size +x];
	}

    //if active return active, (same as oob)
	if(dest[y*size +x]==-1){
		return -1;
	}

    //must be here to close recursion
	if(src[y*size+x] < phase_tol){
		return dest[y*size +x] = 0;
	}

	dest[y*size +x] = -1;//set active if all cases pass

	int min = size*size;
	int tmp;

	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){

			tmp = contour2D(src, dest, x+j, y+i);
            //if not active or out of bounds and ...
			if( tmp != -1 && tmp < min){
				min = tmp;
			}
			if(tmp ==0)
				return dest[y*size +x] = min+1;

		}
	}
//	src[y*size +x] = 1;
	return (dest[y*size +x] = min+1);
	
}


void printArray(double *a){

	for(int i = 0; i < size; i++){
		for(int j = 100; j< 200; j++){
			printf("%f ",a[i*size+j]);
		}
		printf("\n%d\n",i);
	}
}




#endif
