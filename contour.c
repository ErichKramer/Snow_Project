#include <stdio.h>
#include <stdlib.h>

//variables assigned in crystal_phase
int size;
double phase_tol;
//dest is init to 0, src is the double array
int contour(double *src, double *dest, int x, int y){

	if(x < 0 || y < 0 || x >= size || y >= size)//out of bounds case
		return -1;//same case for when active if oob

	if(dest[y*size +x] > 0){//if this spot is solved
		return dest[y*size +x];//don't solve again, return it.
	}

	if(dest[y*size +x]==-1){//if active return active
		return -1;
	}

	if(src[y*size+x] < phase_tol){//must be here to close recursion
		return dest[y*size +x] = 0;
	}

	dest[y*size +x] = -1;//set active if all cases pass

	int min = size*size;
	int tmp;

	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){

			tmp = contour(src, dest, x+j, y+i);
			if( tmp != -1 && tmp < min){
				min = tmp;
			}
			if(tmp ==0)
				return dest[y*size +x] = min+1;

		}
	}
	src[y*size +x] = 1;
	return (dest[y*size +x] = min+1);
	
}

//[ y*10 + x]

void printArray(double *a){

	for(int i = 0; i < size; i++){
		for(int j = 0; j< size; j++){
			printf("%f ",a[i*size+j]);
		}
		printf("\n%d\n",i);
	}
}

/*
int main(){


	int* arr 	= malloc(sizeof(int) * size*size);
	int* lvl 	= malloc(sizeof(int)	* size*size);
	
	for(int i = 0; i< size*size; i++){
		if( i %size > size*.1 && i%size < size*.8 && i > 2*size && i < 8*size)
			lvl[i] = 1;
		else
			lvl[i] = 0;
		arr[i] = 0;
	}
	
	contour(arr, lvl, size/2, size/2);
	printArray(lvl);

}
*/




